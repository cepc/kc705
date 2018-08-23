#include "eudaq/LCEventConverter.hh"
#include "eudaq/RawEvent.hh"

#include "IMPL/TrackerRawDataImpl.h"
#include "IMPL/TrackerDataImpl.h"
#include "UTIL/CellIDEncoder.h"


#define PLANE_ID_OFFSET_LICO 50

class JadeRawEvent2LCEventConverter: public eudaq::LCEventConverter{
  typedef std::vector<uint8_t>::const_iterator datait;
public:
  bool Converting(eudaq::EventSPC d1, eudaq::LCEventSP d2, eudaq::ConfigSPC conf) const override;
  static const uint32_t m_id_factory = eudaq::cstr2hash("JadeRaw");
};
  
namespace{
  auto dummy0 = eudaq::Factory<eudaq::LCEventConverter>::
    Register<JadeRawEvent2LCEventConverter>(JadeRawEvent2LCEventConverter::m_id_factory);
}

bool JadeRawEvent2LCEventConverter::Converting(eudaq::EventSPC d1, eudaq::LCEventSP d2,
						 eudaq::ConfigSPC conf) const{
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

  lcio::LCCollectionVec *zsDataCollection = nullptr;
  auto p_col_names = d2->getCollectionNames();
  if(std::find(p_col_names->begin(), p_col_names->end(), "zsdata_jade") != p_col_names->end()){
    zsDataCollection = dynamic_cast<lcio::LCCollectionVec*>(d2->getCollection("zsdata_jade"));
    if(!zsDataCollection)
      EUDAQ_THROW("dynamic_cast error: from lcio::LCCollection to lcio::LCCollectionVec");
  }
  else{
    zsDataCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERDATA);
    d2->addCollection(zsDataCollection, "zsdata_jade");
  }  
  
  for(auto bn: block_n_list){
    if(bn == 0){
      //info block
      continue;
    }  
    std::vector<uint8_t> block_decoded = ev->GetBlock(bn);
    uint16_t *data_decoded = reinterpret_cast<uint16_t*>( block_decoded.data());
    if(! block_decoded.size() || block_decoded.size() != n_pixel *2 ){
      EUDAQ_THROW("Unknown data, pixel size mismatch");
    }

    lcio::CellIDEncoder<lcio::TrackerDataImpl> zsDataEncoder("sensorID:7,sparsePixelType:5",
                                                           zsDataCollection);
    zsDataEncoder["sensorID"] = PLANE_ID_OFFSET_LICO + bn;
    zsDataEncoder["sparsePixelType"] = 2;
    auto zsFrame = new lcio::TrackerDataImpl;
    zsDataEncoder.setCellID(zsFrame);

    for(uint32_t i = 0; i< n_pixel; i++){    
      zsFrame->chargeValues().push_back(i%x_n_pixel);//x
      zsFrame->chargeValues().push_back(i/x_n_pixel);//y
      zsFrame->chargeValues().push_back(*(data_decoded+i));//signal
      zsFrame->chargeValues().push_back(0);//time
    }
    zsDataCollection->push_back(zsFrame);
  }
  return true; 
}
