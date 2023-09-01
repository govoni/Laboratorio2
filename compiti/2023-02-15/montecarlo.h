#ifndef montecarlo_h
#define montecarlo_h

double rand_range (double min, double max) ;

double rand_TAC (double f (double), double min, double max, double ymax) ;

double parabola (double x) ;

double rand_TCL_unif (int N = 10) ;

double rand_TCL_para (int N = 10) ;

#endif