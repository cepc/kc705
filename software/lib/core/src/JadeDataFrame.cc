#include "JadeDataFrame.hh"
#include "JadeFactory.hh"
#include "JadeUtils.hh"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

using _base_c_ = JadeDataFrame;
using _index_c_ = JadeDataFrame;

// template class DLLEXPORT JadeFactory<_base_c_>;
// template DLLEXPORT
// std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
// JadeFactory<_base_c_>::Instance<const JadeOption&>();

// namespace{
//   auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
//   auto _loading_ = JadeFactory<_base_c_>::Register<_base_c_, const JadeOption&>(typeid(_index_c_));
// }

JadeDataFrame::JadeDataFrame(std::string&& data)
  : m_data_raw(std::move(data))
  , m_is_decoded(false)
  , m_n_x(0)
    , m_n_y(0)
{
}

JadeDataFrame::JadeDataFrame(std::string& data)
  : m_data_raw(data)
  , m_is_decoded(false)
  , m_n_x(0)
    , m_n_y(0)
{
}


JadeDataFrame::~JadeDataFrame()
{
}

const std::vector<int16_t>& JadeDataFrame::Data() const
{
  return m_data;
}

const std::string& JadeDataFrame::RawData() const
{
  return m_data_raw;
}

const std::string& JadeDataFrame::Description() const
{
  return m_description;
}

const std::chrono::system_clock::time_point&
JadeDataFrame::TimeStamp() const
{
  return m_ts;
}

std::vector<int16_t>& JadeDataFrame::Data()
{
  return m_data;
}

std::string& JadeDataFrame::RawData()
{
  return m_data_raw;
}

std::string& JadeDataFrame::Description()
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

uint32_t JadeDataFrame::GetMatrixHighX() const
{
  return m_n_x - 1;
}

uint32_t JadeDataFrame::GetMatrixHighY() const
{
  return m_n_y - 1;
}

uint32_t JadeDataFrame::GetMatrixSizeX() const
{
  return m_n_x;
}

uint32_t JadeDataFrame::GetMatrixSizeY() const
{
  return m_n_y;
}

uint32_t JadeDataFrame::GetTriggerN() const
{
  return m_trigger_n;
}

uint32_t JadeDataFrame::GetExtension() const
{
  return m_extension;
}

void JadeDataFrame::Decode()
{
  m_is_decoded = true;
  if (m_data_raw.size() <= 4) {
    std::cerr << "JadeDataFrame: no length word\n";
    throw;
  }
  const char* p_raw = m_data_raw.data();
  size_t p_offset = 0;
  uint32_t len_raw = LE32TOH(*reinterpret_cast<const uint32_t*>(p_raw + p_offset));
  // Matrix A: size=1936
  // Matrix B: size=3856
  if(len_raw == 1936){
    m_n_x = 16;
    m_n_y = 48;
  }else if(len_raw == 3856){
    m_n_x = 16;
    m_n_y = 96;
  }else if (len_raw != m_data_raw.size()) {
    std::cerr << "JadeDataFrame: raw data length does not match\n";
    throw;
  }
  p_offset +=4;

  m_data.clear();
  m_data.resize(m_n_x * m_n_y, 0);

  uint64_t header32 = LE32TOH(*reinterpret_cast<const uint32_t*>(p_raw + p_offset));
  if (header32 != 0xaaaaaaaa) {
    std::cerr << "JadeDataFrame: data frame header is incorrect\n";
    throw;
  }
  p_offset += 4;
  m_extension = LE16TOH(*reinterpret_cast<const uint16_t*>(p_raw + p_offset));
  p_offset += 2;
  m_trigger_n = LE16TOH(*reinterpret_cast<const uint16_t*>(p_raw + p_offset));
  p_offset += 2;

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
  if (x < m_n_x && y < m_n_y)
    return true;
  else
    return false;
}

int16_t JadeDataFrame::GetHitValue(size_t x, size_t y) const
{
  size_t pos = x + m_n_x * y;
  int16_t val = m_data.at(pos);
  return val;
}

void JadeDataFrame::Print(std::ostream& os, size_t ws) const
{
  os << std::string(ws, ' ') << "{ name:JadeDataFrame,\n";
  os << std::string(ws + 2, ' ') << "is_decoded:" << m_is_decoded << ",\n";
  if (m_is_decoded) {
    os << std::string(ws + 2, ' ') << "description:"
      << "TODO"
      << ",\n";
    os << std::string(ws + 2, ' ') << "ts:"
      << "TODO"
      << ",\n";
    os << std::string(ws + 2, ' ') << "frame_n:" << m_frame_n << ",\n";
    os << std::string(ws + 2, ' ') << "n_x:" << m_n_x << ",\n";
    os << std::string(ws + 2, ' ') << "n_y:" << m_n_y << ",\n";
    if (m_n_x != 0 && m_n_y != 0) {
      os << std::string(ws + 2, ' ') << "data:[\n";
      for (size_t iy = 0; iy < m_n_y; iy++) {
        os << std::string(ws + 4, ' ') << "{row_y:" << iy
          << ",value:[" << GetHitValue(0, 0);
        for (size_t ix = 1; ix < m_n_x; ix++) {
          os << "," << GetHitValue(ix, iy);
        }
        os << "]}\n";
      }
      os << std::string(ws + 2, ' ') << "]\n";
    }
  }
  os << std::string(ws, ' ') << "}\n";
}

// JadeDataFrame::CdsAndRemoveRaw(JadeDataFrame& df)
// {
//   m_data_raw.clear();
//   m_description.append(":CDS:NORAW");
//   //NOTE: a `for loop` might be more efficient.
//   if(m_is_decoded && df.m_is_decoded)
//     std::transform(df.m_data.begin(), df.m_data.end(), m_data.begin(), m_cds_frame_adc.begin(),
//                    std::minus<int16_t>());
// }

// JadeDataFrame::CdsAndAppendRaw(JadeDataFrame& df)
// {
//   m_data_raw.append(df.RawData());
//   m_description.append(":CDS:APPENDRAW");
//   if(m_is_decoded && df.m_is_decoded)
//     std::transform(df.m_data.begin(), df.m_data.end(), m_data.begin(), m_cds_frame_adc.begin(),
//                    std::minus<int16_t>());
// }

JadeDataFrameSP JadeDataFrame::CdsAndReturnNewObject(const JadeDataFrame& earlier,
    const JadeDataFrame& later)
{

  JadeDataFrameSP dfsp(new JadeDataFrame(""));
  auto& df = *dfsp;
  auto& data_cds_result = df.Data();
  auto& data_earlier = earlier.Data();
  auto& data_later = later.Data();
  data_cds_result.resize(data_later.size());
  std::transform(data_later.begin(), data_later.end(), data_earlier.begin(),
      data_cds_result.begin(),
      std::minus<int16_t>());
  df.m_is_decoded = true;
  df.m_description = later.m_description + "+CDS";
  df.m_ts = later.m_ts;
  df.m_trigger_n = later.m_trigger_n;
  df.m_extension = later.m_extension;
  df.m_ts = later.m_ts;
  df.m_frame_n = later.m_frame_n;
  df.m_n_x = later.m_n_x;
  df.m_n_y = later.m_n_y;
  return dfsp;
}
