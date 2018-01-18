void make_histogram(TObjArray *hlist)
{
   // Define Histrogram 
   for(int ch=0;ch<16;ch++)    
   {
      for(int row=0;row<48;row++)     
      {
         std::stringstream hist_title_adc_dcs, hist_title_adc_mean;
         hist_title_adc_dcs     << "h_adc_dcs_ch" << (ch+1) << "_row" << (row+1) ;
         hist_title_adc_mean     << "h_adc_mean_ch" << (ch+1) << "_row" << (row+1) ;

         TH1F* htmp_adc_dcs        = new TH1F(hist_title_adc_dcs.str().c_str(), "ADC distribution", 4000, -2000, 2000);           
         TH1F* htmp_adc_mean        = new TH1F(hist_title_adc_mean.str().c_str(), "ADC distribution", 4000, -2000, 2000);          

         hlist->Add(htmp_adc_dcs);
         hlist->Add(htmp_adc_mean);
      }
   }

   for(int ch=0;ch<16;ch++)
   {
      std::stringstream hist_title_mean, hist_title_rms;
      hist_title_mean     << "h" << (ch+1)*100+1;
      hist_title_rms        << "h" << (ch+1)*100+51;
 
      TH1F* htmp_mean        = new TH1F(hist_title_mean.str().c_str(), "MEAN", 48, 0, 48);
      TH1F* htmp_rms           = new TH1F(hist_title_rms.str().c_str(), "RMS", 48, 0, 48);

      htmp_rms->SetMinimum(0);
 
      hlist->Add(htmp_mean);
      hlist->Add(htmp_rms);
   }

}      
