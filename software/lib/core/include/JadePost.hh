#ifndef JADE_JADEPOST_HH
#define JADE_JADEPOST_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"

class JadePost;
using JadePostSP = JadeFactory<JadePost>::SP;
using JadePostWP = JadeFactory<JadePost>::WP;

class DLLEXPORT JadePost{
 public:
  virtual ~JadePost();
  virtual JadeOption Post(const std::string &url, const JadeOption &opt);
  void AddSubPost(JadePostSP post);
  static void SetSystemPost(JadePostSP post);
  static JadePostSP GetSystemPost();  
 private:
  std::unordered_map<std::string, JadePostWP> m_map_folder_post;
  static JadePostWP s_sys_post;
};

#endif
