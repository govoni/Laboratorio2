#ifndef utils_h
#define utils_h

#include "TH1F.h"
#include <vector>

int fill_histo (TH1F & histo, std::vector<double> & eventi) ;
int conta_in (std::vector<double> & eventi, double min, double max) ;

#endif
