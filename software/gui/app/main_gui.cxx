#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen> 

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen; 
    splash->setPixmap(QPixmap("../software/gui/app/splash.png"));  
    splash->show(); 
    splash->showMessage("loading...",Qt::AlignRight | Qt::AlignBottom);
    MainWindow *w = new MainWindow;
    splash->finish(w); 
    w->show();
    delete splash;
    return a.exec();
}
