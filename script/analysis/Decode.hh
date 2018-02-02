#ifndef  Decode_H
#define Decode_H


class Decode
{

public:
   Decode(){std::cout<<"Data read start!"<<std::endl;}
   
   
   void set_header_footer_word();

   // File Open 
   int open_file(std::string datafile);
   void set_output_filename(std::string filename);

   // ROOT Tree
   void set_tree_pointer(TTree *t){ tree = t; }
   void Define_TTree();
   void write_file(TTree *t);

   int fill_to_tree();       

   // Histogram Operation
   void init_hist();
   void write_hist();

   // Init 
   int set_loop_counter(int frame_number){decode_frame_num = frame_number;}
   void init_loop_counter(); 
   void init_variables();

   // Decode Member Function 
   int find_frame_header();
   int find_frame_footer();
   int decode_pixel_data();
   int compare_two_frame();

   // Misc
   void skip_timestamp();


private:
   // Data Header & Footer
   unsigned long Frame_Header;
   unsigned long Frame_Footer;

   // File Stream
   ifstream fin;
   
   // Output Root Filename
   std::string outfile_name;
   
   // Frame Number
   int decode_frame_num;

   // Error Flag
   int header_error_flag;
   int data_error_flag;
   int footer_error_flag;

   // Container for one frame
   signed short int  adc[48][16];
   
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

   // Tree
   TTree *tree;

   // Structure
   struct cmos_pixel_data { 
      UInt_t   event_type;
      UInt_t   error_flag;
      UInt_t   event_counter;
      Short_t       adc[48];
   }event_tree[16];
  

};

#endif
