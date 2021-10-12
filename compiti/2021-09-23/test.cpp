/*
c++ -o test `root-config --glibs --cflags` test.cpp random.cc
*/

#include "random.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"

using namespace std ;

int main () 
{
  int N_TCL = 10 ;
  int N_eventi = 10000 ;

  double media = 0 ;
  double media_sq = 0. ;

  double valor_vero = 15. ;
  double sigma_vero = 3. ;

  TH1F h_eventi ("h_eventi", "h_eventi", 100, valor_vero - 3 * sigma_vero, valor_vero + 3 * sigma_vero) ;

  for (int i = 0 ; i < N_eventi ; ++i)
    {
      double num = rand_TCL_gaus (valor_vero, sigma_vero, N_TCL) ;
      media += num ;
      media_sq += num * num ;
      h_eventi.Fill (num) ;
    }

  media /= N_eventi ;
  cout << "media   : " << media << endl ;
  cout << "sigma   : " << sqrt (media_sq / N_eventi - media * media) << endl ;
  cout << "sigma u.: " << sqrt ((media_sq - media * media * N_eventi) / static_cast<double> (N_eventi - 1)) << endl ;

  TCanvas c1 ("c1", "c1", 300, 300) ;
  h_eventi.Fit ("gaus") ;
  h_eventi.Draw () ;
  c1.Print ("generazione.png", "png") ;

  return 0 ;
}