#ifndef montecarlo_h
#define montecarlo_h

#include "utils.h"

double rand_range (double min, double max) ;
double rand_TAC_cauchy (double M, double Gamma, double generation_width) ;
double rand_TCL_cauchy (double M, double Gamma, double generation_width, int N = 10) ; 

#endif