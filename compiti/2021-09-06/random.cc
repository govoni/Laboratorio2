#include "random.h"
#include <cmath>

double fgaus (double x, double mean, double sigma) 
  {
    return exp (-0.5 * (x - mean) * (x - mean) / (sigma * sigma)) ; 
  }

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
  } 

double rand_TAC_gaus (double mean, double sigma)
  {
    double x = 0. ;
    double y = 0. ; 
    do {
      x = rand_range (mean - 4 * sigma, mean + 4 * sigma) ;
      y = rand_range (0, fgaus (mean, mean, sigma)) ;
    } while (y > fgaus (x, mean, sigma)) ;
    return x ; 
  }
