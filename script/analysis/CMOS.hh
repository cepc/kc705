#ifndef CMOS_H
#define CMOS_H

class CMOS
{
public:
   CMOS();
   
   void set_tree_data(TTree *t);
   void set_max_ana_event(int nevent);

   void init_variables();

   int calc_mean(int start_event, int end_event);
   int calc_rms(int start_event, int end_event);

   int fill_adc(int start_event, int end_event);   

private:
   TTree *Data_Tree;
    
   int Max_ana_Event;

   // Store previous ADC value
   int pre_adc[48][16];

   // Store Mean & RMS for each 48row*16column
   double Calib_MEAN_ADC[48][16];
   double Calib_RMS_ADC[48][16];

};

#endif
