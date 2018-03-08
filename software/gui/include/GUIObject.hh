#include <QObject>
#include <QDebug>
#include <QThread>
#include <iostream>

class GUIObject : public QObject
{
    Q_OBJECT
  public:
      explicit GUIObject(QObject *parent=0);
      ~GUIObject();

  signals:

  public slots:
    void Thread1();
    void Thread2();
    void Thread3();
  
};

