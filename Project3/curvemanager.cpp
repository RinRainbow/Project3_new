#include "curvemanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

CurveManager::CurveManager(QCustomPlot *plot) :
    plot(plot)
{
    // QcustomPlot setup
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    // Curve setup
    curveAmount = 0;
    CurveList.clear();

}

void CurveManager::addCurve(Curve newCurve)
{
    newCurve.Ctan();
    newCurve.CmoveY(4);
    plot->addGraph();
    newCurve.id = curveAmount;
    CurveList.push_back(newCurve);
    curveAmount++;


    /*
    int randomColoridx = qrand() % COLOR.size();
    plot->graph(0)->setPen(QPen(COLOR[randomColoridx])); // line color blue for first graph

    // generate some data:
    int pixel = PIXEL;
    QVector<double> x(MAX_X * pixel), y(MAX_X * pixel); // initialize with entries 0..100

    for (int i=0; i<MAX_X * pixel; ++i)
    {
      x[i] = i/(MAX_X / pixel * 2.0) - pixel; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    plot->graph(0)->setData(x, y);

    // set axes ranges, so we see all data:
    plot->xAxis->setRange(-pixel, pixel);
    plot->yAxis->setRange(-pixel, pixel);
    plot->replot();
    */
}

void CurveManager::plotCurve()
{
    for (int countCurve = 0; countCurve < curveAmount; countCurve++) {
        Curve &CurveTmp = CurveList[countCurve];
        plot->graph(CurveTmp.id)->setData(CurveTmp.x, CurveTmp.y);
    }
    plot->replot();
}

void CurveManager::clearCurve()
{
    for (int countCurve = 0; countCurve < curveAmount; countCurve++) {
        CurveList[countCurve].showable = false;
    }
    QVector<double> x(MAX_X * PIXEL), y(MAX_X * PIXEL);
    x.clear();
    y.clear();
    plot->graph(0)->setData(x, y);
    plot->replot();
}

void CurveManager::resetView()
{
    plot->xAxis->setRange(-10, 10);
    plot->yAxis->setRange(-10, 10);
    plot->replot();
}



// ======= Curve =======

Curve::Curve()
{
    this->x.clear();
    this->y.clear();

    int randomColoridx = qrand() % ColorList.size();
    this->color = ColorList[randomColoridx];
    this->showable = true;

    x.resize(MAX_X * PIXEL);
    y.resize(MAX_X * PIXEL);
    for (int i=0; i<MAX_X * PIXEL; ++i)
    {
      x[i] = i/(MAX_X / 2.0) - PIXEL;
      y[i] = 0;
    }
}


Curve::Curve(QColor setColor)
{
    this->x.clear();
    this->y.clear();
    this->color = setColor;
    this->showable = true;
    x.resize(MAX_X * PIXEL);
    y.resize(MAX_X * PIXEL);
    for (int i=0; i<MAX_X * PIXEL; ++i)
    {
      x[i] = i/(MAX_X / 2.0) - PIXEL;
      y[i] = 0;
    }
}

Curve::Curve(const Curve & curve)
{
    this->x = curve.x;
    this->y = curve.y;
    this->color = curve.color;
    this->showable = curve.showable;
}

void Curve::setId(int newId)
{
    this->id = newId;
}

void Curve::Csin()
{
    for (int i=0; i<MAX_X * PIXEL; ++i) {
      y[i] += sin(x[i]);
    }
}

void Curve::Ccos()
{
    for (int i=0; i<MAX_X * PIXEL; ++i) {
      y[i] += cos(x[i]);
    }
}

void Curve::Ctan()
{
    for (int i=0; i<MAX_X * PIXEL; ++i) {
      y[i] += tan(x[i]);
      qDebug() << y[i] << "\n";
    }

}

void Curve::Cpow(int n)
{
    for (int i=0; i<MAX_X * PIXEL; ++i) {
        y[i] += x[i];
        for (int j=1; j<n; j++) {
            y[i] *= x[i];
        }
    }
}

void Curve::Cline(int m)
{

    for (int i=0; i<MAX_X * PIXEL; ++i) {
      y[i] += m * (x[i]);
    }
}

void Curve::CmoveX(int n)
{
    for (int i=0; i<MAX_X * PIXEL; ++i) {
      x[i] += n;
    }
}

void Curve::CmoveY(int n)
{
    for (int i=0; i<MAX_X * PIXEL; ++i) {
      y[i] += n;
    }
}
