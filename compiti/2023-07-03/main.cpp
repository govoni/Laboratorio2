/*
c++ -o main main.cpp montecarlo.cc utils.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <iostream>

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

double f (double x)
{
  return pow (x-2, 3) + 3 ;
}

Double_t root_f (Double_t *x, Double_t *par)
{
   return pow (x[0]-par[0], 3) + par[1] ;
}

double g (double x)
{
  return (x-2) * (x-2) + 3 ;
}

Double_t root_g (Double_t *x, Double_t *par)
{
   return pow (x[0]-par[0], 2) + par[1] ;
}

// scale histogram so that its maximum height is 1
double scale_histo (TH1F & histo)
{
  double altezza = histo.GetBinContent (histo.GetMaximumBin ()) ;
  histo.Scale (1. / altezza) ;
  return altezza ;
}
 
int generateToys (double rand_gen (double, double, int), 
                  TH1F & h_q2, TH1F & h_pv, TF1 & fitting_f, 
                  double yError, int nToys, 
                  TString imageFileName = "test.gif") 
{
  vector<double> v_x = {0.5, 1.5, 2.5, 3.5} ;
  vector<double> v_ex (v_x.size (), 0.) ;
  vector<double> v_ey (v_x.size (), yError) ;

  int fails = 0 ;
  for (int iToy = 0 ; iToy < nToys ; ++iToy)
    {
      vector<double> v_y (v_x.size (), 0.) ;
      for (int iPoint = 0 ; iPoint < v_x.size () ; ++iPoint)
        {
          double epsilon = rand_gen (0, yError, 10) ;
          v_y.at (iPoint) = f (v_x.at (iPoint)) + epsilon ;
        }
      TGraphErrors graph (v_x.size (), &v_x.at (0), &v_y.at (0), &v_ex.at (0), &v_ey.at (0)) ;
      TFitResultPtr fit_result = graph.Fit (fitting_f.GetName (), "SQ") ;
      if (!fit_result->IsValid ())
        {
          ++fails ;
          continue ;
        }
      if (iToy == 0)
        {
          TCanvas c1 ;
          graph.Draw ("A*E") ;
          c1.Print (imageFileName, "gif") ;
        }
      h_q2.Fill (fit_result->Chi2 ()) ;
      h_pv.Fill (fit_result->Prob ()) ;
    }
  if (fails > 0) cout << "WARNING: " << fails << " fits did not succeed\n" ;
  return fails ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  // no stats box drawing
  gStyle->SetOptStat (0) ;

  if (argc < 2) 
    {
      cerr << "usage " << argv[0] << " numero_toy" << endl ;
      return 1 ;
    }

  int nToys = atoi (argv[1]) ;
  double yError = 0.2 ;
//  double yError = 2. ;
  
/*
 - genera 4 punti secondo la funzione, in 0.5, 1.5, 2.5, 3.5
 - aggiungi epsilon gaussiano uniforme con sigma 0.2 utilizzando una funzione gaus TCL con sigma ed N come input
*/

  vector<double> v_x = {0.5, 1.5, 2.5, 3.5} ;
  vector<double> v_ex (v_x.size (), 0.) ;

  vector<double> v_y (v_x.size (), 0.) ;
  vector<double> v_ey (v_x.size (), yError) ;
  for (int iPoint = 0 ; iPoint < v_x.size () ; ++iPoint)
    {
      double epsilon = rand_TCL_gaus (0, yError, 10) ;
      v_y.at (iPoint) = f (v_x.at (iPoint)) + epsilon ;
    }

/*
 - trasferisci in TGraph
 - fai fit, controlla successo, calcola Q2, calcola p-value
*/

  TGraphErrors graph (v_x.size (), &v_x.at (0), &v_y.at (0), &v_ex.at (0), &v_ey.at (0)) ;
  TF1 fit_f ("fit_f", root_f, 0., 5., 2) ;
  TFitResultPtr fit_result = graph.Fit ("fit_f", "S") ;
  cout << "successo del fit: " << fit_result->IsValid () << endl ;
  cout << "Valore di Q2: " << fit_result->Chi2 () << endl ;
  cout << "probabilità associata a Q2 (p-value): " << fit_result->Prob () << endl ; 
  
  TCanvas c1 ;
  graph.Draw ("A*E") ;
  c1.Print ("first_fit.gif", "gif") ;

/*
 - costruisci distribuzione di Q2
 - costruisci distribuzione di p-value - che forma ci si aspetta che abbia?
*/

  TH1F h_q2 ("h_q2", "h_q2", 100, 0., 10.) ;
  TH1F h_pv ("h_pv", "h_pv", 100, 0., 1.) ;

  generateToys (rand_TCL_gaus, h_q2, h_pv, fit_f, yError, nToys, "one_fit_f.gif") ;

  scale_histo (h_q2) ;
  h_q2.Draw ("hist") ;
  c1.Print ("q2.gif", "gif") ;

  c1.DrawFrame (0., 0., 1., 1.1 * h_pv.GetMaximum ()) ;
  h_pv.Draw ("hist same") ;
  c1.Print ("pValue.gif", "gif") ;

/*
 - costrusici distribuzione di Q2 con il fit con epsilon uniforme
*/

  TH1F h_q2_unif ("h_q2_unif", "h_q2_unif", 100, 0., 10.) ;
  TH1F h_pv_unif ("h_pv_unif", "h_pv_unif", 100, 0., 1.) ;

  generateToys (rand_unif, h_q2_unif, h_pv_unif, fit_f, yError, nToys, "one_fit_unif.gif") ;

  scale_histo (h_q2_unif) ;
  h_q2_unif.SetLineColor (kOrange + 2) ;
  h_q2_unif.Draw ("hist") ;
  h_q2.Draw ("hist same") ;

  TLegend * legend = new TLegend (0.55, 0.7, 0.9, 0.9) ;
  legend->AddEntry(&h_q2, "Gaussian errors", "l");
  legend->AddEntry(&h_q2_unif, "uniform errors", "l");
  legend->Draw();

  c1.Print ("q2_unif.gif", "gif") ;

/*
 - costrusici distribuzione di Q2 con il fit con la funzione sbagliata (g)
*/

  TH1F h_q2_g ("h_q2_g", "h_q2_g", 300, 0., 300.) ;
  TH1F h_pv_g ("h_pv_g", "h_pv_g", 100, 0., 1.) ;
  TF1 fit_g ("fit_g", root_g, 0., 5., 2) ;

  generateToys (rand_TCL_gaus, h_q2_g, h_pv_g, fit_g, yError, nToys, "one_fit_g.gif") ;

  h_q2_g.SetLineColor (kRed) ;
  scale_histo (h_q2_g) ;
  legend->AddEntry(&h_q2_g, "wrong function", "l");

  h_q2_g.Draw ("hist") ;
  h_q2_unif.Draw ("hist same") ;
  h_q2.Draw ("hist same") ;
  legend->Draw () ;
  c1.Print ("q2_g.gif", "gif") ;

/* 
 - determine the distance between goodness-of-fit tests
   as a function of the measurement uncertainty
*/

  int loop = 0 ;
  for (double yError = 0.2 ; yError < 9 ; yError *= 2, ++loop)
    {

      // fit with the right function

      TH1F h_bis_q2 ("h_bis_q2", "h_bis_q2", 100, 0., 10.) ;
      TH1F h_bis_pv ("h_bis_pv", "h_bis_pv", 100, 0., 1.) ;
    
      generateToys (rand_TCL_gaus, h_bis_q2, h_bis_pv, fit_f, yError, nToys,
                    TString ("one_fit_f_") + loop + ".gif") ;
    
      // fit with the wrong function
    
      TH1F h_bis_q2_g ("h_bis_q2_g", "h_bis_q2_g", 300, 0., 300.) ;
      TH1F h_bis_pv_g ("h_bis_pv_g", "h_bis_pv_g", 100, 0., 1.) ;
    
      generateToys (rand_TCL_gaus, h_bis_q2_g, h_bis_pv_g, fit_g, yError, nToys, 
                    TString ("one_fit_g_") + loop + ".gif") ;

      h_bis_q2_g.SetLineColor (kRed) ;
      scale_histo (h_bis_q2) ;
      scale_histo (h_bis_q2_g) ;
       
      h_bis_q2_g.Draw ("hist") ;
      h_bis_q2.Draw ("hist same") ;
      TString filename = TString ("difference_") + loop + ".gif" ;
      c1.Print (filename, "gif") ;
    }

  return 0 ;
}