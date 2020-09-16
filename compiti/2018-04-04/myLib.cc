#include "myLib.h"

double myGauss(double* x, double* par)
{
  double arg = 0;
  if (par[2] != 0)
    {
      arg = (x[0] - par[1])/par[2];
    }
  
  return par[0]*exp(-arg*arg/2);
}

bool readData(std::string fileName, std::vector<double>& valVec, std::vector<double>& errVec)
{
  std::fstream myFile;
  
  myFile.open(fileName.c_str(),std::ios::in);
  if (myFile.good() == false)
    {
      return false;
    }
  
  double val, err;
  while (true)
    {
      myFile >> val >> err;
      if (myFile.eof() == true) break;
      valVec.push_back(val);
      errVec.push_back(err);
    }
  myFile.close();
  
  return true;
}

double computeAvg(std::vector<double>& valVec)
{
  double average = 0;
  for (unsigned int i = 0; i < valVec.size(); i++)
    {
      average += valVec[i];
    }
  average /= valVec.size();
  
  return average;
}
