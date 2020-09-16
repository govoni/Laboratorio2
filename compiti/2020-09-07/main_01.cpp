/* 
c++ -o main_01 `root-config --glibs --cflags` main_01.cpp
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TGraphErrors.h"

using namespace std ;



double attenuazione (double * x, double * par)
{
  if (x[0] == 0) return 10000 * par[0] ; //???
  return par[0] / x[0] + par[1] ;  
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


double rand_range (double min, double max)
{
  return min + (max - min) * rand () / RAND_MAX ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


int leggi (char * filename, TGraphErrors & grafico)
{
  ifstream input_file (filename) ;
  int i = 0 ;
  while (true) 
    {
      double dummy_x ;
      input_file >> dummy_x ;
      double dummyE_x ;
      input_file >> dummyE_x ;
      double dummy_y ;
      input_file >> dummy_y ;
      double dummyE_y ;
      input_file >> dummyE_y ;
      if (input_file.eof ()) break ;
      grafico.SetPoint (i, dummy_x, dummy_y) ;
      grafico.SetPointError (i, dummyE_x, dummyE_y) ;
      ++i ;
    } 
  return grafico.GetN () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


int main (int argc, char ** argv)
{
  if (argc < 4)
    {
      cerr << "uso: " << argv[0] << " passo prob_interazione file_attenuazione.txt" << endl ;
      exit (1) ;
    }

  // leggo l'andamento della funzione di attenuazione

  TGraphErrors g_attenuazione ;
  leggi (argv[3], g_attenuazione) ;

  TF1 f_attenuazione ("f_attenuazione", attenuazione, 0., 5., 2) ;
  f_attenuazione.SetLineWidth (2) ;
  f_attenuazione.SetLineColor (kRed) ;

  g_attenuazione.Fit ("f_attenuazione", "", "", 1.5, 5.) ;
  TFitResultPtr fr_attenuazione_1 = g_attenuazione.Fit ("f_attenuazione", "S") ;

  cout << f_attenuazione.GetParameter (0) << " +- " << f_attenuazione.GetParError (0) << endl ;
  cout << f_attenuazione.GetParameter (1) << " +- " << f_attenuazione.GetParError (1) << endl ;

  TCanvas c1 ;
  c1.SetLogy () ;
  g_attenuazione.Draw ("A*") ;
  f_attenuazione.Draw ("same") ;
  c1.Print ("attenuazione.png", "png") ;

  double passo    = atof (argv[1]) ; // spessore singolo passo
  double prob     = atof (argv[2]) ; // probabilita' per unita' di lunghezza
  double spessore = 5. ;             // spessore totale del materiale da attraversare

  int    N_part   = 10000 ; 
  int    N_passi = spessore / passo ;

  double Emin = 0. ;
  double Emax = 4. ;

  TH1F h_sopravv ("h_sopravv", "numero di particelle sopravvissute", N_passi, 0, spessore) ;
  TH1F h_energia_prima ("h_energia_prima", "spettro energetico prima dell'interazione", 20, Emin, Emax) ;
  TH1F h_energia_dopo  ("h_energia_dopo" , "spettro energetico dopo l'interazione"    , 20, Emin, Emax) ;

  // ciclo sulle particelle
  for (int i_part = 0 ; i_part < N_part ; ++i_part)
    {
      double E = rand_range (Emin, Emax) ;
      h_energia_prima.Fill (E) ;
      bool assorbita = false ;
      // ciclo sugli step 
      for (int i_passo = 0 ; i_passo <= N_passi ; ++i_passo)
        {
          if (rand_range (0., 1.) < f_attenuazione.Eval (E) * passo)
            {
              // interazione avvenuta
              assorbita = true ;
              break ;
            }
          h_sopravv.Fill (i_passo * passo) ;
        } // ciclo sugli step
      if (!assorbita) h_energia_dopo.Fill (E) ;
    } // ciclo sulle particelle

  c1.SetLogy (0) ;

  h_sopravv.Fit ("expo") ;
  h_sopravv.Draw ("hist") ;
  h_sopravv.GetFunction ("expo")->Draw ("same") ;
  c1.Print ("sopravv.png", "png") ;

  double Hmax = h_energia_prima.GetBinContent (h_energia_prima.GetMaximumBin ()) ;
  TH1F * bkg = c1.DrawFrame (Emin, 0., Emax, Hmax) ;
  bkg->GetXaxis ()->SetTitle ("energia (au)") ; 
  bkg->GetYaxis ()->SetTitle ("popolazione per bin") ; 
  h_energia_prima.SetLineColor (kBlue + 1) ;
  h_energia_dopo.SetLineColor (kBlue - 9) ;
  h_energia_dopo.SetFillColor (kBlue - 9) ;
  h_energia_prima.Draw ("hist same") ; 
  h_energia_dopo.Draw ("hist same") ; 

  c1.Print ("spettri.png", "png") ;

  return 0 ;
}