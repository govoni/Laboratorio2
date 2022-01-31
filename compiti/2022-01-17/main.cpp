/*
c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <vector>
#include <iostream>

#include "TCanvas.h"
#include "TH1F.h"

#include "lib.h"

using namespace std ;

int main (int argc, char ** argv)
{
  TH1F h_pdf ("h_pdf", "h_pdf", 30, -1., 4.) ; 
  for (int i = 0 ; i < 1000 ; ++i)
    {
      h_pdf.Fill (generaTAC ()) ;
    }  
  TCanvas c1 ;
  h_pdf.Draw ("hist") ;
  c1.Print ("histo.png", "png") ;  

  int n_events = 100 ;

  TH1F h_media ("h_media", "h_media", 100,
                h_pdf.GetMean () - 4. * h_pdf.GetStdDev () / sqrt (n_events), 
                h_pdf.GetMean () + 4. * h_pdf.GetStdDev () / sqrt (n_events) ) ;

  double err_varianza = 1.42 * h_pdf.GetStdDev () * h_pdf.GetStdDev () / sqrt (n_events - 1) ;
  double err_sigma    = err_varianza / (2 * sqrt (n_events) * h_pdf.GetStdDev ()) ;
  cout << err_sigma << endl ;

  TH1F h_sigmaMu ("h_sigmaMu", "h_sigmaMu", 100,
                h_pdf.GetStdDev () / sqrt (n_events) - 4. * err_sigma, 
                h_pdf.GetStdDev () / sqrt (n_events) + 4. * err_sigma) ;

  for (int iToy = 0 ; iToy < 10000 ; ++iToy)
    {
      vector<double> campione ;
      for (int i = 0 ; i < n_events ; ++i)
        {
          campione.push_back (generaTAC ()) ;
        }  
      h_media.Fill (calcMedia (campione)) ;
      h_sigmaMu.Fill (sqrt (calcVarianza (campione) / campione.size ())) ;
    }

  h_media.Draw ("hist") ;
  c1.Print ("media.png", "png") ;  
  h_sigmaMu.Draw ("hist") ;
  c1.Print ("devStdMedia.png", "png") ;  


  return 0 ;
}