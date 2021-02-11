#include "image.h"

#include "statistiche_vector.h"
#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TStyle.h>


using namespace std ;

image::image () : n_righe (0), n_colonne (0), 
                  h_spettro (0), h_raw_image (0), h_image (0), h_fetta (0), h_proiezione (0) {} ;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

image::image (const string & fileName) 
{
  readFile (fileName) ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


image::~image () 
{
  if (h_spettro != 0) delete h_spettro ;
  if (h_raw_image != 0) delete h_raw_image ;
  if (h_image != 0) delete h_image ;
  if (h_fetta != 0) delete h_fetta ;
  if (h_proiezione != 0) delete h_proiezione ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

void image::readFile (const string & fileName) 
{
  int x = 0 ;
  int y = 0 ;
  double val = 0. ;
  ifstream inputFile (fileName.c_str ()) ;
  while (1)
    {
      // inputFile >> x ;
      // inputFile >> y ;
      // inputFile >> val ;
      inputFile >> x >> y >> val ;
      if (inputFile.eof () == true) break ; 
      v_x.push_back (x) ;
      v_y.push_back (y) ;
      v_val.push_back (val) ;
    }
  inputFile.close () ;

  v_val_corr = v_val ;
  n_righe = *max_element (v_x.begin (), v_x.end ()) + 1 ;
  n_colonne = *max_element (v_y.begin (), v_y.end ()) + 1 ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

TH1F * image::drawSpectrum (const std::string & histoName)
{
  double max_val = *max_element (v_val.begin (), v_val.end ()) + 1. ;
  double var_sigma = sqrt (varianza (v_val)) ;
  int N_bins_val = 100 * int (max_val / var_sigma) ; 
  if (h_spettro != 0) delete h_spettro ;
  h_spettro = new TH1F (histoName.c_str (), "h_spettro", N_bins_val, 0, max_val) ;

  for (int i = 0 ; i < v_val.size () ; ++i)
    {
      h_spettro->Fill (v_val.at (i)) ;
    }
  return h_spettro ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

TH2F * image::drawRawImage (const std::string & histoName)
{
  if (h_raw_image != 0) delete h_spettro ;
  h_raw_image = new TH2F (histoName.c_str (), "h_raw_image", n_righe, 0, n_righe, n_colonne, 0, n_colonne) ;
  h_raw_image->SetStats (0) ;
  for (int i = 0 ; i < v_x.size () ; ++i)
    {
      h_raw_image->Fill (v_x.at (i), v_y.at (i), v_val.at (i)) ;
    }
  return h_raw_image ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

TH2F * image::drawImage (const string & histoName)
{
  if (h_image != 0) delete h_image ;
  h_image = new TH2F (histoName.c_str (), "h_image", n_righe, 0, n_righe, n_colonne, 0, n_colonne) ;
  h_image->SetStats (0) ;
  for (int i = 0 ; i < v_x.size () ; ++i)
    {
      h_image->Fill (v_x.at (i), v_y.at (i), v_val_corr.at (i)) ;
    }
  return h_image ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

TH1F * image::fettaYfissato (int ybin, const string & histoName)
{
  if (h_fetta != 0) delete h_fetta ;
  h_fetta = new TH1F (histoName.c_str (), "fettaYfissato", n_righe, 0, n_righe) ;
  for (int i = 0 ; i < v_x.size () ; ++i)
    {
      if (v_y.at (i) != ybin) continue ;
      h_fetta->Fill (v_x.at (i), v_val.at (i)) ;
    }
  return h_fetta ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

TH1F * image::proiezioneSuX (const string & histoName)
{
  vector<double> integral (n_righe, 0.) ;
  for (int i = 0 ; i < v_x.size () ; ++i)
    {
      integral.at (v_x.at (i)) += v_val_corr.at (i) ;
    }
  if (h_proiezione != 0) delete h_proiezione ;
  h_proiezione = new TH1F (histoName.c_str (), "proiezioneSuX", n_righe, 0, n_righe) ;
  for (int i = 0 ; i < integral.size () ; ++i)
    {
      h_proiezione->Fill (i, integral.at (i)) ;
    }
  return h_proiezione ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

double image::varianzaLungoX () 
{
  double norm = 0. ;
  double mean = 0. ;
  double meanSq = 0. ;
  for (int i = 0 ; i < v_x.size () ; ++i)
    {
      norm += v_val.at (i) ;
      mean += v_x.at (i) * v_val.at (i) ;
      meanSq += v_x.at (i) * v_x.at (i) * v_val.at (i) ;
    }
  mean /= norm ;
  meanSq /= norm ;
  return meanSq - mean * mean ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

double image::varianzaLungoY () 
{
  double norm = 0. ;
  double mean = 0. ;
  double meanSq = 0. ;
  for (int i = 0 ; i < v_y.size () ; ++i)
    {
      norm += v_val.at (i) ;
      mean += v_y.at (i) * v_val.at (i) ;
      meanSq += v_y.at (i) * v_y.at (i) * v_val.at (i) ;
    }
  mean /= norm ;
  meanSq /= norm ;
  return meanSq - mean * mean ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

double image::covarianza ()
{
  double norm = 0. ;
  double meanx = 0. ;
  double meany = 0. ;
  double xyprd = 0. ;
//  double meanSq = 0. ;
  for (int i = 0 ; i < v_y.size () ; ++i)
    {
      norm += v_val.at (i) ;
      meanx += v_x.at (i) * v_val.at (i) ;
      meany += v_y.at (i) * v_val.at (i) ;
      xyprd += v_x.at (i) * v_y.at (i) * v_val.at (i) ;
    }
  xyprd /= norm ;
  meanx /= norm ;
  meany /= norm ;
  double cov = 0. ;
  for (int i = 0 ; i < v_y.size () ; ++i)
    {
      cov += (v_x.at (i) - meanx) * (v_y.at (i) - meany)  * v_val.at (i) ;
    }
  cov /= norm ;
  return xyprd - meanx * meany ;
} 

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

vector<double> image::getPedestal (double min, double max, TH1F * h_spettro)
{
  TF1 f_noise ("f_noise", "gaus(0)", min, max) ;
  TFitResultPtr fitresult = h_spettro->Fit ("f_noise", "S") ;
  double media = f_noise.GetParameter (1) ;
  double sigma = f_noise.GetParameter (2) ;
  vector<double> result ;
  result.push_back (media) ;
  result.push_back (sigma) ;
  return result ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

void image::subtractPedestal (double valore)
{
  for (int i = 0 ; i < v_val_corr.size () ; ++i)
    {
      if (v_val_corr.at (i) - valore < 0.) continue ;
      v_val_corr.at (i) -= valore ;
    }
  return ;
}

