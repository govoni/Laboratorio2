/*
c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <fstream>
#include <iostream>

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFitResult.h"

#include "lib.h"
#include "statistiche_vector.h"

using namespace std ;

int main (int argc, char ** argv)
{

  if (argc < 2) 
    {
      cerr << "usage " << argv[0] << " coordinate.txt" << endl ;
      return 1 ;
    }

  ifstream f_campione ;
  f_campione.open (argv[1]) ;

  vector<double> x_coord ;
  vector<double> y_coord ;
  while (true)
    {
      double x_temp ;
      double y_temp ;
      f_campione >> x_temp ;
      f_campione >> y_temp ;
      if (f_campione.eof () == true) break ;
      x_coord.push_back (x_temp) ;
      y_coord.push_back (y_temp) ;
    }
  f_campione.close () ;

  TGraph g_traiettoria (x_coord.size (), &x_coord.at (0), &y_coord.at (0)) ;

  TF1 f_traiettoria ("f_traiettoria", "pol2(0)", 0., 1.8) ;
  f_traiettoria.SetLineWidth (1) ;
  f_traiettoria.SetLineStyle (7) ;
  f_traiettoria.SetLineColor (kGreen + 2) ;
  f_traiettoria.FixParameter (0, 0.) ;
  TFitResultPtr fit_result = g_traiettoria.Fit (&f_traiettoria, "S") ;

  // fit_result->PrintCovMatrix (cout) ;
  TMatrixDSym cov = fit_result->GetCovarianceMatrix () ;
  double A      = f_traiettoria.GetParameter (1) ; // linear term
  double var_A  = cov (1, 1) ;
  double B      = f_traiettoria.GetParameter (2) ; // quadratic term
  double var_B  = cov (2, 2) ;
  double cov_AB = cov (1, 2) ;

  double gittata = -1 * A/B ;
  double var_gittata = gittata * gittata
                           * (var_A / (A * A) + var_B / (B * B) + cov_AB / (A * B) ) ;
  double sigma_gittata = sqrt (var_gittata) ;                         

  cout << "gittata : " << gittata 
       << " +- " << sigma_gittata << "\n" ;

  g_traiettoria.SetMarkerStyle (4) ;
  g_traiettoria.SetMarkerSize (1) ;
  g_traiettoria.SetTitle ("") ;
  TCanvas c1 ;
  TH1F * bkg = c1.DrawFrame (0, 0, 1.9, 1.1) ;
  bkg->GetXaxis ()->SetTitle ("x") ;
  bkg->GetYaxis ()->SetTitle ("y") ;
  g_traiettoria.Draw ("P same") ;
  f_traiettoria.Draw ("same") ;
  c1.Print ("coordinate.png", "png") ;

  double x_max = sezioneAurea (f_traiettoria, 0., gittata, 0.0001) ;
  double y_max = f_traiettoria.Eval (x_max) ;
  cout << "coordinate del massimo: (" << x_max << ", " << y_max << ")\n" ;

  return 0 ;
}