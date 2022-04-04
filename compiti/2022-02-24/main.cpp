/*
c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <iostream>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH1F.h"

#include "lib.h"

using namespace std ;

int main (int argc, char ** argv)
{

  if (argc < 2)
    {
      cerr << "uso del programma: "
           << argv[0] << " lambda_poisson\n" ;
      return 1 ;     
    }

  double lambda = atof (argv[1]) ;
  int N = ceil (3 * lambda) ;
  TH1F h_pois ("h_pois", "h_pois", N, 0, N) ;
  vector<int> campione ;

  // ciclo su tanti pseudo-esperimenti
  for (int i = 0 ; i < 1000 ; ++i)
    {
      campione.push_back (rand_pois (lambda)) ;
      h_pois.Fill (campione.back ()) ;
    } // ciclo su tanti pseudo-esperimenti

  TCanvas c1 ;
  h_pois.SetFillColor (kAzure + 1) ;
  h_pois.Draw ("hist") ;
  c1.Print ("poisson.png", "png") ;

  campione.clear () ;
  lambda = 3. ;

  // ciclo su tanti pseudo-esperimenti
  for (int i = 0 ; i < 10 ; ++i)
    {
      campione.push_back (rand_pois (lambda)) ;
      cout << "evento " << i << " = " << campione.back ()
           << " --> probabilità = " << poisson (campione.back (), lambda)
           << "\n" ;
    } // ciclo su tanti pseudo-esperimenti

  for (int i = 1 ; i < 6 ; ++i)
    {
      cout << "lambda = " << i << " --> likelihood = " << calcLikelihood (campione, i) << "\n" ;
    }

  return 0 ;
}