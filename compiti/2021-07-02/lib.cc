#include "lib.h"

#include <cmath>


double f_sin (double x)
  {
    return 0.5 * sin (x) ;
  }

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
  } 

double rand_TAC (double f (double), double xMin, double xMax, double yMax)
  {
    double x = 0. ;
    double y = 0. ; 
    do {
      x = rand_range (xMin, xMax) ;
      y = rand_range (0, yMax) ;
    } while (y > f (x)) ;
    return x ; 
  }

int fillHisto (TH1F * histo, std::vector<double> campione)
  {
    for (int i = 0 ; i < campione.size () ; ++i)
      histo->Fill (campione.at (i)) ;
    return histo->GetEntries () ;
  }
