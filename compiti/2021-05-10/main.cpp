/*
c++ -o main `root-config --glibs --cflags` main.cpp
./main dati.txt
*/

#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TGraphErrors.h>

using namespace std ;

double f_lin (double * x, double * par)
{
  return par[0] + x[0] * par[1] ;
}

double f_lin_zero (double * x, double * par)
{
  return x[0] * par[0] ;
}

int main (int argc, char ** argv)
{
  // lettura del file di testo

  ifstream input_file ; 
  input_file.open (argv[1], ios::in) ;

  vector<double> data_x ;
  vector<double> data_y ;
  vector<double> err_y ;
  double input_x, input_y, input_ey ;
  while (true) 
    {
      input_file >> input_x ;
      input_file >> input_y ;
      input_file >> input_ey ;
      if (input_file.eof () == true) break ;
      data_x.push_back (input_x) ;
      data_y.push_back (input_y) ;
      err_y.push_back (input_ey) ;
    } 
  input_file.close () ;

  // creazione e disegno del TGraph

  vector<double> err_x (err_y.size (), 0.) ;
  
  TGraphErrors g_trend (data_x.size (), 
                        &data_x.at (0), &data_y.at (0),
                        &err_x.at (0), &err_y.at (0)) ;
  TCanvas c1 ;
  g_trend.Draw ("A*") ;
  c1.Print ("lettura.png", "png") ;

  // primo fit

  TF1 tf1_lin ("tf1_lin", f_lin, 0., 10., 2) ;
//  TF1 tf1_lin ("tf1_lin","[0]*x + [1]") ;  //funzione del tipo v=a*t + v0

  tf1_lin.SetLineWidth (1) ;
  TFitResultPtr fit_result_1 = g_trend.Fit ("tf1_lin", "S+") ;
  cout << "fit 1: accelerazione:     " << tf1_lin.GetParameter (1) 
       << "\t+- " << tf1_lin.GetParError (1) << endl ;
  cout << "fit 1: velocità iniziale: " << tf1_lin.GetParameter (0) 
       << "\t+- " << tf1_lin.GetParError (0) << endl ;

  if (!fit_result_1->IsValid ())
    {
      cerr << "il primo fit non ha avuto successo\n" ;
      return 1 ;
    }

  double p_value_1 = fit_result_1->Prob () ;

  cout << "la velocità iniziale dista dallo zero " 
       << tf1_lin.GetParameter (0) / tf1_lin.GetParError (0)
       << " volte la sua incertezza\n" ;

  // secondo fit

  TF1 tf1_lin_zero ("tf1_lin_zero", f_lin_zero, 0., 10., 1) ;
  tf1_lin_zero.SetLineWidth (1) ;
  tf1_lin_zero.SetLineColor (kBlue) ;
  TFitResultPtr fit_result_2 = g_trend.Fit ("tf1_lin_zero", "S+") ;
  cout << "fit 2: accelerazione:     " << tf1_lin_zero.GetParameter (0) 
       << "\t+- " << tf1_lin_zero.GetParError (0) << endl ;
  double p_value_2 = fit_result_2->Prob () ;

  cout << "fit 1: p-value : " << p_value_1 << "\n" ;
  cout << "fit 1: DOF     : " << fit_result_1->Ndf () << "\n" ;
  cout << "fit 2: p-value : " << p_value_2 << "\n" ;
  cout << "fit 2: DOF     : " << fit_result_2->Ndf () << "\n" ;

  g_trend.Draw ("A*") ;
  c1.Print ("fit.png", "png") ;

  // determinazione delle incertezze y

  double scale_varianza = fit_result_1->Chi2 () / fit_result_1->Ndf () ;
  cout << "fattore di scala per la varianza: " << scale_varianza << "\n" ;
  cout << "fattore di scala per la sigma   : " << sqrt (scale_varianza) << "\n" ;

  return 0 ;
}
