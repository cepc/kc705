#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"
#include <Qt>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_monitor(new Monitor())
{
  ui->setupUi(this);
  connect(ui->Action_Open, SIGNAL(triggered()), this, SLOT(Action_Open_Triggered()));
  connect(ui->Action_Save, SIGNAL(triggered()), this, SLOT(Action_Save_Triggered()));
  connect(ui->Action_Exit, SIGNAL(triggered()), this, SLOT(Action_Exit_Triggered()));

  ui->LineEdit_Online_FilePath->setText(QDir::currentPath());

  connect(ui->Btn_Online_Config, SIGNAL(clicked()), this, SLOT(Btn_Online_Config_Clicked()));
  connect(ui->Btn_Online_StartRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StartRun_Clicked()));
  connect(ui->Btn_Online_StopRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StopRun_Clicked()));

}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_monitor;
}

void MainWindow::Action_Open_Triggered()
{
  QString filter = "File Description (*.dat *.df *.bin)";
  auto files = QFileDialog::getOpenFileNames(this, "Open data Files", QDir::currentPath(),filter);
  qDebug() << "Action_Open_Triggered... ";
}

void MainWindow::Action_Save_Triggered()
{
  QFileDialog dialog(this);
  dialog.setWindowModality(Qt::WindowModal);
  qDebug() << "Action_Save_Triggered... ";
}

void MainWindow::Action_Exit_Triggered()
{
  this->close();
  qDebug() << "Action_Exit_Triggered... ";
}

void MainWindow::Btn_Online_Config_Clicked()
{
  m_monitor->set_input_data_path("/tmp/test_pipe");
  m_monitor->set_output_data_path("/home/chenlj/Documents/Code/kc705/data/sim/test");
  m_monitor->set_register_data_path("/tmp/test_reg");
  m_monitor->set_run_time(std::to_string(ui->SpinBox_Online_TimeRun->value()));
  m_monitor->set_ev_print(std::to_string(ui->SpinBox_Online_evPrint->value()));
}

void MainWindow::Btn_Online_StartRun_Clicked()
{  
  m_monitor->start_run();
  Online_Update();
  ui->Btn_Online_StartRun->setAttribute(Qt::WA_UnderMouse, false);
  qDebug() << "Btn_Online_StartRun_Clicked... ";
}

void MainWindow::Btn_Online_StopRun_Clicked()
{
  m_monitor->stop_run();
  Online_Update();
  ui->Btn_Online_StopRun->setAttribute(Qt::WA_UnderMouse, false);
  qDebug() << "Btn_Online_StopRun_Clicked... ";
}

void MainWindow::Online_Update()
{
  Draw_Online_Image();
  qDebug() << "Online_Update...";
}

void MainWindow::Draw_Online_Image()
{
  // configure axis rect:
  ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
  ui->customPlot->axisRect()->setupFullAxesBox(true);
  ui->customPlot->xAxis->setLabel("x");
  ui->customPlot->yAxis->setLabel("y");

  // set up the QCPColorMap:
  QCPColorMap *colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
  int nx = 200;
  int ny = 200;
  colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
  colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
  // now we assign some data, by accessing the QCPColorMapData instance of the color map:
  double x, y, z;
  for (int xIndex=0; xIndex<nx; ++xIndex)
  {
    for (int yIndex=0; yIndex<ny; ++yIndex)
    {
      colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
      double r = 3*qSqrt(x*x+y*y)+1e-2;
      z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
      colorMap->data()->setCell(xIndex, yIndex, z);
    }
  }

  // add a color scale:
  QCPColorScale *colorScale = new QCPColorScale(ui->customPlot);
  ui->customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
  colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
  colorMap->setColorScale(colorScale); // associate the color map with the color scale
  colorScale->axis()->setLabel("Magnetic Field Strength");

  // set the color gradient of the color map to one of the presets:
  colorMap->setGradient(QCPColorGradient::gpPolar);
  // we could have also created a QCPColorGradient instance and added own colors to
  // the gradient, see the documentation of QCPColorGradient for what's possible.

  // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
  colorMap->rescaleDataRange();

  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->customPlot);
  ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

  // rescale the key (x) and value (y) axes so the whole color map is visible:
  ui->customPlot->rescaleAxes();
}
