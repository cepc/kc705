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

      void Update_Online_Image();
      void Draw_Online_Image();

  private:
    Ui::MainWindow *ui;

    void Init_Online_Image();
    void Clear_Online_Image();

    int m_nx;
    int m_ny;

    int m_col;
    int m_row;

    GUIManager* m_GUIManager;

    std::string m_state;

    QCPLayoutGrid* m_LayoutTop;
    QCPLayoutGrid* m_LayoutBottom;
    
    QCPAxisRect* m_adcAxisRect;
    //QCPAxisRect* m_pedestalAxisRect;
    //QCPAxisRect* m_noiseAxisRect;
    QCPAxisRect* m_histADCAxisRect;
   
    QCPMarginGroup* m_marginGroup; 
    
    QCPColorMap* m_adcMap;
    QCPColorScale* m_adcScale;

    //QCPGraph* m_pedestalGraph;
    //QCPGraph* m_noiseGraph;
    QCPGraph* m_histADCGraph;

    QTimer* m_timer;
    QTimer* m_timer_run;
};

#endif // MAINWINDOW_H

