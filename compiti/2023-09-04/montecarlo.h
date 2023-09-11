#ifndef montecarlo_h
#define montecarlo_h

double rand_range (double min, double max) ;

double rand_TAC (double f (double), double min, double max, double ymax) ;

double rand_TCL_gaus (double mean, double sigma, int N = 10) ;

double rand_unif (double mean, double sigma, int N = 1) ;

#endif