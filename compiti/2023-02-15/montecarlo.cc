#include "montecarlo.h"
#include <cstdlib>

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

// defined positive from 1 to 3, with maximum in 2
double parabola (double x)
{
  return -1 * (x-2) * (x-2) + 1 ;
}

double rand_TCL_unif (int N)
{
  double tot = 0 ;
  for (int i = 0 ; i < N ; ++i) tot += rand_range (1., 3.) ;
  return tot / N ;
}

double rand_TCL_para (int N)
{
  double tot = 0 ;
  for (int i = 0 ; i < N ; ++i) tot += rand_TAC (parabola, 1., 3., 1.) ;
  return tot / N ;
}