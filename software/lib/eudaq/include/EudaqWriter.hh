#include "JadeCore.hh"


namespace{
  using _base_c_ = JadeWrite;
  using _index_c_ = EudaqWriter;
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

class EudaqWriter: public JadeWrite, public eudaq::Producer{
  EudaqWriter(const JadeOption &opt);
  ~EudaqWriter() override;

  void DoConfigure() override;
  void DoStartRun() override;
  void DoStopRun() override;
  void DoReset() override;
  
  void SetJadeManagerCallback(JadeManagerSP man);
  
  void Write(JadeDataFrameSp df) override;
  JadeOpton Post(const std::string &url, const JadeOption &opt) override;
  
private:
  JadeManagerSP m_man;
}
