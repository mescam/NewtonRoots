#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextBlock>
#include <QMessageBox>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include "Interval/IntervalArithmetic.h"
#include "mathematics.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ldsize->setText(QString::number(sizeof(long double)) + "B");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calcButton_clicked()
{
    //checking radio buttons
    bool isIntervalArithmetic;
    if(this->ui->standardRadio->isChecked()) {
        qDebug() << "Floating point arithemtic";
        isIntervalArithmetic = false;
    }else{
        qDebug() << "Interval arithmetic";
        isIntervalArithmetic = true;
    }

    qDebug() << "Validating fields";
    if(!this->validateFields(isIntervalArithmetic)) {
        QMessageBox::warning(this, "Błąd", "Wprowadzono niepoprawne dane.");
        return;
    }

    // things we know for sure
    unsigned int maxIter = this->ui->maxIter->text().toInt();
    char st;
    unsigned int it;
    long double epsilon = boost::lexical_cast<long double>(this->ui->eps->text().toStdString());

    // check degree of the polynomial
    unsigned int degree;
    QTextDocument *doc = ui->params->document();
    degree = doc->blockCount() - 1;
    qDebug() << "Detected polynomial of" << degree << "degree";

    if(!isIntervalArithmetic) { //if it's a floating point
        //preparations
        long double *vec = getVectorOfParams(degree);
        long double approx = boost::lexical_cast<long double>(this->ui->approx->text().toStdString());
        long double w, res;

        //calculating
        res = Math::newtonRoots(degree, vec, approx, maxIter, epsilon, w, it, st);
        qDebug() << "function value" << (double) w;

        //show results
        this->ui->result->setText(QString::fromStdString(boost::lexical_cast<std::string>(res)));
        if(st == 0 || st == 3) {
            this->ui->value->setText(QString::fromStdString(boost::lexical_cast<std::string>(w)));
        }else{
            this->ui->value->setText(QString("Nie dotyczy"));
        }
    }else{ //interval arithmetics
        intervalarth::interval *vec,
                approx,
                w,
                res;
        approx = Math::readInterval(this->ui->approx->text().toStdString());
        vec = intervalGetVectorOfParams(degree);

        //calc
        res = Math::intervalNewtonRoots(degree, vec, approx, maxIter, epsilon, w, it, st);

        this->ui->result->setText(QString::fromStdString(intervalToString(res)));
        if(st == 0 || st == 3) {
            this->ui->value->setText(QString::fromStdString(intervalToString(w)));
        }else{
            this->ui->value->setText(QString("Nie dotyczy"));
        }
    }

    //and the common part
    QString statusText;
    switch(st)
    {
    case 0:
        statusText += "Powodzenie\n\n";
        break;
    case 1:
        statusText += "Niepowodzenie:\nbłędny stopień wielomianu lub maksymalna liczba iteracji\n\n";
        break;
    case 2:
        statusText += "Niepowodzenie:\ndzielenie przez zero podczas obliczeń\n\n";
        break;
    case 3:
        statusText += "Niepowodzenie:\nnie osiągnięto pożądanej dokładności obliczeń\n\n";
        break;
    }

    this->ui->doneIter->setText(QString::number(it));
    this->ui->status->setText(statusText);
}

bool MainWindow::validateInterval(std::string n)
{
    if(validateFloatingPoint(n)) {
        qDebug() << "was okay as fp";
        // we can create an interval so it is okay for us
        return true;
    }else{
        std::vector<std::string> tokens;
        boost::split(tokens, n, boost::is_any_of(" "));
        for(auto it = tokens.begin(); it != tokens.end(); it++) {
            qDebug() << QString::fromStdString(*it);
        }
        if(tokens.size() == 2 && validateFloatingPoint(tokens[0]) && validateFloatingPoint(tokens[1])) {
            qDebug() << "okay as interval";
            return true;
        }
    }
    qDebug() << "sorry, it was wrong";
    return false;
}

bool MainWindow::validateFloatingPoint(std::string n)
{
    try {
        boost::lexical_cast<long double>(n);
    }catch(boost::bad_lexical_cast){
        return false;
    }
    return true; // :D
}

bool MainWindow::validateField(std::string n, bool isInterval) {
    if(!isInterval) {
        return validateFloatingPoint(n);
    }else{
        return validateInterval(n);
    }
}

bool MainWindow::validateFields(bool isInterval)
{
    qDebug() << "Validating fields func" << isInterval;
    if(validateFloatingPoint(this->ui->eps->text().toStdString()) &&
       validateField(this->ui->approx->text().toStdString(), isInterval) &&
       validateFloatingPoint(this->ui->maxIter->text().toStdString()))
    {
        qDebug() << "common fields are okay, checking document";
        QTextDocument *doc = this->ui->params->document();
        for(auto i = 0; i < doc->blockCount(); i++) {
            if(!validateField(doc->findBlockByLineNumber(i).text().toStdString(), isInterval))
            {
                qDebug() << "line " << i << "was not okay";
                return false;
            }
        }
    }else{
        qDebug() << "common fields were not okay";
        return false;
    }
    return true;
}

long double *MainWindow::getVectorOfParams(unsigned int degree)
{
    long double *vec = new long double[degree+1];
    QTextDocument *doc = this->ui->params->document();
    for(unsigned int i = 0; i <= degree; i++) {
        try {
            vec[i] = boost::lexical_cast<long double>(doc->findBlockByLineNumber(i).text().toStdString());
        }catch(...){
            qDebug() << "Invalid number on line" << i;
            delete vec;
            return nullptr;
        }

        qDebug() << "a[" << i << "] = " << (double) vec[i];
    }
    return vec;
}

intervalarth::interval *MainWindow::intervalGetVectorOfParams(unsigned int degree)
{
    intervalarth::interval *vec = new intervalarth::interval[degree+1];
    intervalarth::IntervalArithmetic ia;
    QTextDocument *doc = this->ui->params->document();
    for(unsigned int i = 0; i <= degree; i++) {
        vec[i] = Math::readInterval(doc->findBlockByLineNumber(i).text().toStdString());
        qDebug() << "a[" << i << "] =" << QString::fromStdString(intervalToString(vec[i]));
    }
    return vec;
}

std::string MainWindow::intervalToString(intervalarth::interval interval)
{
    return "[" + boost::lexical_cast<std::string>(interval.a) + "; " + boost::lexical_cast<std::string>(interval.b) + "]";
}

