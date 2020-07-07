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
#include "TH1F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TFitResult.h"

using namespace std ;

int main (int argc, char ** argv)
{

  if (argc < 2)
    {
      cerr << "utilizzo: " << argv[0] << " N_split [N_biglie]" << endl ;
      return 1 ;
    } 

  int Nsplit = atoi (argv[1]) ;
  int Nbiglie = 100 ;

  if (argc > 2)
    {
      Nbiglie = atoi (argv[2]) ;
    } 

  srand (time (NULL)) ;

  vector<TH1F *> histos ;

  int N = 0 ;
  while (N++ < Nsplit) 
    {
      histos.push_back (getDistrib (Nbiglie, N)) ;
    }

  TCanvas c1 ; 
  c1.SetLogy () ;

  TGraph g_Variance ;
  for (int i = 0 ; i < histos.size () ; ++i) 
    {
      histos.at (i)->SetFillColor (kOrange) ;
      TFitResultPtr fitResult = histos.at (i)->Fit ("gaus") ;
      histos.at (i)->Draw ("hist") ;
      histos.at (i)->GetFunction ("gaus")->Draw ("same") ;

      g_Variance.SetPoint (
          g_Variance.GetN (),
          i + 1,
          histos.at (i)->GetRMS () * histos.at (i)->GetRMS () 
        ) ;

      TString fileName = histos.at (i)->GetName () ;
      fileName += ".png" ;
      c1.Print (fileName, "png") ;
    }

  c1.SetLogy (0) ;
  g_Variance.SetMarkerStyle (24) ;
  g_Variance.SetMarkerColor (kGreen + 2) ;
  g_Variance.GetHistogram ()->GetXaxis ()->SetTitle ("N_split") ;
  g_Variance.Draw ("APL") ;
  TF1 fitfunc ("fitfunc", trend, 0, Nsplit, 1) ;
  g_Variance.Fit (&fitfunc) ;
  c1.Update () ;
  c1.Print ("Variance.png", "png") ;

  for (int i = 0 ; i < histos.size () ; ++i) delete histos.at (i) ;

  return 0 ;
}

