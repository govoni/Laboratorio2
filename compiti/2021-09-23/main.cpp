/*
c++ -o main main.cpp random.cc `root-config --glibs --cflags`
*/

#include <iostream>
#include <vector>
#include <cmath>
#include "random.h"

#include "TGraph.h"
#include "TCanvas.h"

using namespace std ;

double calcola_media (vector<double> & campione)
{
  double media = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    media += campione.at (i) ;
  return media / campione.size () ;
}

double calcola_sigma (vector<double> & campione)
{
  double media = 0. ;
  double media_sq = 0. ;
  for (int i = 0 ; i < campione.size () ; ++i)
    {
      media += campione.at (i) ;
      media_sq += campione.at (i) * campione.at (i) ;
    }  

  media /= campione.size () ;

  return sqrt ((media_sq - media * media * campione.size ()) / static_cast<double> (campione.size () - 1)) ;
}


double calcola_curtosi (vector<double> & campione)
{
  double media = calcola_media (campione) ;
  double sigma = calcola_sigma (campione) ;  
  double curtosi = 0. ;

  for (int i = 0 ; i < campione.size () ; ++i)
    curtosi += pow ((campione.at (i) - media) / sigma, 4) ;

  return curtosi / campione.size () ;
}


int main (int argc, char ** argv)
{

  int N_eventi = 10000 ;

  double valor_vero = 15. ;
  double sigma_vero = 3. ;

  TGraph g_andamento_curtosi ;

  for (int N_TCL = 1 ; N_TCL < 10000 ; N_TCL *= 2)
    {
      vector<double> campione ;

      for (int i = 0 ; i < N_eventi ; ++i)
        {
          campione.push_back (rand_TCL_gaus (valor_vero, sigma_vero, N_TCL)) ;
        }
      cout << N_TCL << "\t" 
           << calcola_media (campione) << "\t" 
           << calcola_sigma (campione) << "\t" 
           << calcola_curtosi (campione) << endl ;
      g_andamento_curtosi.SetPoint (g_andamento_curtosi.GetN (), N_TCL, calcola_curtosi (campione)) ;
    }

  TCanvas c1 ("c1", "c1", 300, 300) ;
  c1.SetLogx () ;
  g_andamento_curtosi.SetMarkerStyle (4) ;
  g_andamento_curtosi.Draw ("ALP") ;
  c1.Print ("andamento.png", "png") ;


  return 0 ;
}