#include "JadeMonitor.hh"

//+++++++++++++++++++++++++++++++++++++++++
//TestMonitor.hh
class TestMonitor: public JadeMonitor{
 public:
  TestMonitor(const JadeOption& opt);
  ~TestMonitor() override {};
  void Monitor(JadeDataFrameSP df) override;
  JadeOption Post(const std::string &url, const JadeOption &opt) override;
  void Reset() override;
  
  std::string SendCommand(const std::string &cmd, const std::string &para) override;
 private:
  JadeOption m_opt;
  size_t m_ev_print;
  size_t m_ev_n;
  bool m_enable_print_discon;
  size_t m_last_df_n;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestMonitor.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestMonitor)));
  auto _test_ = JadeFactory<JadeMonitor>::Register<TestMonitor, const JadeOption&>(typeid(TestMonitor));
}

TestMonitor::TestMonitor(const JadeOption& opt)
  :m_opt(opt),m_ev_n(0), m_ev_print(0),
   m_last_df_n(0), m_enable_print_discon(false), JadeMonitor(opt){
  
  m_ev_print = opt.GetIntValue("PRINT_EVENT_N");
  m_enable_print_discon = opt.GetBoolValue("PRINT_EVENT_DISCONTINUOUS");
}

void TestMonitor::Monitor(JadeDataFrameSP df){
  if(m_ev_print!=0 && m_ev_n%m_ev_print == 0){
    df->Print(std::cout);
  }
  if(m_enable_print_discon){
    uint32_t df_n = df->GetFrameCount();
    if(m_last_df_n+1!=df_n && m_ev_n && m_last_df_n){
      std::cout<<"JadeMonitor: data frame counter is discontinuous "
               <<m_last_df_n<<" =>  "<< df_n<<std::endl;
    }
    m_last_df_n = df_n;
  }
  m_ev_n++;
}

void TestMonitor::Reset(){
  m_ev_n = 0;
  m_last_df_n = 0;
}

std::string TestMonitor::SendCommand(const std::string &cmd, const std::string &para){
  return "";
}

JadeOption TestMonitor::Post(const std::string &url, const JadeOption &opt){  
  
  if(url == "/set_print_event_n"){
    m_ev_print = opt.GetIntValue("PRINT_EVENT_N");
    return "{\"status\":true}";
  }
  
  if(url == "/set_print_event_discontinuous"){
    m_enable_print_discon = opt.GetBoolValue("PRINT_EVENT_DISCONTINUOUS");
    return "{\"status\":true}";
  }

  // if(url == "/set_enable_cds"){
  //   m_enable_cds = opt.GetBoolValue("ENABLE_CDS");
  //   return "{\"status\":true}";
  // }
  
  static const std::string url_base_class("/JadeMonitor/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeMonitor::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
