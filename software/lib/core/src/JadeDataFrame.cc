#include "JadeDataFrame.hh"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

JadeDataFrame::JadeDataFrame(std::string&& data)
    : m_data_raw(std::move(data))
    , m_is_decoded(false)
    , m_is_cds(false)
    , m_offset_x(0)
    , m_offset_y(0)
    , m_n_x(0)
    , m_n_y(0)
    , m_cds_frame_adc({ 0 })
{
}

JadeDataFrame::JadeDataFrame(const std::string& data)
    : m_data_raw(data)
    , m_is_decoded(false)
    , m_is_cds(false)
    , m_offset_x(0)
    , m_offset_y(0)
    , m_n_x(0)
    , m_n_y(0)
    , m_cds_frame_adc({ 0 })
{
}

JadeDataFrame::JadeDataFrame(size_t nraw)
    : m_is_decoded(false)
    , m_is_cds(false)
    , m_offset_x(0)
    , m_offset_y(0)
    , m_n_x(0)
    , m_n_y(0)
    , m_cds_frame_adc({ 0 })
{
  m_data_raw.resize(nraw);
}

JadeDataFrame::~JadeDataFrame()
{
}

std::string& JadeDataFrame::RawDataString()
{
  return m_data_raw;
}

std::string& JadeDataFrame::DescriptionString()
{
  return m_description;
}

std::chrono::system_clock::time_point&
JadeDataFrame::TimeStamp()
{
  return m_ts;
}

uint32_t JadeDataFrame::GetFrameCount() const
{
  return m_frame_n;
}

uint32_t JadeDataFrame::GetMatrixLowX() const
{
  return m_offset_x;
}

uint32_t JadeDataFrame::GetMatrixLowY() const
{
  return m_offset_y;
}

uint32_t JadeDataFrame::GetMatrixHighX() const
{
  return m_offset_x + m_n_x - 1;
}

uint32_t JadeDataFrame::GetMatrixHighY() const
{
  return m_offset_y + m_n_y - 1;
}

uint32_t JadeDataFrame::GetMatrixSizeX() const
{
  return m_n_x;
}

uint32_t JadeDataFrame::GetMatrixSizeY() const
{
  return m_n_y;
}

std::vector<int16_t> JadeDataFrame::GetFrameCDS()
{
  return m_cds_frame_adc;
}

bool JadeDataFrame::GetCDSStatus()
{
  return m_is_cds;
}

void JadeDataFrame::Decode()
{
  m_is_decoded = true;
  if (m_data_raw.size() != 1928) {
    std::cerr << "JadeDataFrame: unable to decode\n";
    throw;
  }
  m_n_x = 16;
  m_n_y = 48;
  m_data.clear();
  m_data.resize(m_n_x * m_n_y, 0);
  const char* p_raw = m_data_raw.data();
  size_t p_offset = 0;
  uint64_t header32 = LE32TOH(*reinterpret_cast<const uint32_t*>(p_raw + p_offset));
  if (header32 != 0xaaaaaaaa) {
    std::cerr << "JadeDataFrame: data frame header is incorrect\n";
    throw;
  }
  p_offset += 4;
  int16_t* p_data = m_data.data();
  bool is_first_row = true;
  for (size_t yn = 0; yn < m_n_y; yn++) {
    //Y head 4 bytes
    p_offset += 4;
    for (size_t xn = 0; xn < m_n_x; xn++) {
      int16_t val = LE16TOH(*reinterpret_cast<const uint16_t*>(p_raw + p_offset));
      *p_data = val;
      p_data++;
      p_offset += 2;
    }
    //Y tail 4 bytes
    if (is_first_row) {
      m_frame_n = LE16TOH(*reinterpret_cast<const uint16_t*>(p_raw + p_offset));
      is_first_row = false;
    }
    p_offset += 4;
  }

  uint64_t footer32 = LE32TOH(*reinterpret_cast<const uint32_t*>(p_raw + p_offset));
  if (footer32 != 0xf0f0f0f0) {
    std::cerr << "JadeDataFrame: data frame footer is incorrect\n";
    throw;
  }

  p_offset += 4;
  if (p_offset != m_data_raw.size()) {
    std::cerr << "JadeDataFrame: raw data size is incorrect\n";
    throw;
  }
  return;
}

bool JadeDataFrame::IsInMatrix(size_t x, size_t y) const
{
  if (x - m_offset_x < m_n_x && y - m_offset_y < m_n_y && x >= m_offset_x && y >= m_offset_y)
    return true;
  else
    return false;
}

int16_t JadeDataFrame::GetHitValue(size_t x, size_t y) const
{
  size_t pos = (x - m_offset_x) + m_n_x * (y - m_offset_y);
  int16_t val = m_data.at(pos);
  return val;
}

std::vector<int16_t> JadeDataFrame::GetFrameData() const
{
  return m_data;
}

void JadeDataFrame::Print(std::ostream& os, size_t ws) const
{
  os << std::string(ws, ' ') << "{ name:JadeDataFrame,\n";
  os << std::string(ws + 2, ' ') << "data_raw:"
     << "TODO"
     << ",\n";
  os << std::string(ws + 2, ' ') << "is_decoded:" << m_is_decoded << ",\n";
  if (m_is_decoded) {
    os << std::string(ws + 2, ' ') << "description:"
       << "TODO"
       << ",\n";
    os << std::string(ws + 2, ' ') << "ts:"
       << "TODO"
       << ",\n";
    os << std::string(ws + 2, ' ') << "frame_n:" << m_frame_n << ",\n";
    os << std::string(ws + 2, ' ') << "offset_x:" << m_offset_x << ",\n";
    os << std::string(ws + 2, ' ') << "offset_y:" << m_offset_y << ",\n";
    os << std::string(ws + 2, ' ') << "n_x:" << m_n_x << ",\n";
    os << std::string(ws + 2, ' ') << "n_y:" << m_n_y << ",\n";
    if (m_n_x != 0 && m_n_y != 0) {
      os << std::string(ws + 2, ' ') << "data:[\n";
      for (size_t iy = 0; iy < m_n_y; iy++) {
        os << std::string(ws + 4, ' ') << "{row_y:" << iy + m_offset_y
           << ",value:[" << GetHitValue(m_offset_x, m_offset_y);
        for (size_t ix = 1; ix < m_n_x; ix++) {
          os << "," << GetHitValue(ix + m_offset_x, iy + m_offset_y);
        }
        os << "]}\n";
      }
      os << std::string(ws + 2, ' ') << "]\n";
    }
  }
  os << std::string(ws, ' ') << "}\n";
}

JadeDataFrame JadeDataFrame::operator-(JadeDataFrame& df)
{
  m_cds_frame_adc.resize(m_n_x * m_n_y, 0);
  std::transform(df.m_data.begin(), df.m_data.end(), this->m_data.begin(), this->m_cds_frame_adc.begin(), std::minus<int16_t>());
  m_is_cds = true;
  return *this;
}
