#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GUIManager.hh"
#include "QRootCanvas.hh"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLayout>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QWidget>

#include <TCanvas.h>
#include <TF1.h>
#include <TFrame.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualX.h>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  private slots:

  void Action_Open_Triggered();
  void Action_Save_Triggered();
  void Action_Exit_Triggered();

  void Btn_Online_Choose_Clicked();
  void Btn_Online_Config_Clicked();
  void Btn_Online_StartRun_Clicked();
  void Btn_Online_StopRun_Clicked();

  void Init_Online_Image();
  void Update_Online_Image();

  void Handle_Root_Events();

  private:
  void Clear_Online_Image();

  private:
  Ui::MainWindow* ui;

  int m_nx;
  int m_ny;

  int m_col;
  int m_row;

  GUIManager* m_GUIManager;

  std::string m_state;

  QTimer* m_timer;
  QTimer* m_timer_run;
  QTimer* m_root_timer;
};

#endif // MAINWINDOW_H
