/*
c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <iostream>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH1F.h"

#include "lib.h"

using namespace std ;

int main (int argc, char ** argv)
{
  double lambda = atof (argv[1]) ;
  TH1F h_pdf ("h_pdf", "h_pdf", 30, 0., 4./lambda) ; 
  for (int i = 0 ; i < 1000 ; ++i)
    {
      h_pdf.Fill (inv_exp (lambda)) ;
    }  
  TCanvas c1 ;
  h_pdf.Draw ("hist") ;
  c1.Print ("expo.png", "png") ;  

  TH1F h_gaus ("h_gaus", "h_gaus", 150, -7., 17.) ; 
  for (int i = 0 ; i < 1000000 ; ++i)
    {
      h_gaus.Fill (gauss_TCL (5., 3., 100)) ;
    }  
  h_gaus.Draw ("hist") ;
  h_gaus.Fit ("gaus") ;  
  c1.Print ("gauss.png", "png") ;  

  TGraphErrors trend_mediana ;
  TGraphErrors trend_asimm ;
  for (int N = 2 ; N < 10000 ; N *=2)
    {
      TString nome = "gauss_" ;
      nome += N ;
      TH1F * h_gaus = new TH1F (nome, nome, 150, -7., 17.) ; 
      vector<double> campione ;
      for (int i = 0 ; i < 100000 ; ++i)
        {
          double num = gauss_TCL (5., 3., N) ; 
          h_gaus->Fill (num) ;
          campione.push_back (num) ;
        }  
      trend_asimm.SetPoint (trend_mediana.GetN (), N, calcAsimmetria (campione)) ;
      trend_mediana.SetPoint (trend_mediana.GetN (), N, calcMedia (campione) - calcMediana (campione)) ;
      trend_mediana.SetPointError (trend_mediana.GetN () - 1, 0., calcDevStdMedia (campione)) ;

      // h_gaus->Draw ("hist") ;
      // h_gaus->Fit ("gaus") ;
      // nome += ".png" ;  
      // c1.Print (nome, "png") ;  
      delete h_gaus ;
    }  
  trend_mediana.Draw ("AL*") ;
  c1.SetLogx () ;
  c1.Print ("mediana.png", "png") ;
  c1.SetLogy () ;
  trend_asimm.Draw ("AL*") ;
  c1.Print ("asimmetria.png", "png") ;

  return 0 ;
}