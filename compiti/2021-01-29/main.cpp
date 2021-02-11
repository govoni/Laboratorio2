/*
c++ -o main `root-config --glibs --cflags` image.cc main.cpp
*/

#include "statistiche_vector.h"
#include "image.h"

#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TStyle.h>
#include <TEllipse.h>

using namespace std ;

int main (int argc, char ** argv)
{
  image figura (argv[1]) ;  
  TH2F * h_immagine_grezza = figura.drawRawImage ("grezza") ;

  TCanvas c1 ;
  gStyle->SetPalette (kGreyScale) ;
  h_immagine_grezza->Draw ("colz") ;
  c1.Print ("h_immagine_raw.png", "png") ;

  TH1F * h_spettro = figura.drawSpectrum ("spettro") ;
  TF1 f_noise ("f_noise", "gaus(0)", 0., 50.) ;
  TFitResultPtr fitresult = h_spettro->Fit ("f_noise", "S") ;

  h_spettro->Draw ("hist") ;
  f_noise.Draw ("same") ;
  c1.Print ("h_spettro.png", "png") ;

  double mean = f_noise.GetParameter (1) ;
  figura.subtractPedestal (mean) ;

  TH2F * h_immagine = figura.drawImage ("filtrata") ;

  h_immagine->Draw ("colz") ;
  c1.Print ("h_immagine.png", "png") ;

  TH1F * h_fettaY_atX_500 = figura.fettaYfissato (500, "h_fettaY_atX_500") ;

  h_fettaY_atX_500->Draw ("hist") ;
  c1.Print ("h_immagine_1D_Yfissato.png", "png") ;

  TH1F * h_proiezioneSuX = figura.proiezioneSuX ("h_proiezioneSuX") ;
  h_proiezioneSuX->Draw ("hist") ;
  c1.Print ("h_proiezioneSuX.png", "png") ;

  double varianza_x = figura.varianzaLungoX () ;
  cout << "varianza x: " <<  varianza_x << endl ;
  cout << "sigma x: " << sqrt (varianza_x) << endl ;

  double varianza_y = figura.varianzaLungoY () ;
  cout << "varianza y: " <<  varianza_y << endl ;
  cout << "sigma y: " << sqrt (varianza_y) << endl ;

  double covarianza = figura.covarianza () ;
  cout << "sigma xy: " << covarianza << endl ;

  return 0 ;
}  