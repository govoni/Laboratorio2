/*
c++ -o main main.cpp logisticLib.cc `root-config --cflags --glibs`
*/

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"

#include <iostream>

#include "logisticLib.h"

#define MAXITERATIONS 100

int main(int argc, char** argv)
{
  if (argc < 3)
    {
      std::cout << "Argument missing. Synopsis:" << std::endl;
      std::cout << "./main n_step_x n_step_a" << std::endl;
      exit(EXIT_FAILURE);
    }

  int NscanX = atoi(argv[1]);
  int NscanA = atoi(argv[2]);

  TGraph myGraph1;
  double xStep = 1. / NscanX;
  double aStep = MAX_A / NscanA;

  for (int i = 0; i < NscanX; i++)
    for (int j = 0; j < NscanA; j++)
      myGraph1.SetPoint(myGraph1.GetN(), j * aStep, recurrentLogistic(MAXITERATIONS, i * xStep, j * aStep));

  TApplication myApp("myApp", NULL, NULL);

  TCanvas c1("c1", "Chaos", 0, 0, 700, 500);
  c1.cd();
  myGraph1.GetXaxis()->SetTitle("a");
  myGraph1.GetYaxis()->SetTitle("x");
  myGraph1.SetMarkerStyle(20);
  myGraph1.SetMarkerSize(0.1);
  myGraph1.Draw("AP");
  c1.Modified();
  c1.Update();

  TGraph myGraph2;
  for (int i = 1; i <= NscanX; i++)
    myGraph2.SetPoint(myGraph2.GetN(), i, recurrentLogistic(i, 0.01, 3));

  TCanvas c2("c2", "Chaos", 0, 0, 700, 500);
  c2.cd();
  myGraph2.GetXaxis()->SetTitle("x");
  myGraph2.GetYaxis()->SetTitle("logistic");
  myGraph2.Draw("AL");
  c2.Modified();
  c2.Update();

  std::cout << "=== DONE ===" << std::endl;
  myApp.Run();
  return 0;
}
