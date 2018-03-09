#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include "GUIManager.hh"
#include <vector>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:

      void Action_Open_Triggered();
      void Action_Save_Triggered();
      void Action_Exit_Triggered();

      void Btn_Online_Choose_Clicked();
      void Btn_Online_Config_Clicked();
      void Btn_Online_StartRun_Clicked();
      void Btn_Online_StopRun_Clicked();

      void Selection_Changed();    
      void Mouse_Press();    
      void Mouse_Wheel();    

      void Update_Online_Image();
      void Draw_Online_Image();

  private:
    Ui::MainWindow *ui;

    void Init_Online_Image();

    int m_nx;
    int m_ny;
    GUIManager* m_GUIManager;

    std::string m_state;

    QCPColorMap* colorMap;
    QCPColorScale* colorScale;

    QThread* m_thread;
    QThread* m_thread_man;
    QTimer* m_timer;
};

#endif // MAINWINDOW_H

