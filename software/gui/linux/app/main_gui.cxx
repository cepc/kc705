#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

#include <stdlib.h>
#include <TApplication.h>
#include <TSystem.h>

int main(int argc, char* argv[])
{
  TApplication rootapp("Qt ROOT Application", &argc, argv);
  QApplication app(argc, argv);
  
  QSplashScreen* splash = new QSplashScreen;
  splash->setPixmap(QPixmap("../software/gui/linux/form/iheplogo.gif"));
  splash->show();
  splash->showMessage("Copyright@2018 Institute of High Energy Physics,CAS", Qt::AlignRight | Qt::AlignBottom);
  
  MainWindow* m = new MainWindow;
  splash->finish(m);
  delete splash;
  
  m->resize(m->sizeHint());
  m->setWindowTitle("JadePix DAQ");
  m->show();

  return app.exec();
}
