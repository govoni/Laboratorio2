/* 
c++ -o main_00 `root-config --glibs --cflags` main_00.cpp
*/


#include <iostream>
#include <cmath>

#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"

using namespace std ;

double rand_range (double min, double max)
{
  return min + (max - min) * rand () / RAND_MAX ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


int main (int argc, char ** argv)
{
  if (argc < 3)
    {
      cerr << "uso: " << argv[0] << " passo prob_interazione" << endl ;
      exit (1) ;
    }

  double passo    = atof (argv[1]) ; // spessore singolo passo
  double prob     = atof (argv[2]) ; // probabilita' per unita' di lunghezza
  double spessore = 5. ; // spessore totale del materiale da attraversare

  int    N_part   = 1000 ; 
  int    N_passi = spessore / passo ;

  TH1F h_sopravv ("h_sopravv", "numero di particelle sopravvissute", N_passi, 0, spessore) ;

  // ciclo sulle particelle
  for (int i_part = 0 ; i_part < N_part ; ++i_part)
    {
      // ciclo sugli step 
      for (int i_passo = 0 ; i_passo <= N_passi ; ++i_passo)
        {
          if (rand_range (0., 1.) < prob * passo)
            {
              // interazione avvenuta
              break ;
            }
          h_sopravv.Fill (i_passo * passo) ;
        } // ciclo sugli step
    } // ciclo sulle particelle

  TCanvas c1 ;
  h_sopravv.Fit ("expo") ;
  h_sopravv.Draw ("hist") ;
  h_sopravv.GetFunction ("expo")->Draw ("same") ;
  cout << setprecision (3) ;
  cout << h_sopravv.GetFunction ("expo")->GetParameter (1) 
       << " +- " << h_sopravv.GetFunction ("expo")->GetParError (1) 
       << " = " << -1 * prob / passo << endl ;

  c1.SetLogy () ;
  c1.Print ("sopravv.png", "png") ;

  c1.SetLogy (0) ;
  c1.Print ("sopravv_lin.png", "png") ;


  return 0 ;
}