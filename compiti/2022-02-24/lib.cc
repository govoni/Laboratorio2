#include "lib.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>

using namespace std ;

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ; 
}

double inv_exp (double lambda)
{
  return -1. * (log (1 - rand_range (0., 1.)) / lambda) ;
}

double rand_pois (double lambda)
{
  double t_tot = inv_exp (1.) ;
  int N_evt = 0 ;
  while (t_tot < lambda)
    {
      ++N_evt ;
      t_tot += inv_exp (1.) ;
    }
  return N_evt ;
}

int fattoriale (int N)
{
  if (N == 0) return 1 ;
  if (N == 1) return 1 ;
  long int fatt = N ;
  for (int i = N-1 ; i > 1 ; --i) fatt *= i ;
  return fatt ; 
}

double poisson (double n, double lambda)
{
  return pow (lambda, n) * exp (-1 * lambda) / fattoriale  (n) ;
}

double calcLikelihood (const vector<int> & campione, double lambda)
{
  double like = 1. ; 
  for (int i = 0 ; i < campione.size () ; ++i)
    {
      like *= poisson (campione.at (i), lambda) ;
    }
  return like ;
}

