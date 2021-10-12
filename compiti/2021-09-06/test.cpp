/*
c++ -o test test.cpp matrici.cc
*/

#include "matrici.h"


int main () 
{
  matrice2x2 m1 ;
  m1.print () ;
  matrice2x2 m2 (1., 2., 3., 4.) ;
  m2.print () ;
  matrice2x2 m3 (M_PI * 0.25) ;
  m3.print () ;

  vettore2 v1 ;
  v1.print () ;
  vettore2 v2 (1., 0.) ;
  v2.print () ;
  vettore2 v3 = m1 * v2 ;
  v3.print () ;
  vettore2 v4 = m3 * v2 ;
  v4.print () ;
}