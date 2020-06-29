#include "funzioni.h"

#include <cmath>
#include <iostream>
#include <fstream>

#include "TH1F.h"
#include "TCanvas.h"

using namespace std ;

double genera_uniforme (const double & min, const double & max)
{
  return min + (max - min) * rand () / static_cast <double> (RAND_MAX) ; 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


double calc_pi (const int & Npoints)
{
  int Ninside = 0 ;
  for (int i = 0 ; i < Npoints ; ++i)
    {
       double x = genera_uniforme (0., 1.) ;
       double y = genera_uniforme (0., 1.) ;
       if ( (x*x + y*y) < 1) ++Ninside ;
    }
  return 4. * static_cast<double> (Ninside) / static_cast<double> (Npoints) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


vector<double> calc_pi_and_error (const int & Npoints, const int & Nexp)
{
  double var = sqrt (4 * 3.14 * (1-3.14/4.)/Npoints) ;
  TH1F * h_pi = new TH1F ("h_pi", "h_pi", 20, 3.14 - 5 * var, 3.14 + 5 * var) ;
  double sum = 0. ;
  double sumsq = 0. ;
  for (int i = 0 ; i < Nexp ; ++i)
    {
       double pi_val = calc_pi (Npoints) ;
       h_pi->Fill (pi_val) ;
       sum += pi_val ;
       sumsq += pi_val * pi_val ;
    }
  vector<double> result ;
  double media = sum / Nexp ;
  result.push_back (media) ;
  result.push_back (sqrt (sumsq / Nexp - media * media)) ;
  cout << "--> " << result.at (1) << " " << h_pi->GetRMS () << endl ;

  TCanvas * c2 = new TCanvas ; 
  h_pi->Draw ("hist") ;
  c2->Print (TString ("dist_") + Npoints + TString (".png"), "png") ;

  delete c2 ;
  delete h_pi ;
  return result ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


double trend (double * x, double * par)
{
  return par[0] / sqrt (x[0]) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void saveToText (  
  const string & filename,
  const vector<double> & v_N,
  const vector<double> & v_pi,
  const vector<double> & v_pi_unc
  )
{
  ofstream outFile (filename.c_str ()) ;
  for (int i = 0 ; i < v_N.size () ; ++i)
    outFile << v_N.at (i) << "\t"
            << v_pi.at (i) << "\t"
            << v_pi_unc.at (i) << "\n" ;
  outFile.close () ;
  return ;
}

