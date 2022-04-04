#ifndef lib_h
#define lib_h

#include <vector>

double rand_range (double min, double max) ;
double inv_exp (double lambda) ;
double rand_pois (double lambda) ;
double poisson (double n, double lambda) ;
double calcLikelihood (const std::vector<int> & campione, double lambda) ;

#endif