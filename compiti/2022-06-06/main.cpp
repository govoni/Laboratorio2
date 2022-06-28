/*
c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <cmath>
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
  double min = 2. ; 
  double max = 4. ;
  double bin = 200. ;

  int N_max = 100000 ;

  vector<double> v_eventi ;
  TH1F h_eventi ("h_eventi", "h_eventi", bin, min, max) ;
  for (int evt = 0 ; evt < N_max ; ++evt)
    {
      v_eventi.push_back (rand_range (min, max)) ;
      h_eventi.Fill (v_eventi.back ()) ;
    }

  TCanvas c1 ;
  h_eventi.Draw () ;
  c1.Print ("eventi.png") ;

  double mean = N_max / bin ;

  TH1F h_conteggi ("h_conteggi", "h_conteggi", 100, mean - 3 * sqrt (mean), mean + 3 * sqrt (mean)) ; 
  vector<double> v_conteggi ;
  for (int bin = 1 ; bin <= h_eventi.GetNbinsX () ; ++bin)
    {
      v_conteggi.push_back (h_eventi.GetBinContent (bin)) ;
      h_conteggi.Fill (v_conteggi.back ()) ;
    } 

  cout << "media: " << media<double> (v_conteggi) << endl ;
  cout << "errore sulla media " << sqrt (varianza (v_conteggi) / v_conteggi.size ()) << endl ;
  cout << "varianza: " << varianza (v_conteggi)
       << " +- " << sqrt (2 * pow (varianza (v_conteggi), 2) / (v_conteggi.size () - 1)) << endl ;
  cout << "sigma: " << sqrt (varianza (v_conteggi)) << endl ;
  h_conteggi.Draw () ;
  c1.Print ("conteggi.png") ;

  return 0 ;
}