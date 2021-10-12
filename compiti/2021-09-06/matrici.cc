#include "matrici.h"
#include <iostream>

using namespace std ;

matrice2x2::matrice2x2 (double a11, double a12, double a21, double a22) 
  {
    m_mtr[0][0] = a11 ;
    m_mtr[0][1] = a12 ;
    m_mtr[1][0] = a21 ;
    m_mtr[1][1] = a22 ;
  }

matrice2x2::matrice2x2 (double angolo)
  {
    m_mtr[0][0] = cos (angolo) ;
    m_mtr[0][1] = -1 * sin (angolo) ;
    m_mtr[1][0] = -1 * m_mtr[0][1] ;
    m_mtr[1][1] = m_mtr[0][0] ;
  }

matrice2x2 matrice2x2::operator* (const matrice2x2 & seconda) const 
  {
    matrice2x2 risultato (
      this->elem (0,0) * seconda.elem (0,0) + this->elem (0,1) * seconda.elem (1,0),
      this->elem (0,0) * seconda.elem (0,1) + this->elem (0,1) * seconda.elem (1,1),
      this->elem (1,0) * seconda.elem (0,0) + this->elem (1,1) * seconda.elem (1,0),
      this->elem (1,0) * seconda.elem (0,1) + this->elem (1,1) * seconda.elem (1,1)
    ) ;
    return risultato ;
  }

void matrice2x2::print () const 
  {
    cout << m_mtr[0][0] << "\t" << m_mtr[0][1] << "\n" ;
    cout << m_mtr[1][0] << "\t" << m_mtr[1][1] << "\n" ;
  }


vettore2::vettore2 (double a, double b)
  {
    m_vtr[0] = a ;
    m_vtr[1] = b ;
  }

void vettore2::print () const 
  {
    cout << m_vtr[0] << "\n" << m_vtr[1] << "\n" ;
  }


vettore2 operator* (const matrice2x2 & prima, const vettore2 & secondo)
  {
    vettore2 risultato (
      prima.elem (0,0) * secondo.elem (0) + prima.elem (0,1) * secondo.elem (1),
      prima.elem (1,0) * secondo.elem (0) + prima.elem (1,1) * secondo.elem (1)
    ) ;
    return risultato ;
  }

