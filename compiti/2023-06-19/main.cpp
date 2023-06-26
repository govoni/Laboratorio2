/*
c++ -o main main.cpp montecarlo.cc `root-config --glibs --cflags`
*/

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "statistiche_vector.h"
#include "montecarlo.h"

using namespace std ;

double f (double x, double y)
{
  return exp (-1 * x) * cos (y) ;
}


vector<double> calc_integral (int nEvents)
{
  double xMin = 0. ;
  double xMax = 2.72 ;
  double yMin = -1.57 ;
  double yMax = 1.57 ;
  double zMin = 0. ;
  double zMax = f (0., 0.) ;

  double volume = (xMax - xMin) * (yMax - yMin) * (zMax - zMin) ;

  double x = 0. ;
  double y = 0. ;
  double z = 0. ;

  int nUnder = 0 ;

  for (int i = 0 ; i < nEvents ; ++i)
    {
      x = rand_range (xMin, xMax) ;
      y = rand_range (yMin, yMax) ;
      z = rand_range (zMin, zMax) ;

      if (z > f (x,y)) continue ;
      ++nUnder ;
    }

  double fraction = nUnder / static_cast<double> (nEvents) ;

  vector<double> result ;
  result.push_back (volume * fraction) ;
  result.push_back (sqrt (volume * volume * fraction * (1 - fraction) / nEvents)) ;

  return result ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{

  if (argc < 3) 
    {
      cerr << "usage " << argv[0] << " numero_eventi numero_toy" << endl ;
      return 1 ;
    }

  int nEvents = atoi (argv[1]) ;
  int nToys = atoi (argv[2]) ;
  
 // - generare punti nello spazio 3D entro i limiti della funzione
 // - utilizzare i punti per calcolare l'integrale sotto la curva con N eventi e la sua incertezza

  vector<double> result = calc_integral (nEvents) ;
  cout << "integral : " <<  result[0] << "\t" ;
  cout << " +- " << result[1] << endl ;

 // - verificare l'incertezza con N toy

  TH1F h_integrals ("h_integrals", "", nToys/4, 
                    result.at (0) - 3 * result.at (1), result.at (0) + 3 * result.at (1)) ;
  TH1F h_sigmas ("h_sigmas", "", nToys/4, 
                    result.at (1) - sqrt (result.at (1)/nEvents), result.at (1) + sqrt (result.at (1)/nEvents)) ;
  vector<double> integrals ;
  vector<double> sigmas ;
  for (int iToy = 0 ; iToy < nToys ; ++iToy)
    {
      vector<double> result = calc_integral (nEvents) ;
      integrals.push_back (result.at (0)) ;
      h_integrals.Fill (result.at (0)) ;
      sigmas.push_back (result.at (1)) ;
      h_sigmas.Fill (result.at (1)) ;
    }

  cout << "sigma dei toy:       " << sigma (integrals) << endl ;
  cout << "media delle sigma:   " << media (sigmas) << endl ;
  cout << "sigma delle sigma 1: " << sigma (sigmas) << endl ;

  TCanvas c1 ;
  h_integrals.Draw ("hist") ;
  c1.Print ("h_integrals.png", "png") ;
  h_sigmas.Draw ("hist") ;
  c1.Print ("h_sigmas.png", "png") ;

 // - mostrare l'andamento della precisione vs N nei due casi

  TGraphErrors g_trend ;

  int index = 0 ;
  for (nEvents = 100 ; nEvents < 1000000 ; nEvents *= 3, ++index)
    {
      vector<double> result = calc_integral (nEvents) ;
      g_trend.SetPoint (index, nEvents, result.at (0)) ;
      g_trend.SetPointError (index, 0., result.at (1)) ;
    }

  g_trend.Draw ("AL*") ;
  c1.SetLogx (1) ;
  c1.Print ("trend.png", "png") ;

  return 0 ;
}