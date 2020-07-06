#ifndef funzioni_h
#define funzioni_h

#include <cstdlib>
#include <vector>
#include <string>

#include "TH1F.h"
 
double spostaAcaso () ;

TH1F * getDistrib (int Nbiglie, int Nsplit) ;

TH1F * getDistribNorm (int Nbiglie, int Nsplit) ;

double trend (double * x, double * par) ;

#endif