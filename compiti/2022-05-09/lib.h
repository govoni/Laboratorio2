#ifndef lib_h
#define lib_h

#include <vector>
#include <string>
class TH1F ;
class TF1 ;

double f_sin (double * x, double * p) ;
int leggiFile (char * filename, std::vector<double> & data_x, std::vector<double> & data_y) ;
double findAbsMax (std::vector<double> & data_y) ; 
TH1F * calcolaScarti (TF1 & tf1_sin, std::vector<double> const & data_x, std::vector<double> const & data_y) ;
TH1F * riempiHisto (std::vector<double> input, const std::string & histoName) ;



#endif