/*
c++ -o main `root-config --cflags --glibs` funzioni.cc main.cpp
*/

#include "funzioni.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <math.h>

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"

using namespace std ;

int main (int argc, char ** argv)
{

  if (argc < 4)
    {
      cerr << "utilizzo: " << argv[0] << " N_minimo N_massimo N_esperimenti" << endl ;
      return 1 ;
    } 

  int Nmin = atoi (argv[1]) ;
  int Nmax = atoi (argv[2]) ;
  int Nexp = atoi (argv[3]) ;

  vector<double> v_pi ;
  vector<double> v_pi_unc ;
  vector<double> v_N ;
 
  srand (time (NULL)) ;
  for (int N = Nmin ; N <= Nmax ; N *= 2)
    {
      cout << "testing " << N << " events" << endl ;
      vector<double> pi_and_error = calc_pi_and_error (N, Nexp) ; 

      v_pi.push_back (pi_and_error.at (0)) ;
      v_pi_unc.push_back (pi_and_error.at (1)) ;
      v_N.push_back (N) ;
    }

  vector<double> v_N_unc (v_N.size (), 0.) ;

  TGraphErrors pi_trend (v_N.size (), &v_N.at (0), &v_pi.at (0), &v_N_unc.at (0), &v_pi_unc.at (0)) ;
  TGraph unc_trend (v_N.size (), &v_N.at (0), &v_pi_unc.at (0)) ;

  TCanvas c1 ; 
  c1.SetLogx () ;
  pi_trend.SetMarkerStyle (4) ;
  pi_trend.SetMarkerColor (kBlue) ;
  pi_trend.Draw ("AP") ;
  c1.Print ("result_value.png", "png") ;
 
  unc_trend.SetMarkerStyle (4) ;
  unc_trend.SetMarkerColor (kBlue) ;
  unc_trend.Draw ("AP") ;
  TF1 fitfunc ("fitfunc", trend, Nmin, Nmax, 1) ;
  unc_trend.Fit (&fitfunc) ;
  c1.Print ("result_unc.png", "png") ;

  cout << fitfunc.GetParameter (0) << " +- " << fitfunc.GetParError (0) << endl ;
  cout << "valore atteso: "
  cout << sqrt (4 * M_PI - M_PI * M_PI) << endl ; 

  saveToText ("results.txt", v_N, v_pi, v_pi_unc) ;

  return 0 ;
}

