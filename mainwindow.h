#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Interval/IntervalArithmetic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_calcButton_clicked();

private:
    Ui::MainWindow *ui;
    bool validateFields(bool isInterval);
    bool validateField(std::string n, bool isInterval);
    bool validateInterval(std::string n);
    bool validateFloatingPoint(std::string n);
    long double *getVectorOfParams(unsigned int degree);
    intervalarth::interval *intervalGetVectorOfParams(unsigned int degree);
    std::string intervalToString(intervalarth::interval interval);
};

#endif // MAINWINDOW_H
