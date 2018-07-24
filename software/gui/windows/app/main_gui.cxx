#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen; 
    splash->setPixmap(QPixmap("../software/gui/form/iheplogo.gif"));  
    splash->show();
    splash->showMessage("Copyright@2018 Institute of High Energy Physics,CAS",Qt::AlignRight | Qt::AlignBottom); 
    MainWindow *w = new MainWindow;
    splash->finish(w);
    w->show();
    delete splash;
    return a.exec();
}
