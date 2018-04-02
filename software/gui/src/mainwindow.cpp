#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qt>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QDebug>



#include <QTime>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_nx(48),
  m_ny(16),
  m_col(0),
  m_row(0),
  m_state("STOPPED")
{
  ui->setupUi(this);
  this->setWindowIcon(QIcon("../software/gui/form/iheplogo.gif"));

  m_GUIManager = new GUIManager();
  
  connect(ui->Action_Open, SIGNAL(triggered()), this, SLOT(Action_Open_Triggered()));
  connect(ui->Action_Save, SIGNAL(triggered()), this, SLOT(Action_Save_Triggered()));
  connect(ui->Action_Exit, SIGNAL(triggered()), this, SLOT(Action_Exit_Triggered()));

  ui->LineEdit_Online_FilePath->setText(QDir::currentPath());

  connect(ui->Btn_Online_Choose, SIGNAL(clicked()), this, SLOT(Btn_Online_Choose_Clicked()));
  connect(ui->Btn_Online_Config, SIGNAL(clicked()), this, SLOT(Btn_Online_Config_Clicked()));
  connect(ui->Btn_Online_StartRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StartRun_Clicked()));
  connect(ui->Btn_Online_StopRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StopRun_Clicked()));

  m_col = ui->SpinBox_Online_Col->value();
  m_row = ui->SpinBox_Online_Row->value();

  Init_Online_Image();

  m_timer = new QTimer(0);
  m_timer->setInterval(int(1e6/ui->SpinBox_Online_evDisplay->value()));
  connect(m_timer, SIGNAL(timeout()), this, SLOT(Update_Online_Image()));
  connect(m_GUIManager, SIGNAL(IsRunning()), m_timer, SLOT(start()));
  connect(m_GUIManager, SIGNAL(IsStop()), m_timer, SLOT(stop()));

  m_timer_run = new QTimer(0);
  m_timer_run->setInterval(int(ui->SpinBox_Online_TimeRun->value()));
  connect(m_timer_run, SIGNAL(timeout()), this, SLOT(Btn_Online_StopRun_Clicked()));
  connect(m_GUIManager, SIGNAL(IsRunning()), m_timer_run, SLOT(start()));
  connect(m_GUIManager, SIGNAL(IsStop()), m_timer_run, SLOT(stop()));
}

MainWindow::~MainWindow()
{
  delete ui;
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

  m_timer_run->setInterval(int(ui->SpinBox_Online_TimeRun->value()));
  m_GUIManager->set_run_time(std::to_string(ui->SpinBox_Online_TimeRun->value()));
  
  m_timer->setInterval(int(1e6/ui->SpinBox_Online_evDisplay->value()));
  m_GUIManager->set_ev_print(std::to_string(ui->SpinBox_Online_SampleEvents->value()));
  m_GUIManager->set_chip_address(ui->SpinBox_Online_ChipAddress->value());
  m_GUIManager->set_nfiles(ui->SpinBox_Online_NFiles->value());
  m_GUIManager->set_channel(ui->SpinBox_Online_Col->value(), ui->SpinBox_Online_Row->value()); 
  m_GUIManager->set_adc_threshold(ui->SpinBox_Online_ADCThreshold->value());

  m_GUIManager->config();
}

void MainWindow::Btn_Online_StartRun_Clicked()
{  
  std::cout<<"=========GUI Start RUN: " << "======="<< std::endl; 
 
  Clear_Online_Image(); 
  ui->Btn_Online_StartRun->setEnabled(false);
  ui->Btn_Online_StopRun->setEnabled(true);
  m_state = "RUNNING"; 

  m_GUIManager->start_run();
    
  qDebug() << "Btn_Online_StartRun_Clicked... ";
}

void MainWindow::Btn_Online_StopRun_Clicked()
{
  ui->Btn_Online_StopRun->setEnabled(false);
  m_GUIManager->stop_run();
  std::cout<<"=========GUI Stop RUN: " << "======="<< std::endl; 
  
  ui->Btn_Online_StartRun->setEnabled(true);
  qDebug() << "Btn_Online_StopRun_Clicked... ";
}

void MainWindow::Init_Online_Image()
{
  ui->customPlot->plotLayout()->clear();
  ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes);
  
  m_LayoutTop = new QCPLayoutGrid;
  ui->customPlot->plotLayout()->addElement(0, 0, m_LayoutTop);

  m_LayoutBottom = new QCPLayoutGrid;
  ui->customPlot->plotLayout()->addElement(1, 0, m_LayoutBottom);

  m_adcAxisRect = new QCPAxisRect(ui->customPlot);
  m_adcAxisRect->setupFullAxesBox(true);
  m_adcAxisRect->axis(QCPAxis::atBottom)->setLabel("Col");
  m_adcAxisRect->axis(QCPAxis::atLeft)->setLabel("Row");

  QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
  fixedTicker->setTickStep(4.0); 
  fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);   
  m_adcAxisRect->axis(QCPAxis::atBottom)->setTicker(fixedTicker);
  m_adcAxisRect->axis(QCPAxis::atLeft)->setTicker(fixedTicker);

  m_adcScale = new QCPColorScale(ui->customPlot);
  m_adcScale->setType(QCPAxis::atRight); 
  m_adcScale->axis()->setLabel("ADC");

  m_histADCAxisRect = new QCPAxisRect(ui->customPlot);
  m_histADCAxisRect->setupFullAxesBox(true);
  m_histADCAxisRect->axis(QCPAxis::atBottom)->setLabel("ADC");
  m_histADCAxisRect->axis(QCPAxis::atLeft)->setLabel("Count");


  //m_pedestalAxisRect = new QCPAxisRect(ui->customPlot);
  //m_pedestalAxisRect->setupFullAxesBox(true);
  //m_pedestalAxisRect->axis(QCPAxis::atBottom)->setLabel("Pedestal");
  
  //m_noiseAxisRect = new QCPAxisRect(ui->customPlot);
  //m_noiseAxisRect->setupFullAxesBox(true);
  //m_noiseAxisRect->axis(QCPAxis::atBottom)->setLabel("Noise");

  m_LayoutTop->addElement(0,0,m_adcAxisRect);
  m_LayoutTop->addElement(0,1,m_adcScale);   

  m_LayoutBottom->addElement(0,0,m_histADCAxisRect);
  //m_LayoutBottom->addElement(0,0,m_pedestalAxisRect);
  //m_LayoutBottom->addElement(0,1,m_noiseAxisRect);

  m_marginGroup = new QCPMarginGroup(ui->customPlot);
  m_adcAxisRect->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup); 
  m_adcScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup); 
  
  m_histADCAxisRect->setMarginGroup(QCP::msLeft, m_marginGroup); 
  //m_pedestalAxisRect->setMarginGroup(QCP::msLeft, m_marginGroup); 
  //m_noiseAxisRect->setMarginGroup(QCP::msRight, m_marginGroup); 

  Draw_Online_Image();
}

void MainWindow::Draw_Online_Image()
{
  m_adcMap = new QCPColorMap(m_adcAxisRect->axis(QCPAxis::atBottom), m_adcAxisRect->axis(QCPAxis::atLeft));
  m_adcMap->setColorScale(m_adcScale); 

  m_adcMap->setColorScale(m_adcScale); 
  m_adcMap->setGradient(QCPColorGradient::gpPolar);
  m_adcMap->rescaleDataRange();

 
  m_histADCGraph = new QCPGraph(m_histADCAxisRect->axis(QCPAxis::atBottom), m_histADCAxisRect->axis(QCPAxis::atLeft)); 
  //m_histADCGraph->setLineStyle(QCPGraph::lsStepCenter);
  m_histADCGraph->setLineStyle(QCPGraph::lsImpulse);
  m_histADCGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

  //m_pedestalGraph = new QCPGraph(m_pedestalAxisRect->axis(QCPAxis::atBottom), m_pedestalAxisRect->axis(QCPAxis::atLeft)); 
  //m_noiseGraph = new QCPGraph(m_noiseAxisRect->axis(QCPAxis::atBottom), m_noiseAxisRect->axis(QCPAxis::atLeft)); 

  ui->customPlot->rescaleAxes();
  ui->customPlot->replot();
  qRegisterMetaType<QCPRange>("QCPRange");
}

void MainWindow::Update_Online_Image()
{
  if(m_state == "RUNNING") {
    
    //m_adcMap->setData(m_GUIManager->get_monitor()->GetADCMap());
    m_adcMap->setData(m_GUIManager->get_monitor()->GetCountMap());//count model
    qDebug() << "ADC Count Model... ";

    m_adcMap->rescaleDataRange();
    m_adcMap->setColorScale(m_adcScale); 

    m_histADCGraph->data()->clear();
    m_histADCGraph->data()->add(m_GUIManager->get_monitor()->GetHistADC(m_col,m_row));

    //m_pedestalGraph->data()->clear();
    //m_pedestalGraph->data()->add(m_GUIManager->get_monitor()->GetPedestal(m_col,m_row));

    //m_noiseGraph->data()->clear();
    //m_noiseGraph->data()->add(m_GUIManager->get_monitor()->GetNoise(m_col,m_row));

    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
  }
  qRegisterMetaType<QCPRange>("QCPRange");
}

void MainWindow::Clear_Online_Image()
{
  //m_pedestalGraph->data()->clear();
  //m_noiseGraph->data()->clear();
  m_histADCGraph->data()->clear();
  m_adcMap->data()->clear();

  connect(m_GUIManager, SIGNAL(IsRunning()), m_timer, SLOT(start()));
  connect(m_GUIManager, SIGNAL(IsStop()), m_timer, SLOT(stop()));
}
