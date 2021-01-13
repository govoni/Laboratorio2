#include "impulso.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;
//------------------------------------    

impulso::impulso (const char *filename) {

    ifstream input_file ; 
    input_file.open (filename, ios::in) ;
    input_file >> npoints_p;
    input_file >> npretr_p;

    double tempo ;
    for(int i=0 ; i<npoints_p ; i++)  
      {
        input_file >> tempo ;
        campionamenti_p.push_back (tempo);
      }  
    input_file.close () ;
    }


impulso::~impulso () {} 


//metodi

void impulso::Stampa(){
    cout<<"Numero di punti finestra campionata "<<npoints_p<<endl;;
    cout<<"Numero punti sul pre-trigger "<<npretr_p<<endl;
    cout<<"Numero punti rise-time "<<this->GetRiseTime()<<endl;
    cout<<"Numero punti decay-time "<<this->GetDecayTime()<<endl;
    cout<<"Noise RMS "<<this->RMSNoise()<<endl;
    double x_massimo;
    int i_massimo;
    this->GetMaximum(i_massimo, x_massimo);
    cout<<"Posizione e valore del Massimo "<<i_massimo<<" , "<<x_massimo<<endl;
    return;
    }
    

double impulso::GetPoint(int i) {return campionamenti_p[i];}

int impulso::GetRiseTime() {
    double x_max;
    int i_max, i30, i90;
    this->GetMaximum(i_max,x_max);
    while(1) {
        if(campionamenti_p.at (i_max)<x_max*.9) break;
        i_max--;
        }
    i90=i_max;
    while(1) {
        if(campionamenti_p.at (i_max)<x_max*.3) break;
        i_max--;
        }    
    i30=i_max;    //~ cout<<"Punto a 30 "<<campionamenti_p[i_max]<<" "<<i_max<<endl;
    return i90-i30;
    }
    
int impulso::GetDecayTime() {
    double x_max;
    int i_max,i30,i90;
    // vector<double>::iterator itMax = max_element (campionamenti_p.begin (), campionamenti_p.end ()) ;
    // cout << "DEBUG " << itMax - campionamenti_p.begin () << " " << *itMax << endl ;
    this->GetMaximum(i_max,x_max);
    // cout << "DEBUG " << i_max << " " << x_max << endl ;
    while(1) {
        if(campionamenti_p[i_max]<x_max*.9) break;
        i_max++;
        }
    i90=i_max;
    while(1) {
        if(campionamenti_p[i_max]<x_max*.3) break;
        i_max++;
        }    
    i30=i_max;
    return i30-i90;
    }    

double impulso::RMSNoise() { 
    double sum=0, sumq=0; 
    for(int i=0;i<npretr_p;i++) { 
        sum+=campionamenti_p[i];
        sumq+=campionamenti_p[i]*campionamenti_p[i];
        }
        return sqrt(sumq/npretr_p - sum*sum/npretr_p/npretr_p);
    } 
    
void impulso::GetMaximum(int & i_massimo, double & x_massimo) {
    x_massimo=0;
    i_massimo=0;
    for (int i=0;i<npoints_p;i++){
        if (campionamenti_p[i]>x_massimo) {
            x_massimo=campionamenti_p[i]; 
            i_massimo=i;
            }
        }
    return;
    }

double impulso::Maximum() {
    double x_max;
    int i_max;
    this->GetMaximum(i_max,x_max);
    return x_max;
    }
    
double impulso::Confronta(const impulso & riferimento, double norm) {
    double scarti=0;
    double varianza=pow(this->RMSNoise()*norm,2);
    for (int i=0;i<npoints_p;i++){
        scarti+=pow((campionamenti_p[i]-riferimento.campionamenti_p[i]*norm),2);
        }
    return scarti/varianza/npoints_p;
    }
