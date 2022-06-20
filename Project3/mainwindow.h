#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void drawerMouseMoved(QMouseEvent *event);

    //void toggleVisibility();

    void on_createFunction_clicked();

    void on_showMe_stateChanged();

    void on_inputFunction_editingFinished();

    void on_bye_clicked();

private:
    Ui::MainWindow *ui;
    QCustomPlot* customPlot;
    QCPItemTracer *phaseTracer;
    QCPItemText *phaseTracerText;
    int graphCount;
    std::vector<QCheckBox*> checkBox;
    std::vector<QLabel*> lab;
    std::vector<QLineEdit*> lineEdit;
    std::vector<QPushButton*> pushBotton;
    std::vector<QHBoxLayout*> hl;
};
#endif // MAINWINDOW_H
