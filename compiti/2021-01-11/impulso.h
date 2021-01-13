#ifndef impulso_h
#define impulso_h

#include <vector>

class impulso {
  public:

    //ctor
    impulso (const char *filename) ;
    ~impulso ()  ;
    
    //metodi
    void   Stampa(); 
    double GetPoint(int i);
    void   GetMaximum(int&i_massimo, double&x_massimo);
    double Maximum();
    double RMSNoise();
    int    GetRiseTime();
    int    GetDecayTime();
    double Confronta(const impulso & riferimento, double norm);
  
  private:

    //variabili private
    int npoints_p;
    int npretr_p;
    std::vector<double> campionamenti_p;
};
#endif

