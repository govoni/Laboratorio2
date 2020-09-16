#ifndef myLib_h
#define myLib_h

#include <fstream>
#include <vector>
#include <string>
#include <cmath>

// Funzione che che implementa la distribuzione Gaussiana
double myGauss(double* x, double* par);

// Funzione per la lettura del file di dati (restituisce false se la lettura non e` andata a buon fine, altrimenti true)
bool readData(std::string fileName, std::vector<double>& valVec, std::vector<double>& errVec);

// Funzione che calcola la media di un set di numeri
double computeAvg(std::vector<double>& valVec);

#endif
