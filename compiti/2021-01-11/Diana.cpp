#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "TApplication.h"
#include <vector>
#include "impulso.h"
using namespace std;

// c++ -o Diana impulso.cc `root-config --cflags --glibs`   Diana.cpp

int main(){
	TApplication myApp("App", NULL, NULL);
	TCanvas *c1=new TCanvas();
	c1->Divide(2,1);
   
	int Npoints=1000;
	int pretrigger=100;
	double rise=30.;
	double decay=200.;
	double ampiezza=1000.;
	double rmsnoise=ampiezza/1500.;

	impulso pulse1("pulse.txt");
	impulso ptemplato("template.txt");
	
	cout<<"Dati Template"<<endl;
	ptemplato.Stampa();
	cout<<"Max "<<ptemplato.Maximum()<<endl;
	cout<<"**************************"<<endl;
	cout<<"Dati Segnale"<<endl;
	pulse1.Stampa();
	int i_max;
	double p_max;
	pulse1.GetMaximum(i_max,p_max);
	double noise1=pulse1.RMSNoise();
	
	
	TGraphErrors *gr1=new TGraphErrors(Npoints);
	for(int i=0;i<Npoints;i++) { 
		gr1->SetPoint(i,i,pulse1.GetPoint(i));
		gr1->SetPointError(i,0,noise1);
	}
	
	TGraph *gr2=new TGraph(Npoints);
	for(int i=0;i<Npoints;i++) gr2->SetPoint(i,i,ptemplato.GetPoint(i));
	c1->cd(1);
	gr1->Draw("AL");
	c1->cd(2);
	gr2->Draw("AL");
	
	
	TF1 *parabola=new TF1("parabola","pol2",i_max-30,i_max+30);
	gr1->Fit("parabola","R");
	cout<<"Pulse1 - Massimo dalla classe "<<p_max<<endl;
	cout<<"Pulse1 - Massimo dal fit "<<parabola->GetMaximum()<<endl;
	double norm1 = pulse1.Maximum()/ptemplato.Maximum();
	double norm2 = parabola->GetMaximum()/ptemplato.Maximum();
	cout<<norm1<<endl;
	cout<<norm2<<endl;
	
	cout<<"Chi-quadro  1 "<<pulse1.Confronta( ptemplato, norm1 )<<endl;
	
	cout<<"Chi-quadro  2 "<<pulse1.Confronta( ptemplato, norm2 )<<endl;
	
	
	myApp.Run();
	}
