#include "logisticLib.h"

double logistic(double x, double a)
{
  return a * x * (1. - x);
}

double recurrentLogistic(int nIter, double x, double a)
{
  if (nIter > 0)
    x = recurrentLogistic(--nIter, logistic(x, a), a);

  return x;
}
