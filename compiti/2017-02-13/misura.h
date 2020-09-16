#ifndef misura_h
#define misura_h
 
#include <string>
 
using namespace std;
 
class misura {
 
        public:
        misura();
        misura(const string& filename);
        ~misura();
 
        void Somma (const misura& other);
        void SommaCorr (const misura& other, const double &cov);
        void Moltiplica (const misura& other);
        void Confronta (const misura& other) const;
 
        private:
		double X_p;
		double EX_p;
 
};
 
#endif
