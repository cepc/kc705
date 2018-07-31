#include "GUIManager.hh"

using namespace std::chrono_literals;

GUIManager::GUIManager()
    : m_opt_config_file("sample_gui.json")
{
  m_man = new JadeManager(JadeOption("{}"));
}

GUIManager::~GUIManager()
{
}

std::string GUIManager::get_now_str()
{
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%c");
  return ss.str();
}

void GUIManager::start_run()
{
  m_man->DeviceConnect();
  m_man->DeviceControl("STOP");
  std::this_thread::sleep_for(1s);
  std::cout << "=========start at " << get_now_str() << "=======" << std::endl;
  m_man->DeviceControl("START");
  std::cout << "=========" << std::endl;
  m_man->StartDataTaking();
  std::cout << "=========" << std::endl;
  emit IsRunning();
}

void GUIManager::stop_run()
{
  emit IsStop();
  std::cout << "=========" << std::endl;
  m_man->StopDataTaking();
  std::cout << "=========" << std::endl;
  m_man->DeviceControl("STOP");
  std::this_thread::sleep_for(1s);
  std::cout << "=========exit at " << get_now_str() << "=======" << std::endl;
  m_man->DeviceDisconnect();
}

void GUIManager::config()
{
  std::ifstream ifs(m_opt_config_file);
  if (!ifs.good()) {
    std::cerr << "Unable to open configrue file in PATH: " << m_opt_config_file << "\n";
    throw;
  }

  std::string config_str;
  config_str.assign((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));

  JadeOption conf(config_str);
  JadeOption conf_core = conf.GetSubOption("JadeCore");

  JadeOption conf_man = conf_core.GetSubOption("JadeManager");
  JadeOption conf_man_para = conf_man.GetSubOption("parameter");
  JadeOption conf_ctrl = conf_core.GetSubOption("JadeRegCtrl");
  JadeOption conf_ctrl_para = conf_ctrl.GetSubOption("parameter");
  JadeOption conf_read = conf_core.GetSubOption("JadeRead");
  JadeOption conf_read_para = conf_read.GetSubOption("parameter");
  JadeOption conf_flt = conf_core.GetSubOption("JadeFilter");
  JadeOption conf_flt_para = conf_flt.GetSubOption("parameter");
  JadeOption conf_wrt = conf_core.GetSubOption("JadeWrite");
  JadeOption conf_wrt_para = conf_wrt.GetSubOption("parameter");
  JadeOption conf_mnt = conf_core.GetSubOption("GUIMonitor");
  JadeOption conf_mnt_para = conf_mnt.GetSubOption("parameter");

  std::cout << conf_man_para.DumpString() << std::endl;
  std::cout << conf_ctrl_para.DumpString() << std::endl;
  std::cout << conf_read_para.DumpString() << std::endl;
  std::cout << conf_flt_para.DumpString() << std::endl;
  std::cout << conf_wrt_para.DumpString() << std::endl;
  std::cout << conf_mnt_para.DumpString() << std::endl;

  JadeRegCtrlSP pctrl = std::make_shared<JadeRegCtrl>(conf_ctrl_para);
  JadeReadSP pread = std::make_shared<JadeRead>(conf_read_para);
  JadeFilterSP pflt = std::make_shared<JadeFilter>(conf_flt_para);
  JadeWriteSP pwrt = std::make_shared<JadeWrite>(conf_wrt_para);
  m_mnt = std::make_shared<GUIMonitor>(conf_mnt_para);

  size_t nsec = conf_man_para.GetIntValue("SecPerLoop");
  size_t nloop = conf_man_para.GetIntValue("N_Loops");
  std::string chip_address = conf_man_para.GetStringValue("ChipAddress");

  m_man->SetRegCtrl(pctrl);
  m_man->SetReader(pread);
  m_man->SetFilter(pflt);
  m_man->SetWriter(pwrt);
  m_man->SetMonitor(std::dynamic_pointer_cast<JadeMonitor>(m_mnt));
  m_man->DeviceConnect();
  m_man->DeviceControl(chip_address);
  m_man->DeviceControl("SET");
  std::cout << "Select address: " << chip_address << std::endl;
  m_man->DeviceDisconnect();
}

std::shared_ptr<GUIMonitor> GUIManager::get_monitor()
{
  return m_mnt;
}
