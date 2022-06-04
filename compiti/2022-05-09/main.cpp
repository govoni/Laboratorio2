/*
c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <fstream>
#include <iostream>

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"

#include "lib.h"

using namespace std ;

int main (int argc, char ** argv)
{

  if (argc < 2)
    {
      cerr << "uso del programma: "
           << argv[0] << " nome_file.txt\n" ;
      return 1 ;     
    }

  vector<double> data_x ;
  vector<double> data_y ;

  leggiFile (argv[1], data_x, data_y) ;

  for (int i = 0 ; i < data_x.size () ; ++i)
    {
      cout << data_x.at (i) << ", " << data_y.at (i) << "\n" ;
    }

  TGraph g_punti (data_x.size (), &data_x.at (0), &data_y.at (0)) ;

  TF1 tf1_sin ("tf1_sin", f_sin, 0., 20., 3) ;
  tf1_sin.SetParameter (0, findAbsMax (data_y)) ;
  g_punti.Fit (&tf1_sin) ;  

  TCanvas c1 ;
  g_punti.Draw ("A*") ;
  c1.Print ("grafico.png") ;

  TH1F * h_scarti = calcolaScarti (tf1_sin, data_x, data_y) ;
  h_scarti->Fit ("gaus", "L") ;
  h_scarti->Draw () ;
  c1.Print ("scarti.png") ;

  delete h_scarti ;

  return 0 ;
}