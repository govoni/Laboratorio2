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

  if (argc < 3) 
    {
      cerr << "usage " << argv[0] << " x_0 N_toys" << endl ;
      return 1 ;
    }

  double xZero = atof (argv[1]) ;
  double NToys = atof (argv[2]) ;

  func f (xZero) ;
  vector<double> integrale = area_TAC (f, xZero, 10000) ;
  cout << "controllo normalizzazione: " << integrale[0] << " +- " << integrale[1] << "\n";

  TH1F hToys ("hToys", "hToys", NToys/100 + 1, -1 * xZero, xZero) ;
  vector<double> vToys ;
  for (int iToy = 0 ; iToy < NToys ; ++iToy)
    {
      double x = rand_TAC (f, xZero) ;
      vToys.push_back (x) ;
      hToys.Fill (x) ;
    }

  TCanvas c1 ;
  hToys.SetFillColor (kOrange + 1) ;
  hToys.GetXaxis ()->SetTitle ("x") ;
  hToys.GetYaxis ()->SetTitle ("conteggi per bin") ;
  hToys.Draw ("hist") ;
  c1.Print ("distribuzione.png", "png") ;

  double sigmaToys = sigma (vToys) ;
  cout << "media dei punti : " << media (vToys)  
       << " +- " << sigmaToys / sqrt (static_cast <double> (NToys)) << "\n" ;
  cout << "sigma dei punti : " << sigmaToys
       << " +- " << sigmaToys / sqrt (2 * static_cast <double> (NToys - 1)) << "\n" ;

  return 0 ;
}