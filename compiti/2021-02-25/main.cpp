/*
 c++ -o main `root-config --cflags --glibs` libreria.cc main.cpp
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

using namespace std ;

int main (int argc, char ** argv)
{
  // Si scriva una funzione che genera sei punti x_i distribuiti casualmente lungo l'asse orizzontale
  // compresi fra 0 e 10

  int N_max = 6 ;
  float x_min = 0. ;
  float x_max = 10. ;

  srand (time (NULL)) ;

  vector<double> v_x ;
  for (int i = 0 ; i < N_max ; ++i)
    {
      v_x.push_back (rand_range (x_min, x_max)) ; 
    }

  // Si scriva una funzione che, dato un punto x_i, calcoli y_i come:
  // y_i = A_1 * sin (omega_1 * x_i) + A_2 sin (omega_2 * x_i) + G (0,sigma),
  // dove A_1 = 3.0, omega_1=0.2pi, A_2 = 0.2, omega_2=pi 
  // e G (0,sigma) indica un numero casuale 
  // distribuito secondo una Gaussiana di media zero e deviazione standard sigma = 0.2
  // e rappresenta il rumore elettronico.

  double A_1     = 3.0 ;
  double omega_1 = 0.2 * M_PI ;
  double A_2     = 0.2 ;
  double omega_2 = M_PI ;
  TF1 segnale ("segnale", "[0] * sin ([1] * x) + [2] * sin ([3] * x)", 0., 10.) ;
  segnale.SetParameter (0., A_1) ;
  segnale.SetParameter (1., omega_1) ;
  segnale.SetParameter (2., A_2) ;
  segnale.SetParameter (3., omega_2) ;

  vector<double> v_y_clean ;
  vector<double> v_y_noisy ;
  float sigma = 0.2 ;
  for (int i = 0 ; i < v_x.size () ; ++i)
    {
      v_y_clean.push_back (segnale.Eval (v_x.at (i))) ;
      v_y_noisy.push_back (segnale.Eval (v_x.at (i)) + rand_TAC_gaus0 (sigma)) ;
    }

  // Si disegni su un TCanvas di ROOT la forma funzionale del segnale dell'equazione precedente, 
  // senza il rumore elettronico.
  // Si utilizzino le coppie di punti (x_i, y_i) generate al punto 2 per riempire un TGraph di ROOT
  // e lo si disegni sovrapposto al segnale.

  TCanvas c1 ;
  TH1F * bkg = c1.DrawFrame (0., -1.2 * A_1, 11, 1.2 * A_1) ;
  bkg->GetXaxis ()->SetTitle ("asse x") ;
  bkg->GetYaxis ()->SetTitle ("asse y") ;
  TGraph segnale_clean (v_x.size (), &v_x.at (0), &v_y_clean.at (0)) ;
  TGraph segnale_noisy (v_x.size (), &v_x.at (0), &v_y_noisy.at (0)) ;
  segnale.SetLineColor (kGray + 2) ;
  segnale.SetLineWidth (1) ;
  segnale.Draw ("same") ;
  segnale_noisy.SetMarkerStyle (29) ;
  segnale_noisy.SetMarkerColor (kRed) ;
  segnale_noisy.SetMarkerSize (2) ;
  segnale_noisy.Draw ("P same") ;
  segnale_clean.SetMarkerStyle (24) ;
  segnale_clean.SetMarkerColor (kBlue) ;
  segnale_clean.Draw ("P same") ;
  c1.Print ("segnale.png", "png") ;

  // Si esegua il fit della funzione 
  // g(x) = A * sin(omega * x)
  // sul TGraph per ricavare i valori di A_1 ed omega_1.

  TF1 modello ("modello", "[0] * sin ([1] * x)", 0., 10.) ;
  modello.SetParameter (0, get_max_abs (v_y_noisy)) ;
  // modello.SetParameter (0, 3) ;
  modello.SetParameter (1, 0.6) ; 
  modello.SetLineColor (kGreen+2) ;
  TFitResultPtr result = segnale_noisy.Fit ("modello", "S") ;
  modello.Draw ("same") ;
  c1.Print ("segnale_fit.png", "png") ;

  return 0 ;
}