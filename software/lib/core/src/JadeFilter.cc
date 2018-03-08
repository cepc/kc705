#include  "JadeFilter.hh"

using namespace std::chrono_literals;

JadeFilter::JadeFilter(const JadeOption &opt)
  :m_opt(opt){
}

JadeFilter::~JadeFilter(){
}

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df){
  return df;
}
