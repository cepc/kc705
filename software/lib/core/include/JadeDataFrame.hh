#ifndef JADEPIX_JADEDATAFRAME
#define JADEPIX_JADEDATAFRAME

#include "JadeSystem.hh"

#include <string>
#include <vector>

class DLLEXPORT JadeDataFrame{
 public:
  JadeDataFrame(const std::string &data);
  JadeDataFrame(std::string &&data);
  JadeDataFrame() = delete;
  ~JadeDataFrame();

  
  
 private:
  std::string m_data_raw;

  struct  JadeDataHit{
    uint8_t m_col;
    uint8_t m_row;
    uint16_t m_value;
  };
  uint8_t m_n_col;
  uint8_t m_n_row;
  std::vector<JadeDataHit> m_data_hit;
};

using JadeDataFrameSP = std::shared_ptr<JadeDataFrame>;
using JadeDataFrameUP = std::unique_ptr<JadeDataFrame>;

#endif
