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
  qDebug() << "Online_Update...";
}
