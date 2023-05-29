/*
c++ -o genera genera.cpp montecarlo.cc utils.cc `root-config --glibs --cflags`
*/

// #include <vector>
// #include <fstream>
// #include <iostream>

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

#include "statistiche_vector.h"
#include "montecarlo.h"
#include "utils.h"

using namespace std ;


int main (int argc, char ** argv)
{

  if (argc < 3) 
    {
      cerr << "usage " << argv[0] << " numero_eventi numero_toy" << endl ;
      return 1 ;
    }

  int nEvents = atoi (argv[1]) ;
  int nToys = atoi (argv[2]) ;

  TF1 fondo ("fondo", "pol2", 0., 5.) ;

  // concava
  // fondo.SetParameter (0, 1.) ;
  // fondo.SetParameter (1, -16./5.) ;
  // fondo.SetParameter (2, 4./5.) ;

  //convessa
  fondo.SetParameter (0, 1.) ;
  fondo.SetParameter (1, 4./5.) ;
  fondo.SetParameter (2, -1./5.) ;
  fondo.SetLineColor (kRed + 2) ;

  vector<double> params_t ; // true value
  params_t.push_back (1.) ;      // primo parametro parabola di fondo
  params_t.push_back (4./5.) ;   // secondo parametro parabola di fondo
  params_t.push_back (-1./5.) ;  // terzo parametro parabola di fondo
  params_t.push_back (2.) ;      // normalizzazione segnale
  params_t.push_back (2.) ;      // media segnale
  params_t.push_back (0.25) ;     // sigma segnale

  TF1 segnale ("segnale", "gaus", 0., 5.) ;
  segnale.SetParameter (0, params_t.at (3)) ;
  segnale.SetParameter (1, params_t.at (4)) ;
  segnale.SetParameter (2, params_t.at (5)) ;
  segnale.SetLineColor (kBlue) ;

  TF1 totale ("totale", "pol2(0) + gaus (3)", 0., 5.) ;
  totale.SetParameter (0, params_t.at (0)) ;
  totale.SetParameter (1, params_t.at (1)) ;
  totale.SetParameter (2, params_t.at (2)) ;
  totale.SetParameter (3, params_t.at (3)) ;
  totale.SetParameter (4, params_t.at (4)) ;
  totale.SetParameter (5, params_t.at (5)) ;
  segnale.SetLineColor (kBlack) ;


  TCanvas c1 ;
  totale.Draw () ;
  segnale.Draw ("same") ;
  fondo.Draw ("same") ;
  c1.Print ("funzione.png","png") ;

  vector<vector<double> > v_params (6, vector<double> ()) ;

  // ciclo sui toy
  for (int iToy = 0 ; iToy < nToys ; ++iToy)
    {
      vector<double> eventi ;
      // ciclo sugli eventi
      for (int iEv = 0 ; iEv < nEvents ; ++iEv)
        {
          eventi.push_back (rand_TAC (&totale, 0., 5., 2.)) ;
        }   // ciclo sugli eventi
      cout << "toy " << iToy 
           << "\tgenerati " << eventi.size () << " eventi\n" ;
      TH1F h_eventi ("h_eventi", "h_eventi", nEvents/20, 0., 5.) ;
      fill_histo (h_eventi, eventi) ;
    
      // TF1 fondo_fit ("fondo_fit", "pol2", 0., 5.) ;
      // TFitResultPtr f_result_fondo = h_eventi.Fit (&fondo_fit, "S", "", 3.5, 5.) ; 

      TF1 segnale_fit ("segnale_fit", "gaus", 0., 5.) ;
      TFitResultPtr f_result_segnale = h_eventi.Fit (&segnale_fit, "SQ+", "", 1., 3.) ; 
      cout << "   --> segnale: " << f_result_segnale->IsValid () << "\n" ;

      TF1 totale_fit ("totale_fit", "pol2(0) + gaus (3)", 0., 5.) ;
      totale_fit.SetParameter (4, segnale_fit.GetParameter (1)) ;
      totale_fit.SetParameter (5, segnale_fit.GetParameter (2)) ;

      TFitResultPtr f_result_totale = h_eventi.Fit (&totale_fit, "SQ+") ;
      cout << "   --> totale: " << f_result_totale->IsValid () << "\n" ;

      // plot di esempio per un singolo toy
      if (iToy == 1)
        {
          h_eventi.Draw () ;
          c1.Print ("eventi.png","png") ;
        }

      for (int iPar = 0 ; iPar < 6 ; ++iPar) 
        v_params.at (iPar).push_back (totale_fit.GetParameter (iPar)) ;

    } // ciclo sui toy

  // test di distorsione
  for (int iPar = 0 ; iPar < 6 ; ++iPar)
    {
       double media_par = media (v_params.at (iPar)) ;
       double sigma_par = sigma (v_params.at (iPar)) ;
       cout << " param: " << iPar
            << "\ttrue val: " << params_t.at (iPar)
            << "\ttoys mean: " << media_par 
            << "\ttoys sigma: " << sigma_par 
            << endl ;

//       int n_dentro = conta_in (v_params.at (iPar), media_par - sigma_par, media_par + sigma_par, valore_vero) ;
       

    }

  return 0 ;
}