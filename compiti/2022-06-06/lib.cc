#include "lib.h"
#include <cmath>
#include <string>
#include <fstream>
#include <cstdlib>

#include "TH1F.h"
#include "TF1.h"

#include "statistiche_vector.h"

using namespace std ;

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
  } 

double rand_TAC (double f (double, double, double), double mean, double sigma)
  {
    double x = 0. ;
    double y = 0. ; 
    do {
      x = rand_range (mean - 4 * sigma, mean + 4 * sigma) ;
      y = rand_range (0, 1. / (sigma * 2.50)) ; // 2.50 ~ sqrt (2 * pi))
    } while (y > f (x, mean, sigma)) ;
    return x ; 
  }

double findAbsMax (vector<double> & data_y)
{
  double max = 0. ;
  for (int i = 0 ; i < data_y.size () ; ++i)
    if (fabs (data_y.at (i)) > max) max = fabs (data_y.at (i)) ;
  return max ;
} 

TH1F *
riempiHisto (vector<double> input, const string & histoName, int N_bins, double h_min, double h_max)
  {
    TH1F * h = new TH1F (histoName.c_str (), histoName.c_str (), N_bins, h_min, h_max) ;
    for (int i = 0 ; i < input.size () ; ++i)
      h->Fill (input.at (i)) ;
    return h ;
  }



