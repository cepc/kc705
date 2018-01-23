// Jadepix
#include "JadepixFrame.hh"

JadepixFrame::JadepixFrame()
{
};

JadepixFrame::JadepixFrame(int frameId) : m_frameId(frameId)
{
};

JadepixFrame::~JadepixFrame()
{
  ReSet();
}

void JadepixFrame::ReSet()
{
  for (m_itdigi = m_digiVec.begin(); m_itdigi != m_digiVec.end(); ++m_itdigi)
  {
    delete *m_itdigi;
  }
  m_digiVec.clear();
}
