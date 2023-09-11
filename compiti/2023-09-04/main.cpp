/*
c++ -o main main.cpp montecarlo.cc utils.cc `root-config --glibs --cflags`
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

#include "montecarlo.h"

using namespace std ;

double f (double x)
{
  double A = 2 ;
  double B = 0.5 ;
  double C = 0.78 ;
  double D = 0.8 ;
  return A * sin (B * x + C) + D ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


Double_t root_f (Double_t *x, Double_t *par)
{
   return par[0] * sin (par[1] * x[0] + par[2]) + par[3] ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// scale histogram so that its maximum height is 1
double scale_histo (TH1F & histo)
{
  double altezza = histo.GetBinContent (histo.GetMaximumBin ()) ;
  histo.Scale (1. / altezza) ;
  return altezza ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int find_max_element (const vector<double> & container)
{
  int iMax = 0 ;
  for (int i = 0 ; i < container.size () ; ++i)
    if (container[i] > container[iMax]) iMax = i ;
  return iMax ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int find_min_element (const vector<double> & container)
{
  int iMin = 0 ;
  for (int i = 0 ; i < container.size () ; ++i)
    if (container[i] < container[iMin]) iMin = i ;
  return iMin ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int sturges (int N) 
  {
    return floor (1 + 3.322 * log (N)) ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TH1F * build_histo (const TString & histo_name, const vector<double> & sample)
{
  double min = floor (*min_element (sample.begin (), sample.end ())) ;
  double max = ceil (*max_element (sample.begin (), sample.end ())) ;
  int bins = sturges (sample.size ()) ;
  TH1F * histo = new TH1F (histo_name, histo_name, bins, min, max) ;
  for (int i = 0 ; i < sample.size () ; ++i) histo->Fill (sample.at (i)) ;

  return histo ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  // no stats box drawing
  gStyle->SetOptStat (0) ;

  if (argc < 2) 
    {
      cerr << "usage " << argv[0] << " gauss_sigma" << endl ;
      return 1 ;
    }

  double yError = atof (argv[1]) ;
  
/*
 - generate events according to f(x) = A * sin (B * x + C) + D with Gaussian errors
*/

//  vector<double> v_x = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5} ;
  vector<double> v_x = {0.5, 2.5, 4.5, 6.5, 8.5, 10.5} ;
  vector<double> v_ex (v_x.size (), 0.) ;

  vector<double> v_y (v_x.size (), 0.) ;
  vector<double> v_ey (v_x.size (), yError) ;

  cout << " --> v_ey : " ;
  for (int i = 0 ; i < v_ey.size () ; ++i) cout << v_ey.at (i) << " " ;
  cout << "\n" ;

  for (int iPoint = 0 ; iPoint < v_x.size () ; ++iPoint)
    {
      double epsilon = rand_TCL_gaus (0, yError, 10) ;
      v_y.at (iPoint) = f (v_x.at (iPoint)) + epsilon ;
      cout << " -> " << iPoint
           << "f(" << v_x.at (iPoint) << ") = "
           << f (v_x.at (iPoint))
           << " + " << epsilon << "\n" ;
    }

/*
 - trasferisci in TGraph
 - fai fit, controlla successo, calcola Q2, calcola p-value
*/

  double half_lambda = fabs (v_x[find_max_element (v_y)] - v_x[find_min_element (v_y)]) ;
  double vertical_offset = 0.5 * (v_y[find_max_element (v_y)] + v_y[find_min_element (v_y)]) ;
  double amplitude = 0.5 * (v_y[find_max_element (v_y)] - v_y[find_min_element (v_y)]) ;

  cout << "amplitude      :\t" << amplitude << "\n" ;
  cout << "half_lambda    :\t" << half_lambda << "\n" ;
  cout << "vertical_offset:\t" << vertical_offset << "\n" ;

  TGraphErrors graph (v_x.size (), &v_x.at (0), &v_y.at (0), &v_ex.at (0), &v_ey.at (0)) ;
  TF1 fit_f ("fit_f", root_f, 0., 15., 4) ;
  fit_f.SetParameter (0, amplitude) ;
  // fit_f.SetParameter (1, 3.14 / half_lambda) ;
  // fit_f.SetParameter (3, vertical_offset) ;

  TFitResultPtr fit_result = graph.Fit ("fit_f", "S+") ;
  cout << "successo del fit: " << fit_result->IsValid () << endl ;
  cout << "Valore di Q2: " << fit_result->Chi2 () << endl ;
  cout << "probabilità associata a Q2 (p-value): " << fit_result->Prob () << endl ; 
  
  TCanvas c1 ;
  graph.Draw ("A*E") ;
  c1.Print ("first_fit.gif", "gif") ;

/*
 - add a systematic uncertainty to the y points
 - deduce the amount of statistics based on the Q^2 value
*/

  double syst = 1.5 * yError ;
  double syst_actual_value = rand_TCL_gaus (0, syst, 10) ;
  cout << "syst actual value: " << syst_actual_value << "\n" ;
  for (int i = 0 ; i < v_ey.size () ; ++i)
    { 
      v_ey.at (i) = sqrt (v_ey.at (i) * v_ey.at (i) + syst * syst) ;
      v_y.at (i) = v_y.at (i) + syst_actual_value ; 
    }
  cout << " --> v_ey : " ;
  for (int i = 0 ; i < v_ey.size () ; ++i) cout << v_ey.at (i) << " " ;
  cout << "\n" ;

  TGraphErrors graph_syst (v_x.size (), &v_x.at (0), &v_y.at (0), &v_ex.at (0), &v_ey.at (0)) ;
  amplitude = 0.5 * (v_y[find_max_element (v_y)] - v_y[find_min_element (v_y)]) ;
  fit_f.SetParameter (0, amplitude) ;
  TFitResultPtr fit_result_syst = graph_syst.Fit ("fit_f", "S") ;
  cout << "successo del fit: " << fit_result_syst->IsValid () << endl ;
  cout << "Valore di Q2: " << fit_result_syst->Chi2 () << endl ;
  cout << "probabilità associata a Q2 (p-value): " << fit_result_syst->Prob () << endl ;

  double scale_factor = sqrt (fit_result_syst->Chi2 () / fit_result_syst->Ndf ()) ;
  cout << "stima dell'incertezza statistica: " 
       << scale_factor * v_ey.at (0) << "\n" ;

  graph_syst.SetLineColor (kRed) ;
  graph_syst.Draw ("A*E") ;
  c1.Print ("second_fit.gif", "gif") ;

/*
 - verifica con toy experiment
*/

  int nToys = 10000 ;
  vector<double> v_q2 ;
  for (int iToy = 0 ; iToy < nToys ; ++iToy)
    {
      for (int iPoint = 0 ; iPoint < v_x.size () ; ++iPoint)
        {
          double epsilon = rand_TCL_gaus (0, yError, 10) ;
          v_y.at (iPoint) = f (v_x.at (iPoint)) + epsilon + syst_actual_value ;
        }
      TGraphErrors graph_toy (v_x.size (), &v_x.at (0), &v_y.at (0), &v_ex.at (0), &v_ey.at (0)) ;
      amplitude = 0.5 * (v_y[find_max_element (v_y)] - v_y[find_min_element (v_y)]) ;
      fit_f.SetParameter (0, amplitude) ;
      TFitResultPtr fit_result_toy = graph_toy.Fit ("fit_f", "SQ") ;
      v_q2.push_back (fit_result_toy->Chi2 ()) ;
    }

  TH1F * h_q2 = build_histo ("h_q2", v_q2) ;
  cout << "q2 medio: " << h_q2->GetMean () << "\n" ;
  scale_factor = sqrt (h_q2->GetMean () / fit_result_syst->Ndf ()) ;
  cout << "stima media dell'incertezza statistica: " 
       << scale_factor * v_ey.at (0) << "\n" ;

  h_q2->Draw ("hist") ;
  c1.Print ("q2_toys.gif", "gif") ;  

  delete h_q2 ;

  return 0 ;
}  
