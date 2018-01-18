#ifndef JadepixDecoder_H
#define JadepixDecoder_H

// C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>

//Jadepix
#include "JadepixFrame.hh"
#include "JadepixDigi.hh"

class JadepixDecoder{
  public:
    JadepixDecoder():jadepix_rawFile(NULL),m_frameId(0){
      if(jadepix_decoder){ std::cout<<"Warning::IO is constructed twice."<< std::endl;}
      jadepix_decoder=this;
    };
    ~JadepixDecoder(){if(jadepix_rawFile) delete jadepix_rawFile;};

    int OpenRawFile(std::string);
    
    void CloseRawFile(){jadepix_rawFile->close();};

    void SetHeaderFooterWord();

    void InitVariables();

    void InitLoopCounter();

    void SetLoopCounter(int frame_number){decode_frame_num = frame_number;}

    void SkipTimeStamp();
    
    int FindFrameHeader();
    
    int FindFrameFooter();
    
    int DecodePixelData();
    
    int CompareTwoFrame();       

    int FillFrame();

    void ReSet();

    long NoOfFrame() { return m_frameVec.size(); };
    JadepixFrame *GetFrame(long frameId) const { return m_frameVec[frameId]; };

    int Decode(std::string datafile, int data_structure, int max_decode_frame);

    static JadepixDecoder* Instance();

  private:
    std::fstream* jadepix_rawFile;

    static JadepixDecoder* jadepix_decoder;

    unsigned long Frame_Header;
    unsigned long Frame_Footer;

    int decode_frame_num;

    int header_error_flag;
    int data_error_flag;
    int footer_error_flag;

    // Container for one frame
    signed short int adc[48][16];

    unsigned short int header_id[48];
    unsigned short int header_event_type[48];
    unsigned short int header_row_counter[48];
    unsigned short int header_counter_4bit[48];
    unsigned short int header_counter_16bit[48];

    unsigned short int header_id_pre[48];
    unsigned short int header_event_type_pre[48];
    unsigned short int header_row_counter_pre[48];
    unsigned short int header_counter_4bit_pre[48];
    unsigned short int header_counter_16bit_pre[48];

    unsigned short int footer_id[48];
    unsigned short int footer_event_type[48];
    unsigned short int footer_row_counter[48];
    unsigned short int footer_counter_4bit[48];
    unsigned short int footer_memaddr_counter[48];

    unsigned short int footer_id_pre[48];
    unsigned short int footer_event_type_pre[48];
    unsigned short int footer_row_counter_pre[48];
    unsigned short int footer_counter_4bit_pre[48];
    unsigned short int footer_memaddr_counter_pre[48];

    std::vector<JadepixFrame *> m_frameVec;
    std::vector<JadepixFrame *>::iterator m_itframe;

    long m_frameId;
};

#endif //JadepixDecoder_H
