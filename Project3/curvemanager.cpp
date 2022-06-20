#include "curvemanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

CurveManager::CurveManager(QCustomPlot* plot) :
	plot(plot) {
	// QcustomPlot setup
	plot->xAxis->setLabel("x");
	plot->yAxis->setLabel("y");
	plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


	// Curve setup
	curveAmount = 0;
	//CurveList.clear();
}

void CurveManager::addCurve(QString input) {
	EquationParser equation;
	//equation.setPrecision(0.001);
	//equation.setRange(10);
	try {
		equation.setString(input.toStdString());
		plot->addGraph();
		equation.id = curveAmount;
		plot->graph(equation.id)->setPen(COLOR[qrand() % COLOR.size()]);
		EquationList.push_back(equation);
		curveAmount++;
	}
	catch (exception &e){
		qDebug() << e.what() << "\n";
	}
}

void CurveManager::plotCurve() {
	for (int countCurve = 0; countCurve < curveAmount; countCurve++) {
		const EquationParser& eTmp = EquationList[countCurve];
		plot->graph(eTmp.id)->setData(eTmp.xVec, eTmp.yVec);
	}
	plot->replot();
}

void CurveManager::clearCurve() {
	QVector<double> x, y;
	x.clear();
	y.clear();
	EquationList.clear();
	for (int countCurve = 0; countCurve < curveAmount; countCurve++) {
		plot->graph(countCurve)->setData(x, y);
	}
	plot->replot();
}

void CurveManager::resetView() {
	plot->xAxis->setRange(-10, 10);
	plot->yAxis->setRange(-10, 10);
	plot->replot();
}



// ======= Curve =======
/*
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

*/