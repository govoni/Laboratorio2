#ifndef funzioni_h
#define funzioni_h

#include <cstdlib>
#include <vector>
#include <string>

double genera_uniforme (const double & min, const double & max) ;

double calc_pi (const int & Npoints) ;

std::vector<double> calc_pi_and_error (const int & Npoints, const int & Nexp) ;

double trend (double * x, double * par) ;

void saveToText (  
  const std::string & filename,
  const std::vector<double> & v_N,
  const std::vector<double> & v_pi,
  const std::vector<double> & v_pi_unc
  ) ;

#endif