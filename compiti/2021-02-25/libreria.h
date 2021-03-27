#ifndef libreria_h
#define libreria_h

#include <vector>
#include "TF1.h"

double rand_range (double min, double max) ;
double fgaus0 (double x, double sigma) ;
double rand_TAC_gaus0 (double sigma) ;
double get_max_abs (const std::vector<double> & v) ;
void ordina_vettori (const std::vector<double> & v_x, const std::vector<double> & v_y) ;
int conta_cambi_segno (const std::vector<double> & v) ;
double get_max_scarto_residuo (const std::vector<double> & v_x, const std::vector<double> & v_y, TF1 & fitting_function) ;

#endif