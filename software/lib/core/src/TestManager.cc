
#include "JadeManager.hh"

using namespace std::chrono_literals;

//+++++++++++++++++++++++++++++++++++++++++
//TestManager.hh
class TestManager: public JadeManager{
 public:
  TestManager(const JadeOption &opt);
  ~TestManager() override {};
  JadeOption Post(const std::string &url, const JadeOption &opt) override;
  void Init() override;
  void StartDataTaking() override;
  void StopDataTaking() override;

  JadeRegCtrlSP m_ctrl;
  JadeReaderSP m_rd;
  JadeFilterSP m_flt;
  JadeWriterSP m_wrt;
  JadeMonitorSP m_mnt;
  JadeOption m_opt;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestManager.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestManager)));
  auto _test_ = JadeFactory<JadeManager>::Register<TestManager, const JadeOption&>(typeid(TestManager));
}

TestManager::TestManager(const JadeOption &opt):
  JadeManager(opt), m_opt(opt){
}
  
JadeOption TestManager::Post(const std::string &url, const JadeOption &opt){
  static const std::string url_base_class("/JadeManager/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeManager::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}

void TestManager::Init(){
  MakeComponent();

  m_ctrl = GetRegCtrl();
  m_rd = GetReader();
  m_flt = GetFilter();
  m_wrt = GetWriter();
  m_mnt = GetMonitor();

  //hardware specific
  m_ctrl->Open();

  std::string chip_select = m_opt.GetStringValue("CHIP_SELECT");
  m_ctrl->SendCommand(chip_select);
  m_ctrl->SendCommand("SET");
  std::this_thread::sleep_for(200ms);
  m_ctrl->Close();
}

void TestManager::StartDataTaking(){  
  m_rd->Open();
  m_ctrl->Open();
  m_wrt->Open();
  m_flt->Reset();
  m_mnt->Reset();

  //hardware specific
  m_ctrl->SendCommand("STOP");
  std::this_thread::sleep_for(1s);
  m_ctrl->SendCommand("START");

  //Can threads start before the device's startup? 
  StartThread();
}

void TestManager::StopDataTaking(){
  //hardware specific  
  m_ctrl->SendCommand("STOP");  

  //Can threads terminate before the device's shutdown? 
  StopThread();
  m_rd->Close();
  m_ctrl->Close();
  m_wrt->Close();
}
