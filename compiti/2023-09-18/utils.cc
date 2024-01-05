#include "utils.h"

using namespace std ;

double f_cauchy (double x, double M, double Gamma)
{
  // 0.3183098862 =  1 / pi
  return 0.3183098862 * Gamma / ((x - M) * (x - M) + Gamma * Gamma) ; 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int sturges (int N) 
{
  return floor (1 + 3.322 * log (N)) ;
}


