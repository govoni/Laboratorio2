#include "lib.h"
#include <cmath>
//#include <string>
//#include <fstream>
#include <cstdlib>

//#include "TH1F.h"
//#include "TF1.h"

//#include "statistiche_vector.h"

using namespace std ;


func::func (double xZero) :
  m_xZero (xZero)
  {
    m_a = -3. / (4. * pow (xZero, 3)) ;
    m_b = 3. / (4. * xZero) ;
  }

double
func::eval (double x)
{
  if (fabs (x) > m_xZero) return 0. ;
  return m_a * x * x + m_b ;
}

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
  } 

double rand_TAC (func f, double xZero)
  {
    double x = 0. ;
    double y = 0. ; 
    double yMax = f.eval (0.) ;
    do {
      x = rand_range (-1 * xZero, xZero) ;
      y = rand_range (0., yMax) ;
    } while (y > f.eval (x)) ;
    return x ; 
  }

vector<double>
area_TAC (func f, double xZero, int NToys)
  {
    double x = 0. ;
    double y = 0. ; 
    double yMax = f.eval (0.) ;
    int NSotto = 0 ;
    for (int iToy = 0 ; iToy < NToys ; ++iToy)
      {
        x = rand_range (-1 * xZero, xZero) ;
        y = rand_range (0., yMax) ;
        if (y < f.eval (x)) ++NSotto ;
      } 
    vector<double> result ;
    result.push_back (2 * xZero * yMax * NSotto / static_cast<double> (NToys)) ; 
    double p = static_cast<double> (NSotto) / static_cast<double> (NToys) ;
    result.push_back (sqrt (4 * xZero * yMax * xZero * yMax * p * (1-p) / (NToys * NToys))) ; 
    return result ;
  }


double sezioneAurea (TF1 & g, double x0, double x1, double precision)
{
  double r = 0.618;
  double x2 = 0;
  double x3 = 0; 
  double larghezza = fabs (x1-x0) ;
   
  while (larghezza > precision)
    {        
        x2 = x0 + r * (x1 - x0) ;
        x3 = x0 + (1. - r) * (x1 - x0);  
    
      if (g.Eval (x3) < g.Eval (x2) ) 
       {  //restringo l'intervallo tennedo fisso uno dei due estremi e spostando l'altro        
          x0 = x3;
          x1 = x1;         
        }            
      else 
          { //restringo l'intervallo tennedo fisso uno dei due estremi e spopstando l'altro
             x1 = x2;
             x0 = x0;          
          }
          
        larghezza = fabs (x1-x0) ;            
    } 
                                 
  return (x0+x1)/2. ;
}


