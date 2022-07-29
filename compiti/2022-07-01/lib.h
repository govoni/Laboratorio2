#ifndef lib_h
#define lib_h

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
class TH1F ;
class TF1 ;

double fgaus (double x, double mean = 0, double sigma = 1) ;
double rand_range (double min, double max) ;
double rand_TAC (double f (double, double, double), double mean, double sigma) ;
double findAbsMax (std::vector<double> & data_y) ; 
TH1F * riempiHisto (std::vector<double> input, const std::string & histoName, int N_bins, double h_min, double h_max) ;

#endif