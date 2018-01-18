#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TProfile.h"

#include "Decode.hh"



void Decode::set_header_footer_word()
{
   Frame_Header = 0xAAAAAAAA;
   Frame_Footer = 0xF0F0F0F0;
}

void Decode::Define_TTree()
{
   std::string leaf_format = "event_type/i:error_flag/i:event_counter/i:adc[48]/S" ;

   for(int ch=0;ch<16;ch++)     
   {
      std::stringstream branch_title;
      branch_title << "adc_ch" << (ch+1) ;
      tree->Branch(branch_title.str().c_str(),&event_tree[ch],leaf_format.c_str());
   }
   
}

void Decode::skip_timestamp()
{
   for(int i=0;i<35;i++)
   {
      unsigned char tmp_buf_skip;     
      fin.read((char *)&tmp_buf_skip,sizeof(tmp_buf_skip));  

      std::cout << std::dec << "i = " << i << " : " ;
      std::cout << std::setw(8) << std::hex << tmp_buf_skip << std::dec << std::endl;      
   }
}

void Decode::init_variables()
{
   for(int row=0;row<48; row++)
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

      for(int ch=0;ch<16;ch++)     
      {    
         adc[row][ch] = 0;
      }
   }
      
      header_error_flag = 0;
      data_error_flag = 0;
      footer_error_flag = 0;
}       

int Decode::find_frame_header()
{
   int flag=0;  
   header_error_flag = 0;  // Error flag 

   int header_find_flag_frame_header=0;
   int read_count = 0;
   while(header_find_flag_frame_header==0)
   {
      unsigned int tmp_buf_header;
      fin.read((char *)&tmp_buf_header,sizeof(tmp_buf_header));
      read_count++;
      if(fin.eof())
	{
         write_file(tree);
         std::cout << "End of File" << std::endl;
         return 2;
	}
      if(tmp_buf_header==Frame_Header)
	{
	  header_find_flag_frame_header = 1;

          if( read_count > 1 ) 
          {
             flag = 1;
             header_error_flag = 1;
             std::cout << "Header Search :  Read word(=4byte) count = " << read_count << std::endl; 
          }
	}
   }
   
   return  flag;
}

int Decode::find_frame_footer()
{
   int flag=0;  
   footer_error_flag = 0; // Error flag

   unsigned int tmp_buf_footer;
   fin.read((char *)&tmp_buf_footer,sizeof(tmp_buf_footer));

   if( tmp_buf_footer != Frame_Footer )
   {
      flag = 1;
      footer_error_flag = 1;
      std::cout << "Event Footer (0xf0f0_f0f0) is not here ! " << std::endl;
   }

   return flag;
}

int Decode::decode_pixel_data()
{
   int flag=0;  // Error flag

   for(int row=0; row<48; row++)
   {
      for(int i=0;i<10;i++)
      {
         unsigned int tmp_buf_read;
         fin.read((char *)&tmp_buf_read,sizeof(tmp_buf_read));  
         if(fin.eof())
         {
            flag = 1;
            std::cout << "End of Data " << std::endl;
            return 2;
         }

         if( i==0 )  // row header
         {
            header_id[row] =  (tmp_buf_read & 0xf0000000) >> 28;
            header_event_type[row] = (tmp_buf_read & 0x0c000000) >> 26;
            header_row_counter[row] = (tmp_buf_read & 0x03f00000) >> 20;
            header_counter_4bit[row]= (tmp_buf_read & 0x000f0000) >> 16;
            header_counter_16bit[row] = (tmp_buf_read & 0x0000ffff);

            //std::cout << "header_id = " << header_id[row] << std::endl;
            //std::cout << "event_type = " << event_type[row] << std::endl;
            //std::cout << "row_counter = " << row_counter[row] << std::endl;
            //std::cout << "event_counter = " << event_counter[row] << std::endl;
         }
 
         int ch1, ch2;
         if( i == 1 ){ ch1 = 0;  ch2 = 1; }
         if( i == 2 ){ ch1 = 2;  ch2 = 3; }
         if( i == 3 ){ ch1 = 4;  ch2 = 5; }
         if( i == 4 ){ ch1 = 6;  ch2 = 7; }
         if( i == 5 ){ ch1 = 8;  ch2 = 9; }
         if( i == 6 ){ ch1 = 10; ch2 = 11; }
         if( i == 7 ){ ch1 = 12; ch2 = 13; }
         if( i == 8 ){ ch1 = 14; ch2 = 15; }
        
         if( i==9 )  // row footer
         {
            // Valid before a bug fix
            footer_id[row] =  (tmp_buf_read & 0x00f00000) >> 20;
            footer_event_type[row] = (tmp_buf_read & 0x000c0000) >> 18;
            footer_row_counter[row] = (tmp_buf_read & 0x0003f000) >> 12;
            footer_counter_4bit[row]= (tmp_buf_read & 0x00000f00) >> 8;
            footer_memaddr_counter[row] = (tmp_buf_read & 0x0000003f);

            //footer_id[row] =  (tmp_buf_read & 0x00f00000) >> 28;
            //footer_event_type[row] = (tmp_buf_read & 0x000c0000) >> 26;
            //footer_row_counter[row] = (tmp_buf_read & 0x0003f000) >> 20;
            //footer_counter_4bit[row]= (tmp_buf_read & 0x00000f00) >> 16;
            //footer_memaddr_counter[row] = (tmp_buf_read & 0x0000003f);
         }

         if( i>0 && i<9)  // Only Pixel Data
         {            
            adc[row][ch1] = (tmp_buf_read & 0x0000ffff) ;
            adc[row][ch2] = (tmp_buf_read & 0xffff0000) >> 16;

            // Fill to histogram
            std::stringstream hist_adc1, hist_adc2;
            hist_adc1 << "h_adc" << (ch1+1) ;
            hist_adc2 << "h_adc" << (ch2+1) ;
            TH1I* h_adc1 = dynamic_cast<TH1I*>(gDirectory->Get(hist_adc1.str().c_str()));
            TH1I* h_adc2 = dynamic_cast<TH1I*>(gDirectory->Get(hist_adc2.str().c_str()));
            h_adc1->Fill(row+decode_frame_num*48, adc[row][ch1]);
            h_adc2->Fill(row+decode_frame_num*48, adc[row][ch2]);
         }
      }
   }  // End of 48*10 readout

   return flag;
}

int Decode::compare_two_frame()
{
   int flag=0;  
   data_error_flag = 0;  // Error flag

   // Data Check
   for(int row=0;row<48;row++)
   {
      if( header_id[row] != 15 ) // row header
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << decode_frame_num << " : " ;
         std::cout << "Row Header is not find : row = " << row << std::endl;
      }
      if( footer_id[row] != 14 ) // row footer
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << decode_frame_num << " : " ;
         std::cout << "Row Feader is not find : row = " << row << std::endl;
      }

      if( header_event_type[row] != 2 )
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << decode_frame_num << " : " ;      
         std::cout << "Event type is something wrong : row = " << row << std::endl;
      }
      if( header_row_counter[row] != row )
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << decode_frame_num << " : " ;
         std::cout << "Row counter is something wrong : row = " << row << std::endl;
         std::cout << "Row_counter = " << header_row_counter[row] << std::endl;
         std::cout << "Row_counter_pre = " << header_row_counter_pre[row] << std::endl;
      }
      if( (header_counter_4bit[row] - header_counter_4bit_pre[row]) != 1 && (header_counter_4bit[row] - header_counter_4bit_pre[row]) != -15)
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event counter(4bit) is something wrong : row = " << row << std::endl;
         std::cout << "Event_counter(4bit) = " << header_counter_4bit[row] << std::endl;
         std::cout << "Event_counter_pre(4bit) = " << header_counter_4bit_pre[row] << std::endl;
      }
      if( (header_counter_16bit[row] - header_counter_16bit_pre[row]) != 1 && (header_counter_16bit[row] - header_counter_16bit_pre[row]) != -65535)
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event counter(16bit) is something wrong : row = " << row << std::endl;
         std::cout << "Event_counter(16bit) = " << header_counter_16bit[row] << std::endl;
         std::cout << "Event_counter_pre(16bit) = " << header_counter_16bit_pre[row] << std::endl;
      }
   }

   // Copy those information 
   for(int row=0;row<48;row++)
   {
      header_id_pre[row] = header_id[row];
      header_event_type_pre[row] = header_event_type[row];
      header_row_counter_pre[row] = header_row_counter[row];
      header_counter_4bit_pre[row] = header_counter_4bit[row];
      header_counter_16bit_pre[row] = header_counter_16bit[row];
   }

   return 0;
}


int Decode::fill_to_tree()
{

   for( int ch=0;ch<16;ch++)
   {
      event_tree[ch].event_type = header_event_type[7];
      event_tree[ch].event_counter = header_counter_16bit[7];
      
      if( header_error_flag != 0 || data_error_flag != 0 || footer_error_flag != 0 ){
         event_tree[ch].error_flag = 1;
      }
      else{
         event_tree[ch].error_flag = 0;   
      }

      for(int row=0;row<48;row++)     
      {    
         event_tree[ch].adc[row] = adc[row][ch];
      }
   }
   

   tree->Fill();

   return 0;
}


int Decode::open_file(std::string datafile)
{
   fin.open(datafile.c_str(),std::ios::in|std::ios::binary);
   if(!fin.is_open())
   {
      std::cout << "File open error!" << std::endl;
      return 0;
   }
   
   return 1;
}

void Decode::set_output_filename(std::string filename)
{
   outfile_name = filename;
   TFile *ff = new TFile(outfile_name.c_str(),"RECREATE");
   ff->Close();
}

void Decode::write_file(TTree *tree)
{
   fin.close();  
   write_hist();
   TFile *f = new TFile(outfile_name.c_str(),"UPDATE");
   tree->Write(); 
   f->Close();
   std::cout << "Total Event number = " <<  decode_frame_num << std::endl;
}


void Decode::init_loop_counter()
{
   decode_frame_num = 0;
}




//########################   Histogram Operation    ##############################
void Decode::init_hist()
{  
   // Define Histrogram 
   for(int i=0;i<16;i++)
   {
      std::stringstream hist_adc;
      hist_adc << "h_adc" << (i+1) ;
      TH1I *h_adc  = new TH1I(hist_adc.str().c_str(),"ADC",240000,0,240000); // 48row * 5000frame = 240000
   }
}


void Decode::write_hist()
{  
   for(int i=0;i<16;i++)
   {
      std::stringstream hist_adc;
      hist_adc << "h_adc" << (i+1) ;
      TH1I *h_adc  =  dynamic_cast<TH1I*>(gDirectory->Get(hist_adc.str().c_str()));

      TFile *f = new TFile(outfile_name.c_str(),"UPDATE");
      h_adc->Write();
      f->Close();
   }

}
