#include "eudaq/StdEventConverter.hh"
#include "eudaq/RawEvent.hh"

#define PLANE_NUMBER_OFFSET 200
#define SIGNAL_THRESHOLD 60
class JadeRawEvent2StdEventConverter: public eudaq::StdEventConverter{
public:
  bool Converting(eudaq::EventSPC d1, eudaq::StdEventSP d2, eudaq::ConfigSPC conf) const override;
  static const uint32_t m_id_factory = eudaq::cstr2hash("JadeRaw");
};

namespace{
  auto dummy0 = eudaq::Factory<eudaq::StdEventConverter>::
    Register<JadeRawEvent2StdEventConverter>(JadeRawEvent2StdEventConverter::m_id_factory);
}

bool JadeRawEvent2StdEventConverter::Converting(eudaq::EventSPC d1, eudaq::StdEventSP d2, eudaq::ConfigSPC conf) const{
  auto ev = std::dynamic_pointer_cast<const eudaq::RawEvent>(d1);
  size_t nblocks= ev->NumBlocks();
  auto block_n_list = ev->GetBlockNumList();
  //TODO: check block;
  //TODO: check description;
  if(nblocks < 2)
    EUDAQ_THROW("Unknown data");
  
  std::vector<uint8_t> block_info = ev->GetBlock(0);
  uint16_t *data_info = reinterpret_cast<uint16_t*>( block_info.data());
  uint16_t x_n_pixel = *data_info;
  uint16_t y_n_pixel = *(data_info+1);
  uint32_t n_pixel = x_n_pixel * y_n_pixel;

  for(auto bn: block_n_list){
    if(bn == 0){
      //info block
      continue;
    }
    
    std::vector<uint8_t> block_decoded = ev->GetBlock(bn);
    uint16_t *data_decoded = reinterpret_cast<uint16_t*>( block_decoded.data());
    if(block_decoded.size() || block_decoded.size() != n_pixel *2 ){
      EUDAQ_THROW("Unknown data, pixel size mismatch");
    }
    
    eudaq::StandardPlane plane(PLANE_NUMBER_OFFSET+bn, "Jade", "Jade");
    plane.SetSizeZS(n_pixel, 1, 0); //TODO: check this function for its real meaning
    for(uint32_t i = 0; i< n_pixel; i++){
      uint16_t signal = *(data_decoded+i);
      if(signal > SIGNAL_THRESHOLD ){
        plane.PushPixel( i%x_n_pixel , i/x_n_pixel , signal);
      }
    }
    d2->AddPlane(plane);
  }
  return true;
}
