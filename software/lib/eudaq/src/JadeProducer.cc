#include "eudaq/Producer.hh"
#include "JadeCore.hh"

#include <iostream>
#include <chrono>
#include <thread>

namespace{
  auto _loading_print_
    = JadeUtils::FormatPrint(std::cout, "<INFO> %s  this eudaq module is loaded from %s\n\n",
                             JadeUtils::GetNowStr().c_str(), JadeUtils::GetBinaryPath().c_str())
    ;
}

class EudaqWriter: public JadeWriter {
public:
  EudaqWriter(const JadeOption &opt);
  ~EudaqWriter() override;
  void Write(JadeDataFrameSP df) override;
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  void SetProducerCallback(eudaq::Producer *producer);
private:
  eudaq::Producer *m_producer;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestWrite.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(EudaqWriter)));
  auto _test_ = JadeFactory<JadeWriter>::Register<EudaqWriter, const JadeOption&>(typeid(EudaqWriter));
}

void EudaqWriter::SetProducerCallback(eudaq::Producer *producer){
  m_producer = producer;
}

EudaqWriter::EudaqWriter(const JadeOption &opt)
  :JadeWriter(opt), m_producer(nullptr)
{
  
}

EudaqWriter::~EudaqWriter(){
  
}

JadeOption EudaqWriter::Post(const std::string &url, const JadeOption &opt){
  static const std::string url_base_class("/JadeWriter/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeWriter::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}

void EudaqWriter::Write(JadeDataFrameSP df){
  if(m_producer){
    auto ev = eudaq::Event::MakeUnique("JadeRaw");
    ev->SetTriggerN(df->GetFrameCount());

    std::vector<uint16_t> v_info;
    v_info.push_back(df->GetMatrixSizeX());
    v_info.push_back(df->GetMatrixSizeY());
    v_info.push_back(df->GetTriggerExtension());
    v_info.push_back(df->GetTriggerSerialOrder());
    ev->AddBlock<uint16_t>((uint32_t)0, v_info);

    ev->AddBlock<int16_t>((uint32_t)1, df->Data());
    if(m_producer)
      m_producer->SendEvent(std::move(ev));
  }
  
}

class JadeProducer : public eudaq::Producer {
public:
  using eudaq::Producer::Producer;
  ~JadeProducer() override {};
  void DoInitialise() override;
  void DoConfigure() override;
  void DoStartRun() override;
  void DoStopRun() override;
  void DoTerminate() override;
  void DoReset() override;
  
  static const uint32_t m_id_factory = eudaq::cstr2hash("JadeProducer");
private:
  JadeManagerSP m_jade_man;
};

namespace{
  auto _reg_ = eudaq::Factory<eudaq::Producer>::
    Register<JadeProducer, const std::string&, const std::string&>(JadeProducer::m_id_factory);
}

void JadeProducer::DoInitialise(){
  auto ini = GetInitConfiguration();
  std::string json_base64  = ini->Get("JSON_BASE64", "");
  std::string json_str;
  if(json_base64.empty()){
    std::string json_path  = ini->Get("JSON_PATH", "");
    json_str = JadeUtils::LoadFileToString(json_path);
  }
  else{
    json_str = JadeUtils::Base64_atob(json_base64);
  }
  JadeOption opt_conf(json_str);
  JadeOption opt_man = opt_conf.GetSubOption("JadeManager");
  std::cout<<opt_man.DumpString()<<std::endl;
  std::string man_type = opt_man.GetStringValue("type");
  JadeOption opt_man_para = opt_man.GetSubOption("parameter");
  m_jade_man = JadeManager::Make(man_type, opt_man_para);

  m_jade_man->Init();
  auto writer = m_jade_man->GetWriter();
  auto eudaq_writer = std::dynamic_pointer_cast<EudaqWriter>(writer);
  if(!eudaq_writer){
    std::cerr<<"JadeProducer: there is no instance of EudaqWriter in "<<man_type<<std::endl;
    return;
  }
  eudaq_writer->SetProducerCallback(this);
}

void JadeProducer::DoConfigure(){
  //do nothing here
}

void JadeProducer::DoStartRun(){
  m_jade_man->StartDataTaking();
}

void JadeProducer::DoStopRun(){
  m_jade_man->StopDataTaking();
}

void JadeProducer::DoReset(){
  m_jade_man.reset();
}

void JadeProducer::DoTerminate(){
  m_jade_man.reset();
  std::terminate();
}
