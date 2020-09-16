#ifndef Dataset_h
#define Dataset_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

template<class T>
class Dataset
{
public:
  Dataset (); // Inizializza le variabili private a 0
  Dataset (const std::string& fileName);
 
  double Media  ()                      const;
  double DevStd ()                      const;
  int    Ndati  ()                      const;
  T      Dato   (const unsigned int& i) const;

private:
  double media_p;
  double devstd_p; 

  std::vector<T> dati_p;
};

#include "Dataset.cc"
#endif
