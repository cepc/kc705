#include "JadeMonitor.hh"
#include "JadeUtils.hh"

using _base_c_ = JadeMonitor;
using _index_c_ = JadeMonitor;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

JadeMonitor::JadeMonitor(const JadeOption& opt)
  :m_opt(opt),m_ev_n(0), m_ev_print(0),
   m_last_df_n(0), m_enable_print_discon(false){
  m_ev_print = opt.GetIntValue("PRINT_EVENT_N");
  m_enable_print_discon = opt.GetBoolValue("PRINT_EVENT_DISCONTINUOUS");
}

JadeMonitor::~JadeMonitor(){

}

JadeMonitorSP JadeMonitor::Make(const std::string& name, const JadeOption& opt){  
  try{
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeMonitorSP wrt =  JadeFactory<JadeMonitor>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  }
  catch(...){
    std::cout<<"TODO"<<std::endl;
    return nullptr;
  }
}

void JadeMonitor::Reset(){
  m_ev_n = 0;
}

void JadeMonitor::Monitor(JadeDataFrameSP df){
  if(m_ev_print!=0 && m_ev_n%m_ev_print == 0){
    df->Print(std::cout);
  }
  m_ev_n++;

  if(m_enable_print_discon){
    uint32_t df_n = df->GetFrameCount();
    if(m_last_df_n!=0 && m_last_df_n+1!=df_n){
    std::cout<<"JadeMonitor: data frame counter is discontinuous "
	     <<m_last_df_n<<" =>  "<< df_n<<std::endl;
    }
    m_last_df_n = df_n;
  }
}


JadeOption JadeMonitor::Post(const std::string &url, const JadeOption &opt){    
    if(url == "/reload_opt"){
    m_opt = opt;
    return "{\"status\":ture}";
  }

  if(url == "/reset"){
    Reset();
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadePost/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadePost::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
