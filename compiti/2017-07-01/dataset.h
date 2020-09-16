#ifndef dataset_h
#define dataset_h
 
#include <string>
#include <vector>

using namespace std;
 
class dataset {
 
        public:
        dataset(); //inizializza le variabili private a 0
    
        dataset(const string filNam);
    
        ~dataset();
 
        double Media ();
        double Devstd ();
        int Ndati ();
        int Dato(const int& i);
 
        private:
		int N_p;
		int mu_p;
		vector <int> dati_p;
		double media_p;
		double devstd_p;
 
};
 
#endif

