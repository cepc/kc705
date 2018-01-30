#ifndef JADEPIX_JADEDATAFRAME
#define JADEPIX_JADEDATAFRAME

#include "JadeSystem.hh"

#include <string>
#include <vector>

class DLLEXPORT JadeDataFrame{
 public:
  JadeDataFrame(const std::string &data);
  JadeDataFrame(std::string &&data);
  JadeDataFrame(size_t nraw);
  JadeDataFrame() = delete;
  virtual ~JadeDataFrame();
  virtual void Decode();
  std::string& RawDataString();
  uint16_t GetHitValue(size_t x, size_t y) const;
  bool IsInMatrix(size_t x, size_t y) const;
 private:
  std::string m_data_raw;
  bool m_is_decoded;
  uint16_t m_offset_x;
  uint16_t m_offset_y;
  uint16_t m_n_x;
  uint16_t m_n_y;
  std::vector<uint16_t> m_data;
};

using JadeDataFrameUP = std::unique_ptr<JadeDataFrame>;

#endif
