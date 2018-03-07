#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include "monitor.hh"
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

      void Btn_Online_Config_Clicked();
      void Btn_Online_StartRun_Clicked();
      void Btn_Online_StopRun_Clicked();
      
      void Selection_Changed();    
      void Mouse_Press();    
      void Mouse_Wheel();    

  private:
    Ui::MainWindow *ui;

    void Init_Online_Image();
    void Online_Update();
    void Draw_Online_Image();
    QCPColorMapData* Matrix_To_Data(const std::vector<int16_t>&);
    std::vector<int16_t> Generate_Fake_Data();

    int m_nx;
    int m_ny;
    Monitor* m_monitor;

};

#endif // MAINWINDOW_H

