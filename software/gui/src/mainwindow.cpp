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
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->Action_Open, SIGNAL(triggered()), this, SLOT(Action_Open_Triggered()));
  connect(ui->Action_Save, SIGNAL(triggered()), this, SLOT(Action_Save_Triggered()));
  connect(ui->Action_Exit, SIGNAL(triggered()), this, SLOT(Action_Exit_Triggered()));

  ui->LineEdit_Online_FilePath->setText(QDir::currentPath());

  connect(ui->Btn_Online_StartRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StartRun_Clicked()));
  connect(ui->Btn_Online_StopRun, SIGNAL(clicked()), this, SLOT(Btn_Online_StopRun_Clicked()));
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

void MainWindow::Btn_Online_StartRun_Clicked()
{
  qDebug() << "Btn_Online_StartRun_Clicked... ";
}

void MainWindow::Btn_Online_StopRun_Clicked()
{
  qDebug() << "Btn_Online_StopRun_Clicked... ";
}
