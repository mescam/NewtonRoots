#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextBlock>
#include <boost/lexical_cast.hpp>
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
        qDebug() << "Normal arithemtic";
        isIntervalArithmetic = false;
    }else{
        qDebug() << "Interval arithmetic";
        isIntervalArithmetic = true;
    }

    qDebug() << "Validating fields";
    if(!this->validateFields(isIntervalArithmetic)) {
        return;
    }

    long double approx, epsilon;
    unsigned int maxIter = this->ui->maxIter->text().toInt();
    try {
        approx = std::stold(this->ui->approx->text().toStdString());
        epsilon = std::stold(this->ui->eps->text().toStdString());
    }catch(std::invalid_argument){
        qDebug() << "Invalid argument in approx or epsilon";
        return;
    }
    unsigned int degree;
    QTextDocument *doc = ui->params->document();
    degree = doc->blockCount() - 1;
    qDebug() << "Detected polynomial of" << degree << "degree";
    long double *vec = getVectorOfParams(degree);

    long double w;
    unsigned int it;
    char st;
    long double res = Math::newtonRoots(degree, vec, approx, maxIter, epsilon, w, it, st);
    this->ui->result->setText(QString::fromStdString(boost::lexical_cast<std::string>(res)));
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

    if(st == 0 || st == 3) {
        statusText += "Wartość funkcji: " + QString::fromStdString(boost::lexical_cast<std::string>(w)) + "\n";
    }
    statusText += "Przeprowadzonych iteracji: " + it;
    this->ui->status->setText(statusText);
}

bool MainWindow::validateFields(bool isInterval)
{
    return true;
}

long double *MainWindow::getVectorOfParams(unsigned int degree)
{
    long double *vec = new long double[degree+1];
    QTextDocument *doc = this->ui->params->document();
    for(int i = 0; i <= degree; i++) {
        try {
            vec[i] = std::stold(doc->findBlockByLineNumber(i).text().toStdString());
        }catch(std::invalid_argument){
            qDebug() << "Invalid number on line" << i;
            delete vec;
            return nullptr;
        }

        qDebug() << "a[" << i << "] = " << (double) vec[i];
    }
    return vec;
}
