#include "montecarlo.h"

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
}

double rand_TAC (TF1 * func, double min, double max, double xmax)
{
  double ymax = func->Eval (xmax) ;
  double x = min ;
  double y = ymax ;
  do
  {
    x = rand_range (min, max) ;
    y = rand_range (0., ymax) ;

  } while (func->Eval (x) < y) ;
  return x ;
}
