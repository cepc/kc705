
// #include <sys/types>
// #include <dirent>
// #include <errno>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>   
#include <string>   
#include <cmath>
#include <iomanip>
#include <cstdlib>


#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "Rtypes.h"
#include "Riostream.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"


int process_events_add(string filename="D:\\code\\kc705\\test\\dat\\TestFile20180105-000", string outfile_name="tmp.root") {
  
  // pre-defined header & trailer
  unsigned long m_Frame_Header  = 0xAAAAAAAA;
  unsigned long m_Frame_Trailer = 0xF0F0F0F0;
    

  // pixel array 48x16
  const int NROW        = 48;
  const int NCOL        =  8;  //* 8*2 
  const int BUFFER_SIZE =  5;   // process 100 frames, segmented into 5*20
  const int NBUFFER     = 200;
  
  const int SKIP_FRAMES = 100;

  const int PIX_CLUS_TH  = 200;
  const int PIX_SEED_TH  = 500;

  int size_of_frame = (NROW * (NCOL+2)+2) * sizeof(unsigned int);  // frame size
  int size_of_row   = (NCOL +2) *  sizeof(unsigned int);  // row size
  

  // 1000 frames
  short int raw_proc_frames_ADC[1000][NROW]     = {0};    // noise and pedestal
  short int cds_proc_frames_ADC[1000][NROW]     = {0};    // noise and pedestal
  unsigned short int last_frame_ADC[NROW] ={0}; // for CDS


  // histograms
  TFile *outfile = new TFile(outfile_name.c_str(),"RECREATE");

  TH1F* hist_raw_pix_val[NROW];
  TH1F* hist_base_pix_val[NROW];
  TH1F* hist_proc_pix_val[NROW];
  TH1F* hist_proc_clus_val[NROW];  // seed pixel
  TH1F* hist_proc_seed_val[NROW];  // 3 pixels
  for(unsigned int ipx =0; ipx<NROW; ipx++) {

    char hist_name[100];

    sprintf(hist_name, "%s%d","raw_pixel_val_",ipx);
    hist_raw_pix_val[ipx] = new TH1F(hist_name, "ADC",   20000, -10000, 10000);

    sprintf(hist_name, "%s%d","base_pixel_val_",ipx);
    hist_base_pix_val[ipx] = new TH1F(hist_name, "ADC",  2000, -1000, 1000);

    sprintf(hist_name, "%s%d","proc_pixel_val_",ipx);
    hist_proc_pix_val[ipx] = new TH1F(hist_name, "ADC",  5000,     0, 5000);

    sprintf(hist_name, "%s%d","proc_cluster_val_",ipx);
    hist_proc_clus_val[ipx] = new TH1F(hist_name, "ADC", 5000,     0, 5000);

    sprintf(hist_name, "%s%d","proc_seed_val_",ipx);
    hist_proc_seed_val[ipx] = new TH1F(hist_name, "ADC", 5000,     0, 5000);

  }



  
  // open the file
  ifstream file;
  file.open(filename.c_str(), std::ios::in|std::ios::binary);
  if(!file.is_open()) {
    std::cout << "File open error!" << std::endl;
    return 0;
  }
  

  file.seekg(0,std::ios::end); 
  streampos sp=file.tellg();  // unreliable
 
  // std::cout<<"Total Frames: "<< sp/size_of_frame << std::endl;
  long total_frames = sp/size_of_frame;
  
  file.clear();
  // file.seekg(-3*size_of_frame, std::ios::end);
  file.seekg(SKIP_FRAMES*size_of_frame, std::ios::beg);  // skip the first 100 frames


  const int max_frames  =  -1;
  int valid_frames      =  0;  
  

  for(unsigned int n_frame= 0; n_frame!=max_frames; n_frame++) {

    bool eof_flag = false;
    bool find_frame_header = false;
    while(!find_frame_header) {

      if(file.eof())  { eof_flag == true; break;  }// reached the end of file
      
      unsigned int tmp_buf_header;
      file.read((char *)&tmp_buf_header,sizeof(tmp_buf_header));
     
      if(tmp_buf_header==m_Frame_Header) { // 
	find_frame_header = true;
      }
    }


    // std::cout << hex << showbase << uppercase << tmp_buf_header << std::endl;    
    if(eof_flag) break;  // 


    signed short int  adc[NROW] = {};   // pixel data ADC
    
    for(unsigned int i =0; i<NROW; i++) {  //
    
      unsigned int tmp_buf_read;          
      file.seekg(4*sizeof(tmp_buf_read),std::ios::cur);     // move to Channel 6
      
      file.read((char*)&tmp_buf_read, sizeof(tmp_buf_read));
      adc[i] = (tmp_buf_read & 0x0000ffff) ;  // odd **
      // adc[i] = (tmp_buf_read & 0xffff0000) >> 16; // even
      if(file.eof())  { eof_flag == true; break;  }// reached the end of file

      file.seekg(5*sizeof(tmp_buf_read),std::ios::cur);   // move to the row end

    }

    if(eof_flag) break;  // 

    
    // read the frame trailer
    unsigned int tmp_buf_trailer;
    file.read((char *)&tmp_buf_trailer,sizeof(tmp_buf_trailer));
    
    if(file.eof())  { eof_flag == true; break;  }// reached the end of file

    // std::cout << hex << showbase << uppercase << tmp_buf_trailer << std::endl;    
    if( tmp_buf_trailer != m_Frame_Trailer )
      {
	std::cout << "Event Trailer (0xf0f0_f0f0) not found ! " << std::endl;
	continue; // break;
      }

    
    // process valid frames
    short int cds_frame_ADC[NROW] = {0};
    
    valid_frames++;
    if(valid_frames%1000 ==0)
      std::cout << "Processing frames: " << valid_frames << "/" << total_frames<< std::endl;
        
    //  reserve the first valid frame 
    if(valid_frames ==1) {
      for(unsigned int i=0; i<NROW; i++) {
	last_frame_ADC[i] = adc[i];  
      }
      continue;
    }

    
    // CDS subtraction
    for(unsigned int i=0; i<NROW; i++) {
      cds_frame_ADC[i]  = adc[i] - last_frame_ADC[i];  // CDS
      //   std::cout << setw(4) << TMath::Abs(cds_frame_ADC[i]) ;
      last_frame_ADC[i] = adc[i];  // updating
    }    
    // std::cout << endl;
    
    
    
    // save the first 1000 + (1) CDS frames for pedestal and noise estimation   
    if (valid_frames < BUFFER_SIZE * NBUFFER +2) {
      for(unsigned int i=0; i<NROW; i++) {
	cds_proc_frames_ADC[valid_frames-2][i] = cds_frame_ADC[i];
	raw_proc_frames_ADC[valid_frames-2][i] = adc[i];

      }    
      
      // fill up the histogram at the end of 1000 +(1) frames
      if( valid_frames  ==  (BUFFER_SIZE * NBUFFER + 1)) { 
	
	// calculate the pedestal and noise
	for(unsigned int p=0; p< NBUFFER; p++) {
	  
	  short int tmp_5frames_cds[NROW] = {};
	  
	  for(unsigned int pk=0; pk< BUFFER_SIZE; pk++) {
	    for(unsigned int i=0; i<NROW; i++) {
	      tmp_5frames_cds[i] +=  cds_proc_frames_ADC[p*BUFFER_SIZE+pk][i];
	    }
	  }  
	  
	  
	  for(unsigned int pk=0; pk< BUFFER_SIZE; pk++) {
	    for(unsigned int i=0; i<NROW; i++) {
	      
	      if(TMath::Abs(cds_proc_frames_ADC[p*BUFFER_SIZE+pk][i]) > 200) {  // physical signal suppressed
		short int tmp_val = cds_proc_frames_ADC[p*BUFFER_SIZE+pk][i];
		cds_proc_frames_ADC[p*BUFFER_SIZE+pk][i] = (tmp_5frames_cds[i] - tmp_val)/4;  
	      }
	      
	      hist_base_pix_val[i]->Fill(cds_proc_frames_ADC[p*BUFFER_SIZE+pk][i]);  // read out channel 6
	      // hist_raw_pix_val[i]->Fill(raw_proc_frames_ADC[p*BUFFER_SIZE+pk][i]);  // read out channel 6
	      
	    }
	  }
	}
	
      }
      
      // continue;

    }else{ 


      for(unsigned int i=0; i<NROW; i++) { 
	
	hist_proc_pix_val[i]->Fill(TMath::Abs(cds_frame_ADC[i])); 
	hist_raw_pix_val[i]->Fill(adc[i]); 


	// find the seed
	if((i==0) || (i==NROW-1)) continue;
	
	if((TMath::Abs(cds_frame_ADC[i]) < PIX_SEED_TH) )  continue;
	
	else {
	  
	  if((TMath::Abs(cds_frame_ADC[i]) > TMath::Abs(cds_frame_ADC[i-1])) && (TMath::Abs(cds_frame_ADC[i]) > TMath::Abs(cds_frame_ADC[i+1]))) {
	    
	    hist_proc_seed_val[i]->Fill(TMath::Abs(cds_frame_ADC[i]));  
	    
	    int tmp_clus_val = TMath::Abs(cds_frame_ADC[i]);
	    if(TMath::Abs(cds_frame_ADC[i-1]) > PIX_CLUS_TH)   tmp_clus_val +=TMath::Abs(cds_frame_ADC[i-1]);
	    if(TMath::Abs(cds_frame_ADC[i+1]) > PIX_CLUS_TH)   tmp_clus_val +=TMath::Abs(cds_frame_ADC[i+1]);

	    hist_proc_clus_val[i]->Fill(tmp_clus_val);  
	    
	  }
	  
	}
	
      }
      
    }
  }
  
  
  
  outfile->Write();
  outfile->Close();

  
  file.close();  // close the file
  return 0;
}








