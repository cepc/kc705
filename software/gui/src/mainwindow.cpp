#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qt>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_GUIManager(new GUIManager()),
  m_nx(48),
  m_ny(16),
  m_state("STOPPED")
{
  ui->setupUi(this);

  connect(ui->Action_Open, SIGNAL(triggered()), this, SLOT(Action_Open_Triggered()));
  connect(ui->Action_Save, SIGNAL(triggered()), this, SLOT(Action_Save_Triggered()));
  connect(ui->Action_Exit, SIGNAL(triggered()), this, SLOT(Action_Exit_Triggered()));

  ui->LineEdit_Online_FilePath->setText(QDir::currentPath());

  connect(ui->Btn_Online_Choose, SIGNAL(clicked()), this, SLOT(Btn_Online_Choose_Clicked()));
  connect(ui->Btn_Online_Config, SIGNAL(clicked()), this, SLOT(Btn_Online_Config_Clicked()));
  connect(ui->Btn_Online_StartRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StartRun_Clicked()));
  connect(ui->Btn_Online_StopRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StopRun_Clicked()));

  Init_Online_Image();

  m_thread = new QThread(this);
  m_thread->start();
  m_timer = new QTimer(0);
  m_timer->setInterval(1000);
  m_timer->moveToThread(m_thread);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(Update_Online_Image()), Qt::DirectConnection);
  connect(m_thread, SIGNAL(started()), m_timer, SLOT(start()));

}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_GUIManager;
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
  if(m_thread){
    m_thread->quit();
    m_thread->wait();
  }
  this->close();
  qDebug() << "Action_Exit_Triggered... ";
}

void MainWindow::Btn_Online_Choose_Clicked()
{
  auto file_path = QFileDialog::getExistingDirectory(this, "Chose the directory to save data", QDir::currentPath());
  ui->LineEdit_Online_FilePath->setText(file_path);
  qDebug() << "Btn_Online_Choose_Clicked... ";
}

void MainWindow::Btn_Online_Config_Clicked()
{
  QString qinfile = ui->LineEdit_Online_InName->text();
  m_GUIManager->set_input_data_path(qinfile.toStdString());
  
  QString qoutfile = ui->LineEdit_Online_FilePath->text()+ "/" +ui->LineEdit_Online_FileName->text();
  m_GUIManager->set_output_data_path(qoutfile.toStdString());
  
  QString qregfile = ui->LineEdit_Online_RegName->text();
  m_GUIManager->set_register_data_path(qregfile.toStdString());

  m_GUIManager->set_run_time(std::to_string(ui->SpinBox_Online_TimeRun->value()));
  m_GUIManager->set_ev_print(std::to_string(ui->SpinBox_Online_evPrint->value()));
  m_GUIManager->set_chip_address(ui->SpinBox_Online_ChipAddress->value());
  m_GUIManager->set_nfiles(ui->SpinBox_Online_NFiles->value());
}

void MainWindow::Btn_Online_StartRun_Clicked()
{  
  ui->Btn_Online_StartRun->setAttribute(Qt::WA_UnderMouse, false);
  m_state="RUNNING";
  Online_Update();
  for(int i=0; i<m_GUIManager->get_nfiles();i++){
    std::cout << "Start Run: " << i << std::endl;
    m_GUIManager->config();
    m_GUIManager->start_run();
    Delay(1000);
  }
  qDebug() << "Btn_Online_StartRun_Clicked... ";
}

void MainWindow::Btn_Online_StopRun_Clicked()
{
  ui->Btn_Online_StopRun->setAttribute(Qt::WA_UnderMouse, false);
  m_state="STOPPED";
  Online_Update();
  m_GUIManager->stop_run();
  if(m_thread){
    m_thread->quit();
    m_thread->wait();
  }
  qDebug() << "Btn_Online_StopRun_Clicked... ";
}

void MainWindow::Online_Update()
{
  ui->Btn_Online_StartRun->setAttribute(Qt::WA_UnderMouse, (m_state=="STOPPED"));
  ui->Btn_Online_StartRun->setEnabled((m_state=="STOPPED"));

  ui->Btn_Online_StopRun->setAttribute(Qt::WA_UnderMouse, (m_state=="RUNNING"));
  ui->Btn_Online_StopRun->setEnabled((m_state=="RUNNING"));

  qDebug() << "Online_Update...";
}

void MainWindow::Init_Online_Image()
{
  // configure axis rect:
  ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes); // this will also allow rescaling the color scale by dragging/zooming
  ui->customPlot->axisRect()->setupFullAxesBox(true);
  ui->customPlot->xAxis->setLabel("Col");
  ui->customPlot->yAxis->setLabel("Row");

  Draw_Online_Image();

  QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
  fixedTicker->setTickStep(4.0); 
  fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);   

  ui->customPlot->xAxis->setTicker(fixedTicker);
  ui->customPlot->yAxis->setTicker(fixedTicker);
  ui->customPlot->rescaleAxes();

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
  colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
  for (unsigned int i = 0; i < m_nx; ++i)
    for (unsigned int j = 0; j < m_ny; ++j)
    {
      colorMap->data()->setCell(i,j,0);
    }  

  // add a color scale:
  colorScale = new QCPColorScale(ui->customPlot);
  ui->customPlot->plotLayout()->addElement(0, 1, colorScale);   
  colorScale->setType(QCPAxis::atRight); 
  colorMap->setColorScale(colorScale); 
  colorScale->axis()->setLabel("ADC");

  // set the color gradient of the color map to one of the presets:
  colorMap->setGradient(QCPColorGradient::gpPolar);
  colorMap->rescaleDataRange();

  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->customPlot);
  ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

  ui->customPlot->rescaleAxes();
}

void MainWindow::Update_Online_Image()
{
  if(m_state == "RUNNING"){
    colorMap->setData(m_GUIManager->get_monitor()->GetData());
    colorMap->rescaleDataRange();
    colorMap->setColorScale(colorScale); 
  }
  ui->customPlot->rescaleAxes();
  ui->customPlot->replot();
  qRegisterMetaType<QCPRange>("QCPRange");
}

void MainWindow::Delay(int millisecondsToWait)
{
  QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
  while( QTime::currentTime() < dieTime )
  {
    QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
  }
}
