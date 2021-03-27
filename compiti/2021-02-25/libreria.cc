#include "libreria.h"

#include <cstdlib>

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
} 

double fgaus0 (double x, double sigma) 
{
  return exp (-0.5 * x * x / (sigma * sigma)) ; 
}

double rand_TAC_gaus0 (double sigma)
{
  double x = 0. ;
  double y = 0. ; 
  double yMax = fgaus0 (0., sigma) ;
  do {
  x = rand_range (-4. * sigma, 4. * sigma) ;
    y = rand_range (0, yMax) ;
  } while (y > fgaus0 (x, sigma)) ;
  return x ; 
}

double get_max_abs (const std::vector<double> & v)
{
  double max_abs = 0. ;
  for (int i = 0 ; i < v.size () ; ++i)
    if (fabs (v.at (i)) > max_abs) max_abs = fabs (v.at (i)) ;
  return max_abs ;
}







