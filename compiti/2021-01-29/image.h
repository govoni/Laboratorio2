#ifndef image_h
#define image_h

#include <vector>
#include <string>

class TH1F ;
class TH2F ;

class image
{
  public:
    image () ;
    image (const std::string & fileName) ;
    ~image () ;
    
    void readFile (const std::string & fileName) ;
    TH1F * drawSpectrum (const std::string & histoName) ;
    TH2F * drawRawImage (const std::string & histoName) ;
    TH2F * drawImage (const std::string & histoName) ;
    TH1F * fettaYfissato (int ybin, const std::string & histoName) ;
    TH1F * proiezioneSuX (const std::string & histoName) ;
    double varianzaLungoX () ;
    double varianzaLungoY () ;
    double covarianza () ;
    std::vector<double> getPedestal (double min, double max, TH1F * h_spettro) ;
    int at_x (int i) const {return v_x.at (i) ; } ; 
    int at_y (int i) const {return v_y.at (i) ; } ; 
    double at_val (int i) const {return v_val.at (i) ; } ; 
    int size () const { return v_x.size () ; } ;
    int righe () const {return n_righe ; } ;
    int colonne () const {return n_colonne ; } ;
    void subtractPedestal (double value) ;

  private:  

    int n_righe ;
    int n_colonne ;

    std::vector<int> v_x ;
    std::vector<int> v_y ;
    std::vector<double> v_val ;
    std::vector<double> v_val_corr ;

    TH1F * h_fetta ;
    TH1F * h_spettro ;
    TH2F * h_image ;
    TH2F * h_raw_image ;
    TH1F * h_proiezione ;
} ;

#endif