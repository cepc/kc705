#include  "JadeFilter.hh"

JadeFilter::JadeFilter(const std::string &options)
  :m_options(options){
  
}

JadeFilter::~JadeFilter(){

}

JadeDataFrameUP JadeFilter::Filter(JadeDataFrameUP &&df) const {
  return std::move(df);
}

