#ifndef GUIManager_HH
#define GUIManager_HH

#include <QDebug>
#include <QObject>

#include "GUIMonitor.hh"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>


class GUIManager : public QObject {
  Q_OBJECT

public:
  GUIManager();
  ~GUIManager();
  
  void set_config_path(std::string config_path){ m_opt_config_file = config_path; };
  std::shared_ptr<GUIMonitor> get_monitor();
                                           
public slots:
  void start_run();
  void stop_run();
  void config();

private:
  std::string m_opt_config_file;
  JadeManagerSP m_man;
signals:
  void IsRunning();
  void IsStop();
  
};

#endif //GUIManager_HH
