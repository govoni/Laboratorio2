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

using namespace std ;

int main (int argc, char ** argv)
{
  double mean = 0. ; 
  double sigma = 1. ;

  vector<double> entries ;
  vector<double> mean_LS ;  // least squares
  vector<double> sigma_LS ;
  vector<double> mean_ML ;  // maximum likelihood
  vector<double> sigma_ML ;

  double max_mean ;
  double min_mean ;
  double max_sigma ;
  double min_sigma ;

  TCanvas c1 ;

  for (int n_max = 4 ; n_max < 100 ; n_max *= 2)
    {
      entries.push_back (n_max) ;
      vector<double> eventi ;
      for (int i = 0 ; i < n_max ; ++i) eventi.push_back (rand_TAC (fgaus, mean, sigma)) ;
      TH1F * histo = riempiHisto (eventi, "histo", 50, -3. * sigma, 3. * sigma) ;

      TFitResultPtr fr_LS = histo->Fit ("gaus", "SQ") ;
      mean_LS.push_back (histo->GetFunction ("gaus")->GetParameter (1) - mean) ;
      sigma_LS.push_back (histo->GetFunction ("gaus")->GetParameter (2) - sigma) ;
      histo->Draw () ;
      c1.Print (TString ("histo_") + n_max + TString (".png"), "png") ;

      TFitResultPtr fr_ML = histo->Fit ("gaus", "LSQ") ;
      mean_ML.push_back (histo->GetFunction ("gaus")->GetParameter (1) - mean) ;
      sigma_ML.push_back (histo->GetFunction ("gaus")->GetParameter (2) - sigma) ;
      delete histo ;

      if (mean_LS.back () < min_mean) min_mean = mean_LS.back () ;
      if (mean_ML.back () < min_mean) min_mean = mean_ML.back () ;
      if (sigma_LS.back () < min_sigma) min_sigma = sigma_LS.back () ;
      if (sigma_ML.back () < min_sigma) min_sigma = sigma_ML.back () ;
      if (mean_LS.back () > max_mean) max_mean = mean_LS.back () ;
      if (mean_ML.back () > max_mean) max_mean = mean_ML.back () ;
      if (sigma_LS.back () > max_sigma) max_sigma = sigma_LS.back () ;
      if (sigma_ML.back () > max_sigma) max_sigma = sigma_ML.back () ;
    }

  for (int i = 0 ; i < mean_LS.size () ; ++i)
    {
      cout << i << "\t"
           << entries.at (i) << "\t"
           << mean_LS.at (i) << "\t"
           << mean_ML.at (i) << "\n" ;

    }

  TGraph g_mean_LS (mean_LS.size (), &entries[0], &mean_LS[0]) ;
  TGraph g_mean_ML (mean_ML.size (), &entries[0], &mean_ML[0]) ;

  c1.DrawFrame (entries.at (0) - 1, min_mean, entries.back () + 1, max_mean) ;
  g_mean_LS.SetLineColor (kBlue) ;
  g_mean_LS.Draw ("L*") ;
  g_mean_ML.SetLineColor (kRed) ;
  g_mean_ML.Draw ("L*") ;
  c1.Print ("trend_mean.png") ;

  TGraph g_sigma_LS (sigma_LS.size (), &entries[0], &sigma_LS[0]) ;
  TGraph g_sigma_ML (sigma_ML.size (), &entries[0], &sigma_ML[0]) ;

  c1.DrawFrame (entries.at (0) - 1, min_sigma, entries.back () + 1, max_sigma) ;

  g_sigma_LS.SetLineColor (kBlue) ;
  g_sigma_LS.Draw ("L*") ;
  g_sigma_ML.SetLineColor (kRed) ;
  g_sigma_ML.Draw ("L*") ;
  c1.Print ("trend_sigma.png") ;



  return 0 ;
}