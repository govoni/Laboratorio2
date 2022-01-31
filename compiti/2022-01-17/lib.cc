#include "lib.h"
#include <cstdlib>

using namespace std ;

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ; 

}

double generaTAC (double min, double max) 
{
  double inv_d = 1. / (max - min) ;
  double x = rand_range (min, max) ;
  double y = rand_range (0., 2. * inv_d) ;
  while (y > 2 * inv_d * inv_d * (x - min))
    {
      x = rand_range (min, max) ;
      y = rand_range (0., 2. * inv_d) ;
    }
  return x ;
}

double calcMedia (const vector<double> & campione)
{
  double media = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    media += campione.at (i) ;
  return media / campione.size () ;
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