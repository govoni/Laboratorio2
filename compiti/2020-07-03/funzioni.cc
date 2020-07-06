#include "funzioni.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "TH1F.h"
#include "TCanvas.h"
#include "TString.h"

using namespace std ;


double sign (double val)
{
  if (val == 0) return val ; 
  return val / fabs (val) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


double spostaAcaso ()
{
//  return 2. * rand () / static_cast <double> (RAND_MAX) - 1. ; 
  return sign (2. * rand () / static_cast <double> (RAND_MAX) - 1.) ; 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


TH1F * getDistrib (int Nbiglie, int Nsplit)
{

  double limite = Nsplit ;
  TString nome = "h_posizioni_" ;
  nome += Nbiglie ;
  nome += "_" ;
  if (Nsplit < 10) nome += "0" ;
  nome += Nsplit ;
  TH1F * h_posizioni = new TH1F (nome, nome, int (2 * limite + 1), -1. * limite -0.5, limite + 0.5) ;

  for (int i = 0 ; i <Nbiglie ; ++i)
    {
      double posizione = 0. ;

      for (int j = 0 ; j < Nsplit ; ++j)
        {
          posizione += spostaAcaso () ;
        }
      h_posizioni->Fill (posizione) ;
    }

  return h_posizioni ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


double trend (double * x, double * par)
{
  return par[0] * sqrt (x[0]) ;
}
