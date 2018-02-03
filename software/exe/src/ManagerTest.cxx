#include "JadeManager.hh"
#include "JadeRegCtrl.hh"
#include <iostream>
#include <chrono>
#include <thread>

int main(int n, char **args){
	using namespace std::chrono_literals;
	auto preg = new JadeRegCtrl("//./xillybus_mem_8");
	preg->WriteByte(4, 15);
	std::this_thread::sleep_for(100ms);
	preg->WriteByte(3, 15);
  auto pman = new JadeManager();
  //pman->Start("/home/wilkie/workspace/jadepix/kc705/data/RawRunData_0002.dat", "/home/wilkie/workspace/jadepix/kc705/data/output");
  pman->Start("//./xillybus_read_32", "2018020320");
  std::cout<<"\nwaiting\n"<<std::endl;
  int i;
  std::cin>> i;
  delete pman;
  return 0;
}
