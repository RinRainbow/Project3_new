#include "mainwindow.h"
#include "equationparser.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graphCount(-1)
{
    ui->setupUi(this);

    customPlot = ui->drawer;

    QDesktopWidget * deskwidget = QApplication::desktop();
    QRect deskgeo = deskwidget->screenGeometry();
    move((deskgeo.width() - width()) / 2, (deskgeo.height() - height()) / 2);

    /*
    // generate some data:
    QVector<double> x(999999), y0(999999), y1(999999); // initialize with entries 0..100
    for (int i=0; i<999999; ++i)
    {
      x[i] = i/50.0 - 999; // x goes from -1 to 1
      y0[i] = x[i]*x[i]; // let's plot a quadratic function
      y1[i] = qCos(i/10.0); // exponentially decaying cosine
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y0);
    customPlot->addGraph();
    customPlot->graph(1)->setData(x, y1);
    // set the color of the graph
    customPlot->graph(0)->setPen(QPen(Qt::green));
    customPlot->graph(1)->setPen(QPen(Qt::blue));
    */

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-100, 100);
    customPlot->yAxis->setRange(-100, 100);
    // set tick count
    customPlot->xAxis->ticker()->setTickCount(6);
    customPlot->yAxis->ticker()->setTickCount(6);

    /*
    // add phase tracer
    phaseTracer = new QCPItemTracer(customPlot);
    phaseTracer->setGraph(customPlot->graph(0));
    phaseTracer->setGraphKey(0.0);
    phaseTracer->setStyle(QCPItemTracer::tsCrosshair);
    phaseTracer->setPen(QPen(Qt::red));
    phaseTracer->setBrush(Qt::red);
    phaseTracer->setSize(7);
    connect(customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(drawerMouseMoved(QMouseEvent*)));

    // add label for phase tracer
    phaseTracerText = new QCPItemText(customPlot);
    phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    phaseTracerText->setFont(QFont(font().family(), 12));
    phaseTracerText->setPositionAlignment(Qt::AlignLeft);
    phaseTracerText->position->setCoords(QPointF(0, 0));
    phaseTracerText->setText("(-, -)");
    */

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    /*
    // add legend
    customPlot->setAutoAddPlottableToLegend(true);
    customPlot->legend->setVisible(true);

    for(int i = 0; i < 2; i++){
        customPlot->addGraph(customPlot->xAxis, customPlot->yAxis);
        //customPlot->graph(i)->setName(str.append(ch+i));
        customPlot->graph(i)->setPen(customPlot->graph(i)->pen());
        //customPlot->addPlottable(customPlot->graph(i));
    }

    connect(customPlot, SIGNAL(legendClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(toggleVisibility(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)));
    */

    // update changes
    customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawerMouseMoved(QMouseEvent *event){
    QPoint pos = event->pos();
    double key = customPlot->xAxis->pixelToCoord(pos.x());
    phaseTracer->setGraphKey(key);

    double x, y;
    x = qBound(0., 1. + phaseTracer->position->key() / customPlot->xAxis->range().size(), 0.5);
    y = qBound(0., 1. - phaseTracer->position->value() / customPlot->yAxis->range().size(), 0.9);// value  coord is TopLeft origin
    phaseTracerText->position->setCoords(x, y);
    phaseTracerText->setText("(" + QString::number(phaseTracer->position->key()) + ", " + QString::number(phaseTracer->position->value()) + ")");

    ui->drawer->replot();
}

void MainWindow::drawerMousePressed(QMouseEvent *event) {
    // add phase trace
    for(int i = 0; i < graphCount; i++) {
        if(customPlot->graph(i)->selected()) {
            // add phase tracer
            phaseTracer = new QCPItemTracer(customPlot);
            phaseTracer->setGraph(customPlot->graph(i));
            phaseTracer->setGraphKey(0.0);
            phaseTracer->setStyle(QCPItemTracer::tsCrosshair);
            phaseTracer->setPen(QPen(Qt::red));
            phaseTracer->setBrush(Qt::red);
            phaseTracer->setSize(7);

            // add label for phase tracer
            phaseTracerText = new QCPItemText(customPlot);
            phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
            phaseTracerText->setFont(QFont(font().family(), 12));
            phaseTracerText->setPositionAlignment(Qt::AlignLeft);
            phaseTracerText->position->setCoords(QPointF(0, 0));
            phaseTracerText->setText("(-, -)");
            break;
        }
    }
}

/*
void MainWindow::toggleVisibility(){
    if(!item) return;
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    if(!plItem) return;

    plItem->plottable()->setVisible(!plItem->plottable()->visible());
}
*/

void MainWindow::on_createFunction_clicked()
{
    qDebug() << "Create Function was clicked!!";

    QCheckBox* showMe = new QCheckBox();
    showMe->setChecked(true);
    QLabel* label = new QLabel("");  // O X
    QLineEdit* inputFunction = new QLineEdit("");
    QPushButton* bye = new QPushButton("delete");
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(showMe);
    hlayout->addWidget(label);
    hlayout->addWidget(inputFunction);
    hlayout->addWidget(bye);
    ui->verticalLayout->addLayout(hlayout);

    customPlot->addGraph();
    graphCount++;

    checkBox.push_back(showMe);
    lab.push_back(label);
    lineEdit.push_back(inputFunction);
    pushBotton.push_back(bye);
    hl.push_back(hlayout);

    /*
    QVector<double> x(DBL_MAX), y(DBL_MAX);
    for(int i = 0; i < DBL_MAX; i++) {
        x[i] = i/50.0 - DBL_MAX;
        y[i] = x[i] * x[i]; // input: connect to the back_end ^_^
    }
    customPlot->graph(graphCount)->setData(x, y);
    QColor color = QColorDialog::getColor(
        "#800a64c8",
        this,
        tr("choose color"),
        QColorDialog::ShowAlphaChannel
    );
    if (color.isValid()) {
        color.name();
        color.name(QColor::HexArgb);
        int r, g, b, a;
        color.getRgb(&r, &g, &b, &a);
    }
    QPalette palette = label->palette();
    palette.setColor(label->backgroundRole(), color);
    palette.setColor(label->foregroundRole(), Qt::black);
    label->setAutoFillBackground(true);
    label->setPalette(palette);
    label->setText(" ");
    customPlot->graph(graphCount)->setPen(color);
    */

    connect(showMe, SIGNAL(clicked()), this, SLOT(on_showMe_stateChanged()));
    connect(bye, SIGNAL(pressed()), this, SLOT(on_bye_clicked()));
    connect(inputFunction, SIGNAL(returnPressed()), this, SLOT(on_inputFunction_editingFinished()));
    //connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(drawerMousePressd(QMouseEvent*)));
    //connect(customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(drawerMouseMoved(QMouseEvent*)));

    customPlot->replot();
}


void MainWindow::on_showMe_stateChanged()
{
    qDebug() << "showMe was clicked!!";
    for(unsigned long i = 0; i < checkBox.size(); i++) {
        customPlot->graph(i)->setVisible(checkBox[i]->isChecked());
    }
    customPlot->replot();
}

void MainWindow::on_inputFunction_editingFinished()
{
    qDebug() << "inputFunction editing finished!!";
    for(unsigned long i = 0; i < lineEdit.size(); i++) {
        if(lineEdit[i] == sender()) {

            EquationParser equation;

            equation.id = i;
            try {
                equation.setString(lineEdit[i]->text().toStdString());
            }
            catch (logic_error& e) {

                // QMessageBox
                QMessageBox::warning(this, tr("warning"), tr(equation.warning.c_str()));
                /*
                QMessageBox msgBox(this);
                msgBox.setText(tr(equation.warning.c_str()));
                msgBox.setWindowTitle(tr("warning"));
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.
                */

                lab[i]->setText("X");
                QPalette palette = lab[i]->palette();
                palette.setColor(lab[i]->backgroundRole(), Qt::white);
                palette.setColor(lab[i]->foregroundRole(), Qt::black);
                //qDebug() << e.what() << "\n";
                customPlot->replot();
            }
            if (!equation.exception) {
                customPlot->graph(i)->setData(equation.xVec, equation.yVec);
                //plot->addGraph();
                //customPlot->graph(equation.id)->setPen(COLOR[qrand() % COLOR.size()]);
                QColor color = QColorDialog::getColor(
                    "#800a64c8",
                    this,
                    tr("choose color"),
                    QColorDialog::ShowAlphaChannel
                );
                if (color.isValid()) {
                    color.name();
                    color.name(QColor::HexArgb);
                    int r, g, b, a;
                    color.getRgb(&r, &g, &b, &a);
                }
                QPalette palette = lab[i]->palette();
                palette.setColor(lab[i]->backgroundRole(), color);
                palette.setColor(lab[i]->foregroundRole(), Qt::black);
                lab[i]->setAutoFillBackground(true);
                lab[i]->setPalette(palette);
                lab[i]->setText("O");
                customPlot->graph(i)->setPen(color);
                //EquationList.push_back(equation);
                //curveAmount++;
                customPlot->replot();
            }


            break;
        }
    }

}


void MainWindow::on_bye_clicked()
{
    qDebug() << "Bye was clicked!!";
    for(unsigned long i = 0; i < pushBotton.size(); i++) {
        if(pushBotton[i] == sender()) {

            delete checkBox[i];
            delete lab[i];
            delete lineEdit[i];
            delete pushBotton[i];
            delete hl[i];
            customPlot->graph(i)->data()->clear();
            customPlot->removeGraph(i);
            graphCount--;

            checkBox.erase(checkBox.begin()+i);
            lab.erase(lab.begin()+i);
            lineEdit.erase(lineEdit.begin()+i);
            pushBotton.erase(pushBotton.begin()+i);
            hl.erase(hl.begin()+i);

            break;
        }
    }
    customPlot->replot();
}


void MainWindow::on_btn_recovery_clicked()
{
    customPlot->xAxis->setRange(-10, 10);
    customPlot->yAxis->setRange(-10, 10);
    customPlot->replot();
}

