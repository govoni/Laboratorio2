#ifndef lib_h
#define lib_h

#include "TH1F.h"

double f_sin (double x) ;
double rand_range (double min, double max) ;
double rand_TAC (double f (double), double xMin, double xMax, double yMax) ;
int fillHisto (TH1F * histo, std::vector<double> campione) ;


#endif