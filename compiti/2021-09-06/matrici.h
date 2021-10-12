#ifndef matrici_h
#define matrici_h

#include <cmath>

class matrice2x2
{
  public:

    matrice2x2 (double a11, double a12, double a21, double a22) ;
    matrice2x2 (double angolo = M_PI * 0.5) ;

    double elem (int i, int j) const { return m_mtr[i][j] ;} ;
    matrice2x2 operator* (const matrice2x2 & seconda) const ;
    void print () const ;

  private:

    double m_mtr[2][2] ;


} ;


class vettore2
{
  public:

    vettore2 (double a = 0., double b = 0.) ;
    double elem (int i) const { return m_vtr[i] ;} ;
    void print () const ;

  private:

    double m_vtr[2] ; 

} ;


vettore2 operator* (const matrice2x2 & prima, const vettore2 & secondo) ;


#endif