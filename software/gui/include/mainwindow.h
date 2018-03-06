#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

      void Btn_Online_StartRun_Clicked();
      void Btn_Online_StopRun_Clicked();

  private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

