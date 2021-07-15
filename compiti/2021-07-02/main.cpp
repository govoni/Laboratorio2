/*
c++ -o main `root-config --glibs --cflags` main.cpp lib.cc
*/

#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TCanvas.h"

#include "lib.h"

using namespace std ;

int main (int argc, char ** argv)
{
  if (argc < 2)
    {
      cout << "uso : " << argv[0] << " N_eventi\n" ;
      exit (1) ;
    }  

  /*
  si generi un campione di N numeri casuali inseriti dall'utente distribuiti secondo la distribuzione di densit\`a di probabilit\`a
  f (x) = \frac{1}{2} ( \sin x)
  definita sull'insieme $(0,\pi)$ utilizzando la tecnica di try and catch
  */

  vector<double> v_campione ;
  int N_eventi = atoi (argv[1]) ;
  for (int i = 0 ; i < N_eventi ; ++i)
    v_campione.push_back ( rand_TAC (f_sin, 0, M_PI, 1.)) ;
  
  /* 
  Si visualizzi il campione in un istogramma di ROOT, 
  salvato in un file di tipo png.
  */

  TH1F * h_campione = new TH1F (
    "h_campione", "h_campione", 
    v_campione.size () / 10,
    floor (*min_element (v_campione.begin (), v_campione.end ())),
    ceil (*max_element (v_campione.begin (), v_campione.end ()))
    ) ;
  fillHisto (h_campione, v_campione) ;
  TCanvas c1 ;
  h_campione->Draw ("hist") ;
  c1.Print ("campione.png", "png") ;

  /*
  si calcoli la media e l'incertezza sulla media del campione di eventi
  */

  double media = 0. ;
  double mediaSq = 0. ;
  for (int i = 0 ; i < v_campione.size () ; ++i) 
    {
      media += v_campione.at (i) ;
      mediaSq += v_campione.at (i) * v_campione.at (i) ;
    }  
  media /= v_campione.size () ;
  mediaSq /= v_campione.size () ;
  cout << "media del campione: " << media 
       << " +- " << sqrt ((mediaSq - media * media) / (v_campione.size () - 1)) << endl ;

  /* 
  utilizzando il metodo dei toy experiment e dopo aver calcolato analiticamente il valore di aspettazione della distribuzione utilizzata,
  si mostri che la media campionaria ne e\` uno stimatore non distorto.
  */

  int N_toys = 1000 ;
  vector<double> v_medie ;
  // ciclo di toy experiment
  for (int i_toy = 0 ; i_toy < N_toys ; ++i_toy)
    {
      // generazione di un campione
      vector<double> v_campione_local ;
      int N_eventi = atoi (argv[1]) ;
      for (int i = 0 ; i < N_eventi ; ++i)
        v_campione_local.push_back ( rand_TAC (f_sin, 0, M_PI, 1.)) ;

      double media = 0. ;
      for (int i = 0 ; i < v_campione_local.size () ; ++i) 
        {
          media += v_campione_local.at (i) ;
        }  
      media /= v_campione_local.size () ;
      v_medie.push_back (media) ;
    } // ciclo di toy experiment

  media = 0 ; 
  mediaSq = 0 ;
  for (int i = 0 ; i < v_medie.size () ; ++i) 
    {
      media += v_medie.at (i) ;
      mediaSq += v_medie.at (i) * v_medie.at (i) ;
    }  
  media /= v_medie.size () ;
  mediaSq /= v_medie.size () ;
  sigma = sqrt (mediaSq - media * media) * v_medie.size () / (v_medie.size () - 1) ;

  if (fabs (media - 0.5 * M_PI) / sigma < 1.) cout << "stimatore non distorto: " ;
  else                                        cout << "stimatore distorto: " ;
  cout << media << " +- " << sigma << endl ;

  delete h_campione ;
  return 0 ;
}