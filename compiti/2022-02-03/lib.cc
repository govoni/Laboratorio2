#include "lib.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std ;

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ; 
}

double inv_exp (double lambda)
{
  return -1. * (log (1 - rand_range (0., 1.)) / lambda) ;
}

double gauss_TCL (double mean, double sigma, int N)
{
  double number = 0. ;
  double lambda = 1. / (sqrt (N) * sigma) ;
  for (int i = 0 ; i < N ; ++i) number += inv_exp (lambda) ;
  number /= N ;
  return mean + number - 1. / lambda ;
}

double calcMedia (const vector<double> & campione)
{
  double media = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    media += campione.at (i) ;
  return media / campione.size () ;
}

double calcMediana (vector<double> campione)
{
  sort (campione.begin (), campione.end ()) ;
  if (campione.size () / 2 == 0) 
    return 0.5 * (campione.at (campione.size () / 2 -1) + campione.at (campione.size () / 2)) ;
  return campione.at (campione.size () / 2) ;
}

double calcVarianza (const vector<double> & campione)
{
  double media = 0. ;
  double mediaSq = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    {
      media += campione.at (i) ;
      mediaSq += campione.at (i) * campione.at (i) ;
    }  
  media /= campione.size () ;  
  return campione.size () * (mediaSq / campione.size () - media * media) / (campione.size () - 1) ;
}

double calcSigma (const vector<double> & campione)
{
  return sqrt (calcVarianza (campione)) ;
}

double calcDevStdMedia (const vector<double> & campione)
{
  return sqrt (calcVarianza (campione)/campione.size ()) ;
}

double calcAsimmetria (const vector<double> & campione)
{
  double media = calcMedia (campione) ;
  double sigma = calcSigma (campione)  ;
  double asimmetria = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    {
      asimmetria += pow ((campione.at (i) - media) / sigma, 3) ;
    }  
  asimmetria /= campione.size () ;  
  return asimmetria ;
}

