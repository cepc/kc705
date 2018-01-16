#include "Wrappixelana.hh"

using namespace std;

Wrappixelana::Wrappixelana(){
    std::cout << "Start Analysis ..." << std::endl;
};

Wrappixelana::~Wrappixelana(){
    std::cout << "End Analysis ..." << std::endl;
}

int Wrappixelana::pixelana(int max_ana_event, int max_ana_file, std::string outfile, const char* infile)
{
   //std::vector<std::string> arg(infile, infile+max_ana_file);
   
   // Use CMOS Class
   CMOS ana;

   // Input argument
   //int max_ana_event      = std::atoi(argv[1]);
   //int max_ana_file          = std::atoi(argv[2]);
   //std::string& outfile      = arg[3];    // output ROOT Filename
   
   std::cout << "Analysis Output Filename = " << outfile << std::endl;

   // Input files
   TChain *chain = new TChain("tree");
   for(int i=0;i<max_ana_file;i++)chain->Add(infile);
   ana.set_tree_data(chain);

   // Configuration
   int number_of_event = chain->GetEntries();
   int number_of_ana_event = (number_of_event < max_ana_event) ? number_of_event : max_ana_event;
   ana.set_max_ana_event(number_of_ana_event);

   // Histrogram Delearation
   TObjArray hlist;        
   make_histogram(&hlist);

   // Initilization 
   ana.init_variables();
   
   // Calculation of Mean & RMS 
   //int start_event_num = 100;
   //int end_event_num = 500;
   int start_event_num = 15000;
   int end_event_num = 20000;

   ana.calc_mean( start_event_num,  end_event_num );
   ana.calc_rms( start_event_num,  end_event_num );

   // ADC distribution
   //ana.fill_adc(start_event_num,  end_event_num );
   ana.fill_adc(100,  number_of_ana_event );

   // Output ROOT file
   TFile *fout = new TFile(outfile.c_str(),"RECREATE");
   hlist.Write();
   fout->Close();
   
   chain->Delete();
   hlist.Delete();


   return 0;

}


void Wrappixelana::make_histogram(TObjArray *hlist)
{
	// Define Histrogram 
	for (int ch = 0; ch<16; ch++)
	{
		for (int row = 0; row<48; row++)
		{
			std::stringstream hist_title_adc_dcs, hist_title_adc_mean;
			hist_title_adc_dcs << "h_adc_dcs_ch" << (ch + 1) << "_row" << (row + 1);
			hist_title_adc_mean << "h_adc_mean_ch" << (ch + 1) << "_row" << (row + 1);

			TH1F* htmp_adc_dcs = new TH1F(hist_title_adc_dcs.str().c_str(), "ADC distribution", 4000, -2000, 2000);
			TH1F* htmp_adc_mean = new TH1F(hist_title_adc_mean.str().c_str(), "ADC distribution", 4000, -2000, 2000);

			hlist->Add(htmp_adc_dcs);
			hlist->Add(htmp_adc_mean);
		}
	}

	for (int ch = 0; ch<16; ch++)
	{
		std::stringstream hist_title_mean, hist_title_rms;
		hist_title_mean << "h" << (ch + 1) * 100 + 1;
		hist_title_rms << "h" << (ch + 1) * 100 + 51;

		TH1F* htmp_mean = new TH1F(hist_title_mean.str().c_str(), "MEAN", 48, 0, 48);
		TH1F* htmp_rms = new TH1F(hist_title_rms.str().c_str(), "RMS", 48, 0, 48);

		htmp_rms->SetMinimum(0);

		hlist->Add(htmp_mean);
		hlist->Add(htmp_rms);
	}

}

