#include "montecarlo.h"
#include <cstdlib>
#include <cmath>

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
}

double rand_TAC (double f (double), double min, double max, double ymax)
{
  double x = min ;
  double y = ymax ;
  do
  {
    x = rand_range (min, max) ;
    y = rand_range (0., ymax) ;

  } while (f (x) < y) ;
  return x ;
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

double rand_unif (double mean, double sigma, int N)
  {
    double delta = 1.732 * sigma ; // sqrt (3) = 1.732
    return rand_range (mean - delta, mean + delta) ;
  } 
