#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nx(48)
    , m_ny(16)
    , m_col(0)
    , m_row(0)
    , m_state("STOPPED")
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

  connect(ui->Btn_Draw_Histogram, SIGNAL(clicked()), this, SLOT(Init_Online_Image()));

  m_timer = new QTimer(this);
  m_timer->setInterval(int(1e6 / ui->SpinBox_Online_evDisplay->value()));
  connect(m_timer, SIGNAL(timeout()), this, SLOT(Update_Online_Image()));
  connect(m_GUIManager, SIGNAL(IsRunning()), m_timer, SLOT(start()));
  connect(m_GUIManager, SIGNAL(IsStop()), m_timer, SLOT(stop()));

  m_timer_run = new QTimer(this);
  m_timer_run->setInterval(int(ui->SpinBox_Online_TimeRun->value()));
  connect(m_timer_run, SIGNAL(timeout()), this, SLOT(Btn_Online_StopRun_Clicked()));
  connect(m_GUIManager, SIGNAL(IsRunning()), m_timer_run, SLOT(start()));
  connect(m_GUIManager, SIGNAL(IsStop()), m_timer_run, SLOT(stop()));

  m_root_timer = new QTimer(this);
  m_root_timer->setInterval(int(1e6 / ui->SpinBox_Online_evDisplay->value()));
  connect(m_root_timer, SIGNAL(timeout()), this, SLOT(Handle_Root_Events()));
  connect(m_GUIManager, SIGNAL(IsRunning()), m_root_timer, SLOT(start()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::Action_Open_Triggered()
{
  QString filter = "File Description (*.dat *.df *.bin)";
  auto files = QFileDialog::getOpenFileNames(this, "Open data Files", QDir::currentPath(), filter);
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
  QString qinfile = ui->LineEdit_Online_cfName->text();
  m_GUIManager->set_config_path(qinfile.toStdString());

  m_GUIManager->config();
}

void MainWindow::Btn_Online_StartRun_Clicked()
{
  std::cout << "=========GUI Start RUN: "
            << "=======" << std::endl;

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
  std::cout << "=========GUI Stop RUN: "
            << "=======" << std::endl;

  ui->Btn_Online_StartRun->setEnabled(true);
  qDebug() << "Btn_Online_StopRun_Clicked... ";
}

void MainWindow::Init_Online_Image()
{
  // Handle the "Draw Histogram" button clicked() event.

  static TH1F* h1f = 0;
  static TH2F* h2f = 0;

  ui->Canvas_Plot->getCanvas()->Clear();
  ui->Canvas_Plot->getCanvas()->cd();
  ui->Canvas_Plot->getCanvas()->SetBorderMode(0);
  ui->Canvas_Plot->getCanvas()->SetFillColor(0);
  ui->Canvas_Plot->getCanvas()->SetGrid();
  ui->Canvas_Plot->getCanvas()->Divide(1, 2);
  ui->Canvas_Plot->getCanvas()->cd(1);

  h1f = new TH1F("h1f", "ADC Hist", 4000, -2000, 2000);
  h1f->SetFillColor(kViolet + 2);
  h1f->SetFillStyle(3001);
  h1f->Draw();

  ui->Canvas_Plot->getCanvas()->cd(2);
  h2f = new TH2F("h2f", "ADC MAP", m_nx, 0, m_nx, m_ny, 0, m_ny);
  h2f->SetFillColor(kViolet + 2);
  h2f->SetFillStyle(3001);
  h2f->Draw();

  ui->Canvas_Plot->getCanvas()->Modified();
  ui->Canvas_Plot->getCanvas()->Update();
}

void MainWindow::Update_Online_Image()
{
  if (m_state == "RUNNING") {

    ui->Canvas_Plot->getCanvas()->cd(1);
    auto h1f = (TH1F*)m_GUIManager->get_monitor()->GetADCHist()->Clone("ADC");
    h1f->SetFillColor(kViolet + 2);
    h1f->SetFillStyle(3001);
    h1f->Draw();

    ui->Canvas_Plot->getCanvas()->cd(2);
    auto hist2D = (TH2F*)m_GUIManager->get_monitor()->GetADCMap()->Clone("ADC MAP");
    hist2D->Draw("COLZ");
    ui->Canvas_Plot->getCanvas()->Modified();
    ui->Canvas_Plot->getCanvas()->Update();
  }
}

void MainWindow::Clear_Online_Image()
{
  connect(m_GUIManager, SIGNAL(IsRunning()), m_timer, SLOT(start()));
  connect(m_GUIManager, SIGNAL(IsStop()), m_timer, SLOT(stop()));
}

void MainWindow::Handle_Root_Events()
{
  gSystem->ProcessEvents();
}
