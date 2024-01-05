/*
c++ -o main main.cpp montecarlo.cc utils.cc `root-config --glibs --cflags`
*/

/*

La distribuzione di Cauchy
--------------------------

*/


#include <vector>
#include <iostream>
#include <cmath>

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TString.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

#include "statistiche_vector.h"
#include "montecarlo.h"
#include "utils.h"

using namespace std ;


int main (int argc, char ** argv)
{

  if (argc < 4)
    {
      cout << "uso del programma: "
           << argv[0]
           << " M Gamma numero_eventi\n" ;
      exit (1) ;     
    }
  float M = atof (argv[1]) ;
  float Gamma = atof (argv[2]) ;
  int N_events = atoi (argv[3]) ;


  vector<double> v_cauchy ;
  TH1F h_cauchy ("h_cauchy", "h_cauchy", sturges (N_events), M - 3 * Gamma, M + 3 * Gamma) ;
  for (int i_event = 0 ; i_event < N_events ; ++ i_event)
    {
      double x = rand_TAC_cauchy (M, Gamma, 3 * Gamma) ;
      v_cauchy.push_back (x) ;
      h_cauchy.Fill (x) ;
    }
  
  TCanvas c1 ;
  h_cauchy.SetFillColor (kOrange) ;
  h_cauchy.Draw ("hist") ;
  c1.Print ("plot_cauchy.png", "png") ;

  cout << "media della distribuzione: " << media (v_cauchy) << "\n" ;
  cout << "sigma della distribuzione: " << sigma (v_cauchy) << "\n" ;

  vector<double> v_xWidth ;
  vector<double> v_media ; 
  vector<double> v_sigma ; 
  for (int i_gamma = 1 ; i_gamma <= 100 ; ++i_gamma)
    {
      vector<double> v_cauchy_loc ;
      for (int i_event = 0 ; i_event < N_events ; ++ i_event)
        {
          double x = rand_TAC_cauchy (M, Gamma, i_gamma * Gamma) ;
          v_cauchy_loc.push_back (x) ;
        }
      v_media.push_back (media (v_cauchy_loc)) ;
      v_sigma.push_back (sigma (v_cauchy_loc)) ;
      v_xWidth.push_back (i_gamma) ;
    }

  TGraph g_media (v_xWidth.size (), &v_xWidth.at (0), &v_media.at (0)) ;
  TGraph g_sigma (v_xWidth.size (), &v_xWidth.at (0), &v_sigma.at (0)) ;

  g_media.Draw ("AL*") ;
  c1.Print ("plot_media.png", "png") ;
  g_sigma.Draw ("AL*") ;
  c1.Print ("plot_sigma.png", "png") ;

  vector<double> v_TCL ;
  TH1F h_TCL ("h_TCL", "h_TCL", sturges (N_events), M - 3 * Gamma, M + 3 * Gamma) ;
  for (int i_event = 0 ; i_event < N_events ; ++ i_event)
    {
      double x = rand_TCL_cauchy (M, Gamma, 3 * Gamma) ;
      v_TCL.push_back (x) ;
      h_TCL.Fill (x) ;
    }

  TF1 segnale ("segnale", "gaus(0)", M - 3 * Gamma, M + 3 * Gamma) ;
  segnale.SetParameter (0, v_TCL.size ()) ;
  segnale.SetParameter (1, media (v_TCL)) ;
  segnale.SetParameter (0, sigma (v_TCL)) ;
  TFitResultPtr fit_result = h_TCL.Fit ("segnale", "S") ;
  cout << "goodness of fit test: p-value = " << fit_result->Prob () << "\n" ;

  h_TCL.SetFillColor (kOrange) ;
  h_TCL.Draw ("hist") ;
  segnale.Draw ("same") ;
  c1.Print ("plot_TCL.png", "png") ;
  

  return 0 ;
}
