#include  "JadeFilter.hh"

JadeFilter::JadeFilter(const std::string &options)
  :m_options(options){
  
}

JadeFilter::~JadeFilter(){

}

bool JadeFilter::Filter(const JadeDataFrame &) const {
  return true;
}
