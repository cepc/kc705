#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TProfile.h"

#include "CMOS.hh"

//----------------------------------    For General Routine     --------------------------------------- 


CMOS::CMOS()
{
   std::cout<<"Analysis start!"<<std::endl;
}

void CMOS::set_tree_data(TTree *t)
{
   Data_Tree = t;
}

void CMOS::set_max_ana_event(int n)
{
   Max_ana_Event = n;
   std::cout << "Max event = " << Max_ana_Event << std::endl;
}

void CMOS::init_variables()
{
   for(int row=0;row<48;row++)
   {
      for(int ch=0;ch<16;ch++)
      {
         Calib_MEAN_ADC[row][ch] = 0.0;
         Calib_RMS_ADC[row][ch] = 0.0;

         pre_adc[row][ch] = 0;
      }
   }

}


int CMOS::calc_mean(int event_number_start, int event_number_end)
{
   int total_event = event_number_end - event_number_start;

   for(int event=event_number_start; event<event_number_end; event++)
   {      
      Data_Tree->GetEntry(event);	
      for(int ch=0;ch<16;ch++)     
      {
         std::stringstream branch_title;
         branch_title << "adc_ch" << (ch+1) ;

         TBranch *br = Data_Tree->GetBranch(branch_title.str().c_str());   
         for(int row=0;row<48;row++)     
         {    
            int adc = br->GetLeaf("adc")->GetValue(row);
            Calib_MEAN_ADC[row][ch] += (double)adc;
         }
      }       
   }

   for(int ch=0;ch<16;ch++)
   {
      std::stringstream hist_title_mean;
      hist_title_mean     << "h" << (ch+1)*100+1;
      TH1F* htmp_mean        = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_mean.str().c_str()));

     for(int row=0;row<48;row++)
      {
         Calib_MEAN_ADC[row][ch] /= total_event;
         htmp_mean->Fill( row, Calib_MEAN_ADC[row][ch] );
      }
   }

   return 0;
}

   
int CMOS::calc_rms(int event_number_start, int event_number_end)
{
   int total_event = event_number_end - event_number_start;
   
   for(int event=event_number_start; event<event_number_end; event++)
   {      
      Data_Tree->GetEntry(event);	
      for(int ch=0;ch<16;ch++)     
      {
         std::stringstream branch_title;
         branch_title << "adc_ch" << (ch+1) ;

         TBranch *br = Data_Tree->GetBranch(branch_title.str().c_str());   
         for(int row=0;row<48;row++)     
         {    
            int adc = br->GetLeaf("adc")->GetValue(row);
            double diff = Calib_MEAN_ADC[row][ch] - (double)adc;
            Calib_RMS_ADC[row][ch] += diff*diff;
         }
      }       
   }

   for(int ch=0;ch<16;ch++)
   {
      std::stringstream hist_title_rms;
      hist_title_rms        << "h" << (ch+1)*100+51;
      TH1F* htmp_rms           = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_rms.str().c_str()));

     for(int row=0;row<48;row++)
      {
         Calib_RMS_ADC[row][ch]        = sqrt( Calib_RMS_ADC[row][ch]/(double)(total_event) );
         htmp_rms->Fill( row, Calib_RMS_ADC[row][ch] );
      }
   }

   return 0;
}




int CMOS::fill_adc(int event_number_start, int event_number_end)
{   
   for(int event=event_number_start; event<event_number_end; event++)
   {      
      Data_Tree->GetEntry(event);	
      for(int ch=0;ch<16;ch++)     // All of 16 channels 
      //for(int ch=6;ch<7;ch++)     // only channel 7
      {
         std::stringstream branch_title;
         branch_title << "adc_ch" << (ch+1) ;

         TBranch *br = Data_Tree->GetBranch(branch_title.str().c_str());   
         for(int row=0;row<48;row++)     
         {    
            int adc = br->GetLeaf("adc")->GetValue(row);

            double diff_DCS = (double)adc  - (double)pre_adc[row][ch];
            double diff_Mean = (double)adc - Calib_RMS_ADC[row][ch]; 

            //std::cout << "DCS = " << diff_DCS << ", Mean = " << diff_Mean << std::endl;
            pre_adc[row][ch] = adc;
            if( event == event_number_start)diff_DCS=0.0; // First event.

            std::stringstream hist_title_adc_dcs, hist_title_adc_mean;
            hist_title_adc_dcs     << "h_adc_dcs_ch" << (ch+1) << "_row" << (row+1) ;
            hist_title_adc_mean     << "h_adc_mean_ch" << (ch+1) << "_row" << (row+1) ;
            TH1F* htmp_adc_dcs        = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_dcs.str().c_str()));            
            TH1F* htmp_adc_mean        = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_mean.str().c_str()));            

            htmp_adc_dcs->Fill(diff_DCS);
            htmp_adc_mean->Fill(diff_Mean);
         }
      }       
   }
   

   return 0;
}
