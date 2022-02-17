#ifndef lib_h
#define lib_h

#include <vector>

double rand_range (double min, double max) ;
double inv_exp (double lambda) ;
double gauss_TCL (double mean, double sigma, int N = 20) ;

double calcMedia (const std::vector<double> & campione) ;
double calcMediana (std::vector<double> campione) ;
double calcVarianza (const std::vector<double> & campione) ;
double calcSigma (const std::vector<double> & campione) ;
double calcDevStdMedia (const std::vector<double> & campione) ;
double calcAsimmetria (const std::vector<double> & campione) ;

#endif