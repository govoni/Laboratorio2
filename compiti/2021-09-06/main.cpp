/*
c++ -o main main.cpp matrici.cc random.cc `root-config --glibs --cflags`
*/

#include <iostream>
#include <vector>
#include <cmath>
#include "matrici.h"
#include "random.h"

#include "TH2F.h"
#include "TCanvas.h"

using namespace std ;


double covarianza (const vector<vettore2> & punti)
  {
    double somma_x = 0. ;
    double somma_y = 0. ;
    double somma_prod = 0. ;
    for (int i = 0 ; i < punti.size () ; ++i)
      {
        somma_x += punti.at (i).elem (0) ;
        somma_y += punti.at (i).elem (1) ;
        somma_prod += punti.at (i).elem (0) * punti.at (i).elem (1) ; 
      }
    return somma_prod / punti.size () - somma_x * somma_y / (punti.size () * punti.size ()) ;
  }


int main (int argc, char ** argv)
{

  double angolo = M_PI / 3. ;
  matrice2x2 ruota (angolo) ; 
  int n_punti = 1000000 ;
  vector<vettore2> punti ;
  vector<vettore2> punti_ruotati ;

  TH2F h_punti ("h_punti", "h_punti", 100, -9., 9., 100, -9., 9.) ;
  TH2F h_punti_ruotati ("h_punti_ruotati", "h_punti_ruotati", 100, -9., 9., 100, -9., 9.) ;

  for (int i = 0 ; i < n_punti ; ++i)
    {
      vettore2 gen (
        rand_TAC_gaus (0, 3.), 
        rand_TAC_gaus (0, 1.) 
      ) ;
      punti.push_back (gen) ;
      vettore2 rot = ruota * gen ;
      punti_ruotati.push_back (rot) ;

      h_punti.Fill (gen.elem (0), gen.elem (1)) ;
      h_punti_ruotati.Fill (rot.elem (0), rot.elem (1)) ;
    }

  TCanvas c1 ("c1", "", 500, 500) ;
  h_punti.SetMarkerColor (kRed) ;
  h_punti.Draw () ;
  h_punti_ruotati.SetMarkerColor (kBlue) ;
  h_punti_ruotati.Draw ("same") ;
  c1.Print ("plot.png", "png") ;

  cout << "covarianza prima della rotazione: " << covarianza (punti) << "\n" ;
  cout << "covarianza dopo la rotazione:     " << covarianza (punti_ruotati) << "\n" ;

  return 0 ;
}