#include "lib.h"
#include <cmath>
#include <string>
#include <fstream>
#include <cstdlib>

#include "TH1F.h"
#include "TF1.h"

#include "statistiche_vector.h"

using namespace std ;


double f_sin (double * x, double * p)
{
  return p[0] * sin (p[1] * x[0] + p[2]) ;
}


int leggiFile (char * filename, vector<double> & data_x, vector<double> & data_y)
{
  ifstream input_file (filename) ;
  double input_val_x ;
  double input_val_y ;
  while (true)
    {
      input_file >> input_val_x ;
      input_file >> input_val_y ;
      if (input_file.eof () == true) break ;
      data_x.push_back (input_val_x) ;
      data_y.push_back (input_val_y) ;
    }
  input_file.close () ;
  return data_x.size () ;
}


double findAbsMax (vector<double> & data_y)
{
  double max = 0. ;
  for (int i = 0 ; i < data_y.size () ; ++i)
    if (fabs (data_y.at (i)) > max) max = fabs (data_y.at (i)) ;
  return max ;
} 


TH1F *
calcolaScarti (TF1 & tf1_sin, vector<double> const & data_x, vector<double> const & data_y)
  {
    vector<double> scarti ; 
    for (int i = 0 ; i < data_x.size () ; ++i)
      scarti.push_back (tf1_sin.Eval (data_x.at (i)) - data_y.at (i)) ;
    return riempiHisto (scarti, "scarti") ;
  }


TH1F *
riempiHisto (vector<double> input, const string & histoName)
  {
    double var = varianza (input) ;
    double extreme = findAbsMax (input) ;
    int nbins = ceil (1.5 * extreme / var) ;
    TH1F * h = new TH1F (histoName.c_str (), histoName.c_str (), nbins, -1.5 * extreme, 1.5 * extreme) ;
    for (int i = 0 ; i < input.size () ; ++i)
      h->Fill (input.at (i)) ;
    return h ;
  }



