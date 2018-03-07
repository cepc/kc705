#include "mainwindow.h"
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
  m_monitor(new Monitor()),
  m_nx(48),
  m_ny(16)
{
  ui->setupUi(this);
  connect(ui->Action_Open, SIGNAL(triggered()), this, SLOT(Action_Open_Triggered()));
  connect(ui->Action_Save, SIGNAL(triggered()), this, SLOT(Action_Save_Triggered()));
  connect(ui->Action_Exit, SIGNAL(triggered()), this, SLOT(Action_Exit_Triggered()));

  ui->LineEdit_Online_FilePath->setText(QDir::currentPath());

  connect(ui->Btn_Online_Config, SIGNAL(clicked()), this, SLOT(Btn_Online_Config_Clicked()));
  connect(ui->Btn_Online_StartRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StartRun_Clicked()));
  connect(ui->Btn_Online_StopRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StopRun_Clicked()));

  Init_Online_Image();
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
  m_monitor->set_chip_address(ui->SpinBox_Online_ChipAddress->value());
  m_monitor->config();
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

void MainWindow::Init_Online_Image()
{
  // configure axis rect:
  ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes); // this will also allow rescaling the color scale by dragging/zooming
  ui->customPlot->axisRect()->setupFullAxesBox(true);
  ui->customPlot->xAxis->setLabel("Col");
  ui->customPlot->yAxis->setLabel("Row");
 
  QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
  fixedTicker->setTickStep(4.0); 
  fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);   
  
  ui->customPlot->xAxis->setTicker(fixedTicker);
  ui->customPlot->yAxis->setTicker(fixedTicker);
  ui->customPlot->rescaleAxes();

  Draw_Online_Image();
  
  connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(Selection_Changed()));
  connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(Mouse_Press()));
  connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(Mouse_Wheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::Selection_Changed()
{
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::Mouse_Press()
{
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::Mouse_Wheel()
{
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::Draw_Online_Image()
{
// set up the QCPColorMap:
  QCPColorMap *colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
  std::vector<int16_t> m_data = Generate_Fake_Data();
  colorMap->setData(Matrix_To_Data(m_data));
  
  // add a color scale:
  QCPColorScale *colorScale = new QCPColorScale(ui->customPlot);
  ui->customPlot->plotLayout()->addElement(0, 1, colorScale);   
  colorScale->setType(QCPAxis::atRight); 
  colorMap->setColorScale(colorScale); 
  colorScale->axis()->setLabel("ADC");

  // set the color gradient of the color map to one of the presets:
  colorMap->setGradient(QCPColorGradient::gpPolar);
  //colorMap->setInterpolate(false);
  colorMap->rescaleDataRange();

  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->customPlot);
  ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  
  ui->customPlot->rescaleAxes();
}

QCPColorMapData* MainWindow::Matrix_To_Data(const std::vector<int16_t> &matrix)
{
  QCPColorMapData* data = new QCPColorMapData(m_nx, m_ny, QCPRange(0,m_nx), QCPRange(0,m_ny));
  for (unsigned int i = 0; i < m_nx; ++i)
    for (unsigned int j = 0; j < m_ny; ++j)
    {
      size_t pos = i + m_nx*j;
      data->setCell(i, j, matrix.at(pos));
    }
  return data;
}

std::vector<int16_t> MainWindow::Generate_Fake_Data()
{
  std::vector<int16_t> m_data;
  // now we assign some data, by accessing the QCPColorMapData instance of the color map:
  int16_t ADC;
  for (int xIndex=0; xIndex<m_nx; ++xIndex)
  {
    for (int yIndex=0; yIndex<m_ny; ++yIndex)
    {
      ADC = xIndex*100; 
      m_data.push_back(ADC);
    }
  }
  return m_data;
}
