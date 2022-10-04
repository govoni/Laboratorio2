#ifndef lib_h
#define lib_h

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include "TF1.h"

class func
{
  public:
    
    func (double xZero) ;
    double eval (double x) ;

  private:  

    double m_xZero ;
    double m_a ;
    double m_b ;
} ;

double rand_range (double min, double max) ;
double rand_TAC (func f, double xZero) ;
std::vector<double> area_TAC (func f, double xZero, int NToys = 1000) ;

double sezioneAurea (TF1 & g, double x0, double x1, double precision = 0.0001) ;

#endif