#ifndef JADE_JADEPOST_HH
#define JADE_JADEPOST_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"


class JadePost;

class DLLEXPORT JadePost{
 public:
  virtual ~JadePost();
  virtual JadeOption Post(const std::string &url, const JadeOption &opt);
  
};



#endif
