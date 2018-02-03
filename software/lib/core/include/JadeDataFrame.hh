#ifndef JADEPIX_JADEDATAFRAME
#define JADEPIX_JADEDATAFRAME

#include "JadeSystem.hh"

#include <string>
#include <vector>
#include <chrono>

class DLLEXPORT JadeDataFrame{
 public:
  JadeDataFrame(const std::string &data);
  JadeDataFrame(std::string &&data);
  JadeDataFrame(size_t nraw);
  JadeDataFrame() = delete;
  virtual ~JadeDataFrame();
  virtual void Decode();
  std::string& RawDataString();
  std::string& DescriptionString();
  uint32_t GetFrameCount() const;
  std::chrono::system_clock::time_point& TimeStamp();
  bool IsInMatrix(size_t x, size_t y) const;
  int16_t GetHitValue(size_t x, size_t y) const;
  uint32_t GetMatrixLowX() const;
  uint32_t GetMatrixLowY() const;
  uint32_t GetMatrixHighX() const;
  uint32_t GetMatrixHighY() const;
  uint32_t GetMatrixSizeX() const;
  uint32_t GetMatrixSizeY() const;
  void Print(std::ostream& os, size_t ws = 0) const;
 private:
  bool m_is_decoded;
  std::string m_data_raw;
  std::string m_description;
  std::chrono::system_clock::time_point m_ts;
  uint32_t m_frame_n;
  uint16_t m_offset_x;
  uint16_t m_offset_y;
  uint16_t m_n_x;
  uint16_t m_n_y;
  std::vector<int16_t> m_data;
};

using JadeDataFrameUP = std::unique_ptr<JadeDataFrame>;
using JadeDataFramePtr = std::shared_ptr<JadeDataFrame>;

#endif
