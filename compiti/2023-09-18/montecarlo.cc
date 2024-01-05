#include "montecarlo.h"
#include <cstdlib>
#include <cmath>

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ----


double rand_TAC_cauchy (double M, double Gamma, double generation_width)
{
  double y_max = f_cauchy (M, M, Gamma) ; 
  double x = M ;
  double y = 0. ;
  do {
    x = rand_range (M - generation_width, M + generation_width) ;
    y = rand_range (0, y_max) ;
  }
  while (y > f_cauchy (x, M, Gamma)) ;
  return x ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ----


double rand_TCL_cauchy (double M, double Gamma, double generation_width, int N)
{
  double x = 0. ;
  for (int i = 0 ; i < N ; ++i)
    {
      x += rand_TAC_cauchy (M, Gamma, generation_width) ;
    }
  return x / static_cast<double> (N) ; 
} 

