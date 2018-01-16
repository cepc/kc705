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
//#include "define_hist.hh"   // All of histograms are defined in this file.


#ifndef WRAPPIXEL_HH
#define WRAPPIXEL_HH

class Wrappixelana{
public:
Wrappixelana();
~Wrappixelana();

int pixelana(int max_ana_event, int max_ana_file, std::string outfile, const char* infile);
void make_histogram(TObjArray *hlist);

private:
};

#endif