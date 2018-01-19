#include "JadepixDecoder.hh"

JadepixDecoder *JadepixDecoder::jadepix_decoder = NULL;

JadepixDecoder *JadepixDecoder::Instance()
{
  if (!jadepix_decoder)
    jadepix_decoder = new JadepixDecoder();
  return jadepix_decoder;
}

int JadepixDecoder::OpenRawFile(std::string fname)
{
  jadepix_rawFile = new std::fstream();
  jadepix_rawFile->open(fname, std::ios::binary | std::ios::in);

  if (!jadepix_rawFile)
  {
    std::cout << "" << std::endl;
    std::cout << "------------> Error:: " << fname << " can NOT be opened!!!" << std::endl;
    return 0;
  }
  std::cout << "------------> Process: " << fname << std::endl;
  return 1;
}

void JadepixDecoder::SetHeaderFooterWord()
{
  Frame_Header = 0xAAAAAAAA;
  Frame_Footer = 0xF0F0F0F0;
}

void JadepixDecoder::InitVariables()
{
  for (int row = 0; row < 48; row++)
  {
    header_id[row] = 0;
    header_event_type[row] = 0;
    header_row_counter[row] = 0;
    header_counter_4bit[row] = 0;
    header_counter_16bit[row] = 0;

    header_id_pre[row] = 0;
    header_event_type_pre[row] = 0;
    header_row_counter_pre[row] = 0;
    header_counter_4bit_pre[row] = 0;
    header_counter_16bit_pre[row] = 0;

    footer_id[row] = 0;
    footer_event_type[row] = 0;
    footer_row_counter[row] = 0;
    footer_counter_4bit[row] = 0;
    footer_memaddr_counter[row] = 0;

    footer_id_pre[row] = 0;
    footer_event_type_pre[row] = 0;
    footer_row_counter_pre[row] = 0;
    footer_counter_4bit_pre[row] = 0;
    footer_memaddr_counter_pre[row] = 0;

    for (int ch = 0; ch < 16; ch++)
    {
      adc[row][ch] = 0;
    }
  }

  header_error_flag = 0;
  data_error_flag = 0;
  footer_error_flag = 0;
}

void JadepixDecoder::SkipTimeStamp()
{
  for (int i = 0; i < 35; i++)
  {
    unsigned char tmp_buf_skip;
    jadepix_rawFile->read((char *)&tmp_buf_skip, sizeof(tmp_buf_skip));

    std::cout << std::dec << "i = " << i << " : ";
    std::cout << std::setw(9) << std::hex << tmp_buf_skip << std::dec << std::endl;
  }
}

// return 0-error 1-find 2-end
int JadepixDecoder::FindFrameHeader()
{
  int flag = 0;
  header_error_flag = 0; // Error flag

  int header_find_flag_frame_header = 0;
  int read_count = 0;
  while (header_find_flag_frame_header == 0)
  {
    unsigned int tmp_buf_header;
    jadepix_rawFile->read((char *)&tmp_buf_header, sizeof(tmp_buf_header));
    read_count++;
    if (jadepix_rawFile->eof())
    {
      std::cout << "End of File" << std::endl;
      return 2;
    }
    if (tmp_buf_header == Frame_Header)
    {
      header_find_flag_frame_header = 1;

      if (read_count > 1)
      {
        flag = 1;
        header_error_flag = 1;
        std::cout << "Header Search :  Read word(=4byte) count = " << read_count << std::endl;
      }
    }
  }

  return flag;
}

// return 0-error 1-find
int JadepixDecoder::FindFrameFooter()
{
  int flag = 0;
  footer_error_flag = 0; // Error flag

  unsigned int tmp_buf_footer;
  jadepix_rawFile->read((char *)&tmp_buf_footer, sizeof(tmp_buf_footer));

  if (tmp_buf_footer != Frame_Footer)
  {
    flag = 1;
    footer_error_flag = 1;
    std::cout << "Event Footer (0xf0f0_f0f0) is not here ! " << std::endl;
  }

  return flag;
}

int JadepixDecoder::DecodePixelData()
{
  int flag = 0; // Error flag

  for (int row = 0; row < 48; row++)
  {
    for (int i = 0; i < 10; i++)
    {
      unsigned int tmp_buf_read;
      jadepix_rawFile->read((char *)&tmp_buf_read, sizeof(tmp_buf_read));
      if (jadepix_rawFile->eof())
      {
        flag = 1;
        std::cout << "End of Data " << std::endl;
        return 2;
      }

      if (i == 0) // row header
      {
        header_id[row] = (tmp_buf_read & 0xf0000000) >> 28;
        header_event_type[row] = (tmp_buf_read & 0x0c000000) >> 26;
        header_row_counter[row] = (tmp_buf_read & 0x03f00000) >> 20;
        header_counter_4bit[row] = (tmp_buf_read & 0x000f0000) >> 16;
        header_counter_16bit[row] = (tmp_buf_read & 0x0000ffff);

        //std::cout << "header_id = " << header_id[row] << std::endl;
        //std::cout << "event_type = " << event_type[row] << std::endl;
        //std::cout << "row_counter = " << row_counter[row] << std::endl;
        //std::cout << "event_counter = " << event_counter[row] << std::endl;
      }

      int ch1, ch2;
      if (i == 1) { ch1 = 0; ch2 = 1; }
      if (i == 2) { ch1 = 2; ch2 = 3; }
      if (i == 3) { ch1 = 4; ch2 = 5; }
      if (i == 4) { ch1 = 6; ch2 = 7; }
      if (i == 5) { ch1 = 8; ch2 = 9; }
      if (i == 6) { ch1 = 10; ch2 = 11; }
      if (i == 7) { ch1 = 12; ch2 = 13; }
      if (i == 8) { ch1 = 14; ch2 = 15; }

      if (i == 9) // row footer
      {
        // Valid before a bug fix
        footer_id[row] = (tmp_buf_read & 0x00f00000) >> 20;
        footer_event_type[row] = (tmp_buf_read & 0x000c0000) >> 18;
        footer_row_counter[row] = (tmp_buf_read & 0x0003f000) >> 12;
        footer_counter_4bit[row] = (tmp_buf_read & 0x00000f00) >> 8;
        footer_memaddr_counter[row] = (tmp_buf_read & 0x0000003f);

        //footer_id[row] =  (tmp_buf_read & 0x00f00000) >> 28;
        //footer_event_type[row] = (tmp_buf_read & 0x000c0000) >> 26;
        //footer_row_counter[row] = (tmp_buf_read & 0x0003f000) >> 20;
        //footer_counter_4bit[row]= (tmp_buf_read & 0x00000f00) >> 16;
        //footer_memaddr_counter[row] = (tmp_buf_read & 0x0000003f);
      }

      if (i > 0 && i < 9) // Only Pixel Data
      {
        adc[row][ch1] = (tmp_buf_read & 0x0000ffff);
        adc[row][ch2] = (tmp_buf_read & 0xffff0000) >> 16;

        // Fill to histogram
        std::stringstream hist_adc1, hist_adc2;
        hist_adc1 << "h_adc" << (ch1 + 1);
        hist_adc2 << "h_adc" << (ch2 + 1);
      }
    }
  } // End of 48*10 readout

  return flag;
}

int JadepixDecoder::CompareTwoFrame()
{
  int flag = 0;
  data_error_flag = 0; // Error flag

  // Data Check
  for (int row = 0; row < 48; row++)
  {
    if (header_id[row] != 15) // row header
    {
      flag = 1;
      data_error_flag = 1;
      //std::cout << "Event num = " << decode_frame_num << " : ";
      //std::cout << "Row Header is not find : row = " << row << std::endl;
    }
    if (footer_id[row] != 14) // row footer
    {
      flag = 1;
      data_error_flag = 1;
      //std::cout << "Event num = " << decode_frame_num << " : ";
      //std::cout << "Row Feader is not find : row = " << row << std::endl;
    }

    if (header_event_type[row] != 2)
    {
      flag = 1;
      data_error_flag = 1;
      //std::cout << "Event num = " << decode_frame_num << " : ";
      //std::cout << "Event type is something wrong : row = " << row << std::endl;
    }
    if (header_row_counter[row] != row)
    {
      flag = 1;
      data_error_flag = 1;
      //std::cout << "Event num = " << decode_frame_num << " : ";
      //std::cout << "Row counter is something wrong : row = " << row << std::endl;
      //std::cout << "Row_counter = " << header_row_counter[row] << std::endl;
      //std::cout << "Row_counter_pre = " << header_row_counter_pre[row] << std::endl;
    }
    if ((header_counter_4bit[row] - header_counter_4bit_pre[row]) != 1 && (header_counter_4bit[row] - header_counter_4bit_pre[row]) != -15)
    {
      flag = 1;
      data_error_flag = 1;
      //std::cout << "Event counter(4bit) is something wrong : row = " << row << std::endl;
      //std::cout << "Event_counter(4bit) = " << header_counter_4bit[row] << std::endl;
      //std::cout << "Event_counter_pre(4bit) = " << header_counter_4bit_pre[row] << std::endl;
    }
    if ((header_counter_16bit[row] - header_counter_16bit_pre[row]) != 1 && (header_counter_16bit[row] - header_counter_16bit_pre[row]) != -65535)
    {
      flag = 1;
      data_error_flag = 1;
      //std::cout << "Event counter(16bit) is something wrong : row = " << row << std::endl;
      //std::cout << "Event_counter(16bit) = " << header_counter_16bit[row] << std::endl;
      //std::cout << "Event_counter_pre(16bit) = " << header_counter_16bit_pre[row] << std::endl;
    }
  }

  // Copy those information
  for (int row = 0; row < 48; row++)
  {
    header_id_pre[row] = header_id[row];
    header_event_type_pre[row] = header_event_type[row];
    header_row_counter_pre[row] = header_row_counter[row];
    header_counter_4bit_pre[row] = header_counter_4bit[row];
    header_counter_16bit_pre[row] = header_counter_16bit[row];
  }

  return 0;
}


int JadepixDecoder::FillFrame(long nFrame)
{
  JadepixFrame *_frame = new JadepixFrame(nFrame);
  
  for (int ch = 0; ch < 16; ch++)
  {
    _frame->SetEventType(header_event_type[7]);
    _frame->SetEventCounter(header_counter_16bit[7]);

    if (header_error_flag != 0 || data_error_flag != 0 || footer_error_flag != 0)
    {
      _frame->SetErrorFlag(1);
    }
    else
    {
      _frame->SetErrorFlag(0);
    }

    for (int row = 0; row < 48; row++)
    {
      JadepixDigi* _digi = new JadepixDigi(row, ch, adc[row][ch]);
      _frame->AddDigi(_digi);
    }
  }
  m_frameVec.push_back(_frame);

  return 0;
}

void JadepixDecoder::ReSet()
{
  for(m_itframe=m_frameVec.begin(); m_itframe!=m_frameVec.end();++m_itframe){
    delete *m_itframe;
  }
  m_frameVec.clear();
}

void JadepixDecoder::InitLoopCounter()
{
   decode_frame_num = 0;
}

int JadepixDecoder::Decode(std::string datafile, int data_structure, int max_decode_frame)
{
    // Set Header & Footer word
    SetHeaderFooterWord();

    // Open Data File
    OpenRawFile(datafile);

    // Init Loop Counters
    InitLoopCounter();

    // Cut TimeStamp saved at the beginning.
    if (data_structure == 1)
    {
        SkipTimeStamp();
    }

    // Init Variables
    InitVariables();

    for (int nFrame = 0; nFrame < max_decode_frame; nFrame++)
    {
        //std::cout << "Frame number = " << nFrame << std::endl;
        SetLoopCounter(nFrame);

        // Frame Header
        int end_of_file = FindFrameHeader();
        if (end_of_file == 2)
            break;

        // Data :  (row_header + data + row_footer)*16
        end_of_file = DecodePixelData();
        if (end_of_file == 2)
            break;
        CompareTwoFrame();

        // Frame Footer
        end_of_file = FindFrameFooter();
        if (end_of_file == 2)
            break;

        // Fill to the struct
        FillFrame(nFrame);
    }
    return 0;
}
