#include  "JadeFilter.hh"

using namespace std::chrono_literals;

JadeFilter::JadeFilter(const std::string &options)
  :m_options(options){
  m_ev_n = 0;
  if(!options.empty())
    m_ev_print = std::stoul(options);
}

JadeFilter::~JadeFilter(){

}

JadeDataFrameUP JadeFilter::Filter(JadeDataFrameUP &&df) const {
  //if(m_ev_n%m_ev_print == 0){
  //  df->Print(std::cout);
  //}
  return std::move(df);
}

