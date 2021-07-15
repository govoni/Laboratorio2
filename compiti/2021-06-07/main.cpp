/*
c++ -o main `root-config --glibs --cflags` main.cpp
./main dati.txt

*/

#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <TF1.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TGraphErrors.h>

using namespace std ;


double f_expo (double * x, double * par)
  {
    return par[0] * exp (-1 * x[0] * par[0]) ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double calcLogLikelihood (double par, vector<double> & dati, int maxNum = -1)
  {
    double logL = 0 ;
    if (maxNum == -1 || maxNum > dati.size ()) maxNum = dati.size () ;
    for (int i = 0 ; i < maxNum ; ++i)
      {
         logL += f_expo (&dati.at (i), &par) ;
      }
    return logL ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double calcLikelihood (double par, vector<double> & dati, int maxNum = -1)
  {
    double like = 1 ;
    if (maxNum == -1 || maxNum > dati.size ()) maxNum = dati.size () ;
    for (int i = 0 ; i < maxNum ; ++i)
      {
         like *= f_expo (&dati.at (i), &par) ;
      }
    return like ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double maxSezioneAurea (double min_x, double max_x, double precision, vector<double> & data, int max_N = -1)
{
  double r = 0.618;
  double x2 = 0;
  double x3 = 0; 
  double larghezza = fabs (max_x - min_x) ;
   
  while (larghezza > precision)
    {        
        x2 = min_x + r * (max_x - min_x) ;
        x3 = min_x + (1. - r) * (max_x - min_x) ;  

      //restringo l'intervallo tennedo fisso uno dei due estremi e spostando l'altro
      if ( calcLikelihood (x3, data, max_N) < calcLikelihood (x2, data, max_N) ) 
        min_x = x3 ;
      else 
        max_x = x2 ;      
      larghezza = fabs (max_x - min_x) ;            
    } 
                                 
  return 0.5 * (min_x + max_x) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{

  // leggere il file di dati e riempire un vector 

  ifstream input_file ; 
  input_file.open (argv[1], ios::in) ;

  vector<double> data ;
  double input_x, input_y ;
  while (true) 
    {
      input_file >> input_x ;
      if (input_file.eof () == true) break ;
      data.push_back (input_x) ;
    } 
  input_file.close () ;

  // disegnare la distribuzione dei dati in un istogramma
  // che abbia l'intervallo scelto in funzione dei eventi raccolti
  // ed il numero di bin determinato con un algoritmo basato sul numero di eventi
  // in modo da permettere una agevole visualizzazione della distribuzione

  TH1F h_data ("h_data", "h_data", 
               0.5 * data.size (),  
               *min_element (data.begin (), data.end ()),
               *max_element (data.begin (), data.end ())
               ) ;

  for (int i = 0 ; i < data.size () ; ++i) h_data.Fill (data.at (i)) ;

  TCanvas c1 ;
  h_data.Draw ("hist") ;
  c1.Print ("input.png", "png") ;

  cout << "stima di k: " << 1. / h_data.GetMean () << endl ;
 
  // si costruisca una funzione di C++ che calcoli la verosimiglianza
  // associata ai dati

  // assumendo che gli eventi seguano una distribuzione esponenziale $f(x) = k e^{kx}$,
  // si faccia un'ipotesi ragionevole sul valore del parametro $k$
  // basata sull'istogramma

  // si scriva un ciclo che disegni in un TGraph l'andamento del valore della verosimiglianza
  // in funzione del valore del parametro $k$,
  // dopo aver scelto un ragionevole passo di campionamento,
  // disegnando il TGraph in un'immagine

  // si utilizzi il metodo della sezione aurea 
  // per deteminare il massimo della verosimiglianza in un intervallo ragionevole di valori 
  // del parametro $k$, 
  // determinato a partire dal TGraph disegnato

  cout << "maximum of the likelihood: " << maxSezioneAurea (0.1, 1., 0.0001, data) << endl ;

  TGraph g_like_best ;
  for (double par = 0.01 ; par < 1 ; par+= 0.01)
    {
      double y = calcLikelihood (par, data) ;
      g_like_best.SetPoint (g_like_best.GetN (), par, y) ;
    }
  g_like_best.Draw ("AL") ;
  c1.Print ("LL_best.png", "png") ;

  TGraph g_like_19 ;
  for (double par = 0.01 ; par < 1 ; par+= 0.01)
    {
      double y = calcLikelihood (par, data, 19) ;
      g_like_19.SetPoint (g_like_19.GetN (), par, y) ;
    }
  g_like_19.Draw ("AL") ;
  c1.Print ("LL_19.png", "png") ;


  // si mostri che il profilo della verosimiglianza in funzione del parametro $k$ 
  // diventa pi\`u stretto all'aumentare del numero di eventi a disposizione

  vector<int> v_max_N ;
  vector<TGraph *> v_like_trends ;
  vector<double> v_x_max ;
  vector<double> v_y_max ;
  vector<string> v_legend ;

  for (int max_N = data.size () ; max_N > 10 ; max_N -= 1)
    {
      TGraph * g_like = new TGraph () ;
      v_x_max.push_back (maxSezioneAurea (0.1, 1., 0.0001, data, max_N)) ;
      v_y_max.push_back (calcLikelihood (v_x_max.back (), data, max_N)) ;

      v_like_trends.push_back (g_like) ;
      v_max_N.push_back (max_N) ;
      stringstream ss ;
      ss << max_N ;
      v_legend.push_back (ss.str ()) ;
      for (double par = 0.01 ; par < 1 ; par+= 0.01)
        {
          double y = calcLikelihood (par, data, max_N) ; // - v_y_max.back () ;
          g_like->SetPoint (g_like->GetN (), par, y) ;
        }
    }

  TLegend legend (0.1,0.7,0.48,0.9) ;
  v_like_trends.at (0)->Draw ("AL") ;
  v_like_trends.at (0)->SetLineWidth (2) ;
  legend.AddEntry (v_like_trends.at (0), v_legend.at (0).c_str (), "l") ;

  for (int i = 1 ; i < v_like_trends.size () ; ++i)
    {
      cout << v_legend.at (i) << ":\t(" << v_x_max.at (i) << "," << v_y_max.at (i) << ")\n" ;
     legend.AddEntry (v_like_trends.at (i), v_legend.at (i).c_str (), "l") ;
      v_like_trends.at (i)->SetLineWidth (2) ;
      v_like_trends.at (i)->SetLineColor (i + 2) ;
      v_like_trends.at (i)->Draw ("L same") ;
    }
  legend.Draw () ;
  c1.Print ("LL.png", "png") ;

  for (int i = 1 ; i < v_like_trends.size () ; ++i) delete v_like_trends.at (i) ;
  
  return 0 ;
}

