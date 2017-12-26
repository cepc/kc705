#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TProfile.h"
#include "TChain.h"

#include "CMOS.hh"
#include "define_hist.hh"   // All of histograms are defined in this file.

using namespace std;

int main(int argc, char *argv[])
{
   std::vector<std::string> arg(argv,argv+argc);
   
   // Use CMOS Class
   CMOS ana;

   // Input argument
   int max_ana_event      = std::atoi(argv[1]);
   int max_ana_file          = std::atoi(argv[2]);
   std::string& outfile      = arg[3];    // output ROOT Filename
   
   std::cout << "Analysis Output Filename = " << outfile.c_str() << std::endl;

   // Input files
   TChain *chain = new TChain("tree");
   for(int i=0;i<max_ana_file;i++)chain->Add(arg[i+4].c_str());
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
