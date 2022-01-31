#ifndef lib_h
#define lib_h

#include <vector>

double rand_range (double min, double max) ;
double generaTAC (double min = 0., double max = 3.) ;
double calcMedia (const std::vector<double> & campione) ;
double calcVarianza (const std::vector<double> & campione) ;

#endif