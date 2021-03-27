/*
 c++ -o main_01 `root-config --cflags --glibs` libreria.cc main_01.cpp
*/

#include <iostream>
#include <vector>
#include <ctime>

#include <TF1.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFitResultPtr.h>

#include "libreria.h"
#include "statistiche_vector.h"

using namespace std ;

int main (int argc, char ** argv)
{
  // Si scriva una funzione che genera sei punti x_i distribuiti casualmente lungo l'asse orizzontale
  // compresi fra 0 e 10

  int N_max = 6 ;
  float x_min = 0. ;
  float x_max = 10. ;
  double A_1     = 3.0 ;
  double omega_1 = 0.2 * M_PI ;
//  double omega_1 = 0.3 * M_PI ;
  double A_2     = 0.2 ;
  double omega_2 = 4 * M_PI ;

  TF1 segnale ("segnale", "[0] * sin ([1] * x) + [2] * sin ([3] * x)", 0., 10.) ;
  segnale.SetParameter (0., A_1) ;
  segnale.SetParameter (1., omega_1) ;
  segnale.SetParameter (2., A_2) ;
  segnale.SetParameter (3., omega_2) ;

  TF1 modello ("modello", "[0] * sin ([1] * x)", 0., 10.) ;

  TH1F h_bias_A ("h_bias_a", "bias A", 100, -2., 2.) ;
  TH1F h_bias_omega ("h_bias_omega", "bias omega", 100, -0.2, 0.2) ;

  srand (time (NULL)) ;

  vector<double> v_a ;
  vector<double> v_omega ;
  int N_toys = 1000 ;
  // ciclo sui toy per determinare il valore di aspettazione dello stimatore
  // a partire dalla sua pdf
  for (int i_toy = 0 ; i_toy < N_toys ; ++i_toy)
    {
      vector<double> v_x ;
      vector<double> v_y_noisy ;
      float sigma = 0.2 ;
      for (int i = 0 ; i < N_max ; ++i)
        {
          v_x.push_back (rand_range (x_min, x_max)) ; 
          v_y_noisy.push_back (segnale.Eval (v_x.back ()) + rand_TAC_gaus0 (sigma)) ;
        }
      TGraph segnale_noisy (v_x.size (), &v_x.at (0), &v_y_noisy.at (0)) ;
      modello.SetParameter (0, get_max_abs (v_y_noisy)) ;
      // modello.SetParameter (0, 3) ;
      modello.SetParameter (1, 0.6) ; 
      modello.SetLineColor (kGreen+2) ;
      TFitResultPtr result = segnale_noisy.Fit ("modello", "SQ") ;
      v_a.push_back (modello.GetParameter (0)) ;
      v_omega.push_back (modello.GetParameter (1)) ;
      h_bias_A.Fill (modello.GetParameter (0) - A_1) ;
      h_bias_omega.Fill (modello.GetParameter (1) - omega_1) ;
    } // ciclo sui toy per il test del bias

  TCanvas c1 ;
  h_bias_A.Draw ("hist") ;
  c1.Print ("bias_A.png", "png") ;
  h_bias_omega.Draw ("hist") ;
  c1.Print ("bias_omega.png", "png") ;

  double bias_a = media (v_a) - A_1 ;
  double bias_omega = media (v_omega) - omega_1 ;

  if (fabs (bias_a) < sqrt (varianza (v_a)))
    {
      cout << "Lo stimatore di A non e' distorto: "
           << " il bias (" << bias_a
           << ") e' minore in modulo dell'incertezza sul valore di aspettazione ("
           << sqrt (varianza (v_a)) << ")\n" ;
    }
  else  
    {
      cout << "Lo stimatore di A e' distorto: "
           << " il bias (" << bias_a
           << ") e' maggiore in modulo dell'incertezza sul valore di aspettazione ("
           << sqrt (varianza (v_a)) << ")\n" ;
    }

  if (fabs (bias_omega) < sqrt (varianza (v_omega)))
    {
      cout << "Lo stimatore di Omega non e' distorto: "
           << " il bias (" << bias_omega
           << ") e' minore in modulo dell'incertezza sul valore di aspettazione ("
           << sqrt (varianza (v_omega)) << ")\n" ;
    }
  else  
    {
      cout << "Lo stimatore di Omega e' distorto: "
           << " il bias (" << bias_omega
           << ") e' maggiore in modulo dell'incertezza sul valore di aspettazione ("
           << sqrt (varianza (v_omega)) << ")\n" ;
    }
  return 0 ;
}