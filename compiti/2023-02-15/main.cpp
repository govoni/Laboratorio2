/*
c++ -o main main.cpp montecarlo.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <iostream>

#include "TH1F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "montecarlo.h"
#include "statistiche_vector.h"

using namespace std ;


int bins (int N_eventi)
{
  return ceil (0.5 * sqrt (N_eventi)) ;
}


// ---- ---- ---- ---- ---- ---- 


int main (int argc, char ** argv)
{

  if (argc < 2)
    {
      cerr << "uso: " << argv[0] << " numero_eventi" << endl ;
      return 1 ;
    }
  int N_somme_max = 4100 ;
  if (argc > 2)
    {
      N_somme_max = atoi (argv[2]) ;
    }

  int N_eventi = atoi (argv[1]) ;

  TH1F h_parabola ("h_parabola", "h_parabola", bins (N_eventi), 1., 3.) ;
  for (int i = 0 ; i < N_eventi ; ++i) h_parabola.Fill (rand_TAC (parabola, 1., 3., 1.)) ;
  TCanvas c1 ;
  h_parabola.Draw ("hist") ;
  c1.Print ("parabola.png", "png") ;

  TH1F h_gauss_TCL_unif ("h_gauss_TCL_unif", "h_gauss_TCL_unif", bins (N_eventi), 1., 3.) ;
  for (int i = 0 ; i < N_eventi ; ++i) h_gauss_TCL_unif.Fill (rand_TCL_unif ()) ;
  h_gauss_TCL_unif.Draw ("hist") ;
  c1.Print ("h_gauss_TCL_unif.png", "png") ;

  TH1F h_gauss_TCL_para ("h_gauss_TCL_para", "h_gauss_TCL_para", bins (N_eventi), 1., 3.) ;
  for (int i = 0 ; i < N_eventi ; ++i) h_gauss_TCL_para.Fill (rand_TCL_para ()) ;
  h_gauss_TCL_para.Draw ("hist") ;
  c1.Print ("h_gauss_TCL_para.png", "png") ;

  TGraph g_asim_unif ;
  TGraph g_asim_para ;
  
  TGraph g_curt_unif ;
  TGraph g_curt_para ;

  // ciclo sul numero di pdf sommate nella tecnica TCL
  for (int N_somme = 2 ; N_somme < N_somme_max ; N_somme *= 2) 
    {
      cout << "testing " << N_somme << " pdf\n" ;

      vector<double> v_TCL_unif ;
      for (int i = 0 ; i < N_eventi ; ++i) v_TCL_unif.push_back (rand_TCL_unif (N_somme)) ;
      g_asim_unif.SetPoint (g_asim_unif.GetN (), N_somme, asimmetria (v_TCL_unif)) ;
      g_curt_unif.SetPoint (g_curt_unif.GetN (), N_somme, curtosi (v_TCL_unif)) ;
      
      vector<double> v_TCL_para ;
      for (int i = 0 ; i < N_eventi ; ++i) v_TCL_para.push_back (rand_TCL_para (N_somme)) ;
      g_asim_para.SetPoint (g_asim_para.GetN (), N_somme, asimmetria (v_TCL_para)) ;
      g_curt_para.SetPoint (g_curt_para.GetN (), N_somme, curtosi (v_TCL_para)) ;

    } // ciclo sul numero di pdf sommate nella tecnica TCL

  g_asim_unif.SetLineColor (kRed) ;
  g_asim_para.SetLineColor (kBlue) ;

  g_asim_unif.Draw ("AL") ;  
  g_asim_para.Draw ("Lsame") ;  

  TLegend legenda_asim (0.65, 0.1, 0.9, 0.25) ;
  legenda_asim.AddEntry (&g_asim_unif, "uniforme", "L") ;
  legenda_asim.AddEntry (&g_asim_para, "parabola", "L") ;
  legenda_asim.Draw () ;

  c1.SetLogx () ;
  c1.Print ("asimmetria.png", "png") ;

  g_curt_unif.SetLineColor (kRed) ;
  g_curt_para.SetLineColor (kBlue) ;

  g_curt_unif.Draw ("AL") ;  
  g_curt_para.Draw ("Lsame") ;  

  TLegend legenda_curt (0.65, 0.1, 0.9, 0.25) ;
  legenda_curt.AddEntry (&g_curt_unif, "uniforme", "L") ;
  legenda_curt.AddEntry (&g_curt_para, "parabola", "L") ;
  legenda_curt.Draw () ;

  c1.Print ("curtosi.png", "png") ;



  return 0 ;
}