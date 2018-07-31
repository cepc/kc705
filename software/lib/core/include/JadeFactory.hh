#ifndef JADE_FACTORY_HH_
#define JADE_FACTORY_HH_
#include <map>
#include <memory>
#include <iostream>
#include <utility>
#include <functional>
#include <cstdint>

#include <typeindex>
#include <unordered_map>


template <typename B>
class JadeFactory{
    
public:

  using UP = std::unique_ptr<B, std::function<void(B*)> >;
  using SP = std::shared_ptr<B>;
  using WP = std::weak_ptr<B>;
  using SPC = std::shared_ptr<const B>;
    
  template <typename ...ARGS>
  static typename JadeFactory<B>::UP
  MakeUnique(const std::type_index& id, ARGS&& ...args);

  template <typename... ARGS>
  static std::unordered_map<std::type_index,
                            UP (*)(ARGS&&...)>&
  Instance();
    
  template <typename D, typename... ARGS>
  static std::uint64_t
  Register(const std::type_index& id);
  
private:
  template <typename D, typename... ARGS>
  static UP MakerFun(ARGS&& ...args){
    return UP(new D(std::forward<ARGS>(args)...), [](B *p) {delete p; });
  }
};

template <typename B>
template <typename ...ARGS>
typename JadeFactory<B>::UP
JadeFactory<B>::MakeUnique(const std::type_index& id, ARGS&& ...args){
  auto &ins = Instance<ARGS&&...>();
  std::remove_reference_t<decltype( ins.at(id) )> it;
  try{
    it = ins.at(id);
  }
  catch (const std::out_of_range& oor){
    std::cerr<<"JadeFactory:: ERROR, unable to find  "<< id.name() << " in " << (typeid(ins)).name() <<"\n";
    return nullptr;
  }
  return (*it)(std::forward<ARGS>(args)...);
};

    
template <typename B>
template <typename... ARGS>
std::unordered_map<std::type_index,
                   typename JadeFactory<B>::UP (*)(ARGS&&...)>&
JadeFactory<B>::Instance(){
  static std::unordered_map<std::type_index,
                            typename JadeFactory<B>::UP (*)(ARGS&&...)> m;
  static bool init = true;
  if(init){
    // std::cout<<"Instance a new JadeFactory<"<<static_cast<const void *>(&m)<<">"<<std::endl;
    init=false;
  }
  return m;
};
    
template <typename B>
template <typename D, typename... ARGS>
std::uint64_t
JadeFactory<B>::Register(const std::type_index& id){
  auto &ins = Instance<ARGS&&...>();
  ins[id] = &MakerFun<D, ARGS&&...>;
  return reinterpret_cast<std::uintptr_t>(&ins);
};

#endif
