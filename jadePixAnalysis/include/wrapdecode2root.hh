#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

#include "Decode.hh"

#ifndef WRAPDECODE2ROOT_H
#define WRAPDECODE2ROOT_H

class Wrapdecode2root {

public:
  Wrapdecode2root();
  ~Wrapdecode2root();

  void decode2root(std::string, std::string, int, int);

private:

};

#endif