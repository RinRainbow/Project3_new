#define MAX_X 101
#define PIXEL 10

#include <QDebug>

#ifndef CURVEMANAGER_H
#define CURVEMANAGER_H

#include <QWidget>
#include <QMainWindow>
#include "qcustomplot.h"

class Curve;

class CurveManager
{
public:
    CurveManager();
    CurveManager(QCustomPlot *plot);
    void addCurve(Curve newCurve);
    void plotCurve();
    void clearCurve();
    void resetView();

private:
    QCustomPlot *plot;
    int curveAmount;
    QVector<Curve> CurveList;

    QVector<QColor> COLOR = { Qt::blue, Qt::black, Qt::red, Qt::yellow,
                              Qt::green, Qt::cyan, Qt::darkRed, Qt::darkBlue,
                              Qt::darkCyan, Qt::darkGray, Qt::magenta, Qt::darkYellow };
};

#endif // CURVEMANAGER_H


#ifndef CURVE_H
#define CURVE_H

class Curve{
public:
    Curve();
    Curve(QColor setColor);
    Curve(const Curve &);
    void setId(int newId);


    void Csin();
    void Ccos();
    void Ctan();
    void Cpow(int n);
    void Cline(int m);
    void Ccircle();
    void CmoveX(int n);
    void CmoveY(int n);

    int id;
    bool showable;
    QColor color;
    QVector<double> x;
    QVector<double> y;
    QVector<QColor> ColorList = { Qt::blue, Qt::black, Qt::red, Qt::yellow,
                              Qt::green, Qt::cyan, Qt::darkRed, Qt::darkBlue,
                              Qt::darkCyan, Qt::darkGray, Qt::magenta, Qt::darkYellow };
};

#endif // CURVE_H
