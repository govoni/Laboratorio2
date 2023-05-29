#ifndef montecarlo_h
#define montecarlo_h

#include "TF1.h"

double rand_range (double min, double max) ;
double rand_TAC (TF1 * func, double min, double max, double xmax) ;

#endif