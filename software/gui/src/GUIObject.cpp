#include "GUIObject.hh"

GUIObject::GUIObject(QObject *parent) : QObject(parent)
{
}

GUIObject::~GUIObject()
{
}

void GUIObject::Thread1(){
  qDebug() << QThread::currentThreadId();
  std::cout << "Thread1.."<<std::endl;
}

void GUIObject::Thread2(){
  qDebug() << QThread::currentThreadId();
  std::cout << "Thread2.."<<std::endl;
}

void GUIObject::Thread3(){
  qDebug() << QThread::currentThreadId();
  std::cout << "Thread3.."<<std::endl;
}
