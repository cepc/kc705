#ifndef JADEPIXFRAME_HH
#define JADEPIXFRAME_HH

// Jadepix
#include "JadepixDigi.hh"

// C++
#include <iostream>
#include <vector>

class JadepixFrame
{
    public:
        JadepixFrame();
        JadepixFrame(int frameId);
        ~JadepixFrame();

        void SetEventType(int etype) {m_event_type = etype;};
        void SetErrorFlag(int error){m_error_flag = error;};
        void SetEventCounter(int ecounter) {m_event_counter = ecounter;};
        void SetFrameId(int frameId) {m_frameId = frameId;};

        unsigned int GetEventType() {return m_event_type;};
        unsigned int GetErrorFlag() {return m_error_flag;};
        unsigned int GetEventCounter() {return m_event_counter;};
        long GetFrameId() {return m_frameId;};
        size_t NoOfDigi() { return m_digiVec.size(); };
        JadepixDigi *GetDigi(int digiId) const { return m_digiVec[digiId]; };

        void AddDigi(JadepixDigi *digi) { m_digiVec.push_back(digi); };

        void ReSet();

      private:
        unsigned int m_event_type;
        unsigned int m_error_flag;
        unsigned int m_event_counter;
        long m_frameId;
        std::vector<JadepixDigi *> m_digiVec;
        std::vector<JadepixDigi *>::iterator m_itdigi;
};


#endif //JADEPIXFRAME_HH
