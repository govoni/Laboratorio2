#include "utils.h"

using namespace std ;

int fill_histo (TH1F & histo, vector<double> & eventi)
{
  for (int i = 0 ; i < eventi.size () ; ++i)
    {
      histo.Fill (eventi.at (i)) ;
    }
  return histo.GetEntries () ;
}


int conta_in (std::vector<double> & eventi, double min, double max) 
{
  int numero = 0 ;
  for (int i = 0 ; i < eventi.size () ; ++i)
    {
      if (eventi.at (i) < min) continue ;
      if (eventi.at (i) > max) continue ;
      ++numero ;
    }
  return numero ;
}
