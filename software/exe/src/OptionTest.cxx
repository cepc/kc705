#include "JadeCore.hh"

int main(int argc, char **argv){
  JadeUtils::PrintTypeIndexMap();

  std::cout<<"end of app"<<std::endl;
  std::string inputstr("{\"key0\":0, \"key1\":\"1fefe\", \"key2\":2.1, \"key3\":{\"key3_1\":31, \"key3_2\":321}}");
  // std::string inputstr("abc");
  JadeOption opt(inputstr);
  std::cout<<opt.DumpString()<<std::endl;

  auto key0 = opt.GetSubOption("key0");
  std::cout<<key0.DumpString()<<std::endl;

  auto key1 = opt.GetSubOption("key1");
  std::cout<<key1.DumpString()<<std::endl;
  std::cout<<key1.GetStringValue()<<std::endl;

  auto key2 = opt.GetSubOption("key2");
  std::cout<<key2.DumpString()<<std::endl;

  auto key3 = opt.GetSubOption("key3");
  std::cout<<key3.DumpString()<<std::endl;

  auto key3_1 = key3.GetSubOption("key3_1");
  std::cout<<key3_1.DumpString()<<std::endl;
  
  std::cout<<"end of app"<<std::endl;

  auto a = JadeWriter::Make("JadeWriter", JadeOption("{\"PRINT_EVENT_N\":0}") );
  if(a)
    std::cout<< "great "<<std::endl;

  std::string b_(R"foo(std::cout<<key3_1.DufmpSftfring() <<std::endl;a a = JadeWrite::Make("JadeWrite", JadeOption("{\"PRINT_EVENT_N\":0}") );\n\n\n

\
bb
\
\
)foo");
  std::cout<<b_<<std::endl;
  std::string a_ = JadeUtils::Base64_btoa(b_);
  std::cout<<a_<<std::endl;
  std::string c_ = JadeUtils::Base64_atob(a_);
  std::cout<<c_<<std::endl;
  return 0;
}
