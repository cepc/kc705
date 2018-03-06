#include  "JadeFilter.hh"

using namespace std::chrono_literals;

JadeFilter::JadeFilter(const std::string &options)
  :m_options(options){
}

JadeFilter::~JadeFilter(){
}

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df){
  return df;
}
