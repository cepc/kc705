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
  m_nx(48),
  m_ny(16),
  m_state("STOPPED")
{
  ui->setupUi(this);

  m_GUIManager = new GUIManager();
  m_thread_man = new QThread(this);
  m_GUIManager->moveToThread(m_thread_man);
  
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
  m_timer = new QTimer(0);
  m_timer->setInterval(1000);
  m_timer->moveToThread(m_thread);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(Update_Online_Image()), Qt::DirectConnection);
  connect(m_GUIManager, SIGNAL(IsRunning()), m_thread, SLOT(start()));
  connect(m_thread, SIGNAL(started()), m_timer, SLOT(start()));
  connect(m_thread_man, SIGNAL(finished()), m_thread, SLOT(quit()));
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
  if(m_thread_man){
    m_thread_man->quit();
    m_thread_man->wait();
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
  std::cout<<"=========GUI Start RUN: " << "======="<< std::endl; 
  
  ui->Btn_Online_StartRun->setEnabled(false);
  ui->Btn_Online_StopRun->setEnabled(true);
  m_state = "RUNNING"; 

  m_thread_man->start();
  connect(m_thread_man, SIGNAL(started()),m_GUIManager, SLOT(start_run()));
    
  qDebug() << "Btn_Online_StartRun_Clicked... ";
}

void MainWindow::Btn_Online_StopRun_Clicked()
{
  ui->Btn_Online_StopRun->setEnabled(false);
  m_GUIManager->stop_run();
  std::cout<<"=========GUI Stop RUN: " << "======="<< std::endl; 
  
  m_thread_man->quit(); 
  m_thread_man->wait(); 

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

  m_pedestalAxisRect = new QCPAxisRect(ui->customPlot);
  m_pedestalAxisRect->setupFullAxesBox(true);
  
  m_noiseAxisRect = new QCPAxisRect(ui->customPlot);
  m_noiseAxisRect->setupFullAxesBox(true);

  m_LayoutTop->addElement(0,0,m_adcAxisRect);
  m_LayoutTop->addElement(0,1,m_adcScale);   

  m_LayoutBottom->addElement(0,0,m_pedestalAxisRect);
  m_LayoutBottom->addElement(0,1,m_noiseAxisRect);

  m_marginGroup = new QCPMarginGroup(ui->customPlot);
  m_adcAxisRect->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup); 
  m_adcScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup); 
  
  m_pedestalAxisRect->setMarginGroup(QCP::msLeft, m_marginGroup); 
  m_noiseAxisRect->setMarginGroup(QCP::msRight, m_marginGroup); 

  Draw_Online_Image();
}

void MainWindow::Draw_Online_Image()
{
  m_adcMap = new QCPColorMap(m_adcAxisRect->axis(QCPAxis::atBottom), m_adcAxisRect->axis(QCPAxis::atLeft));
  m_adcMap->setColorScale(m_adcScale); 

  for (unsigned int i = 0; i < m_nx; ++i)
    for (unsigned int j = 0; j < m_ny; ++j)
    {
      m_adcMap->data()->setCell(i,j,i+j);
    }  

  m_adcMap->setColorScale(m_adcScale); 
  m_adcMap->setGradient(QCPColorGradient::gpPolar);
  m_adcMap->rescaleDataRange();

 
  m_pedestalGraph = new QCPGraph(m_pedestalAxisRect->axis(QCPAxis::atBottom), m_pedestalAxisRect->axis(QCPAxis::atLeft)); 

  m_noiseGraph = new QCPGraph(m_noiseAxisRect->axis(QCPAxis::atBottom), m_noiseAxisRect->axis(QCPAxis::atLeft)); 

  ui->customPlot->rescaleAxes();
  ui->customPlot->replot();
  qRegisterMetaType<QCPRange>("QCPRange");
}

void MainWindow::Update_Online_Image()
{
  if(m_state == "RUNNING") {
    
    m_GUIManager->get_monitor()->ProcessData();
    
    m_adcMap->setData(m_GUIManager->get_monitor()->GetADCMap());
    m_adcMap->rescaleDataRange();
    m_adcMap->setColorScale(m_adcScale); 
  
    m_pedestalGraph->addData(m_GUIManager->get_monitor()->GetPedestal(1,1)->data());

    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
  }
  qRegisterMetaType<QCPRange>("QCPRange");
}
