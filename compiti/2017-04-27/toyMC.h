#ifndef toy_h
#define toy_h
 
#include <string>
 
using namespace std;
 
class toy {
 
        public:
        toy(); //inizializza le variabili private a 0 ,il puntatore conteggi_p a un solo elemento nullo
        toy(const int& N, const int& mu, const int& seed); //ricordarsi di definire la dimensione del puntatore
        ~toy();
 
        double Media ();
        double Varianza ();
        int Nestrazioni ();
        int Estrazione(const int& i);
 
        private:
		int N_p;
		int mu_p;
		int *conteggi_p;
		double media_p;
		double varianza_p;
 
};
 
#endif
