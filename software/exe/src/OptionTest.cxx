#include "JadeOption.hh"

int main(int argc, char **argv){
  std::string inputstr("{\"key0\":0, \"key1\":\"1fefe\", \"key2\":2.1, \"key3\":{\"key3_1\":31, \"key3_2\":321}}");
  // std::string inputstr("abc");
  auto opt = std::make_shared<JadeOption>(inputstr);
  std::cout<<opt->DumpString()<<std::endl;

  auto key0 = opt->GetSubOption("key0");
  std::cout<<key0->DumpString()<<std::endl;

  auto key1 = opt->GetSubOption("key1");
  std::cout<<key1->DumpString()<<std::endl;
  std::cout<<key1->GetStringValue()<<std::endl;

  auto key2 = opt->GetSubOption("key2");
  std::cout<<key2->DumpString()<<std::endl;

  auto key3 = opt->GetSubOption("key3");
  std::cout<<key3->DumpString()<<std::endl;

  auto key3_1 = key3->GetSubOption("key3_1");
  std::cout<<key3_1->DumpString()<<std::endl;
  
  std::cout<<"end of app"<<std::endl;
  return 0;
  
}
