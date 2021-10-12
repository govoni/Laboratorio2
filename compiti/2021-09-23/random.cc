#include "random.h"
#include <cmath>

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
  } 

double rand_TCL_gaus (double mean, double sigma, int N) 
  {
    double delta = sqrt (3 * N) * sigma ;
    double y = 0. ; 
    for (int i = 0 ; i < N ; ++i)
      y += rand_range (mean - delta, mean + delta) ;
    y /= N ;
    return y ;
  }
