#ifndef toyMC_h
#define toyMC_h

#include <iostream>
#include <random>

class toyMC
{
public:
  toyMC  (); // Default constructor: inizializza le variabili private a 0, il puntatore conteggi_p a NULL
  toyMC  (const int& N, const int& mu); // Constructor
  ~toyMC (); // Destructor

  double Media       ();
  double Varianza    ();
  int    Nestrazioni ();
  int    Estrazione  (const int& i);
  void   Estrazioni  (const int& N, const int& mu);
  void   Clear       ();

private:
  int N_p;
  int mu_p;
  double media_p;
  double varianza_p;
  int* conteggi_p;
  std::default_random_engine generator;
};

#endif
