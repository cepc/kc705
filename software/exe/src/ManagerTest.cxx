#include "JadeManager.hh"

#include <iostream>
int main(int n, char **args){
  
  auto pman = new JadeManager();
  pman->Start("/home/wilkie/workspace/jadepix/kc705/data/RawRunData_0002.dat", "/home/wilkie/workspace/jadepix/kc705/data/output");
  std::cout<<"\nwaiting\n"<<std::endl;
  int i;
  std::cin>> i;
  delete pman;
  return 0;
}
