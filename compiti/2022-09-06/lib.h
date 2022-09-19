#ifndef lib_h
#define lib_h

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

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


#endif