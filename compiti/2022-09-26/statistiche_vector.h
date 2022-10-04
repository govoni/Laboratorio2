#ifndef statistiche_vector_h
#define statistiche_vector_h

#include <vector>

template <class T>
T media (const std::vector<T> & input_v) 
{
  T somma = 0 ;
  for (int i = 0 ; i < input_v.size () ; ++i) somma += input_v.at (i) ;
  return somma / static_cast<float> (input_v.size ()) ;
}


template <class T>
T varianza (const std::vector<T> & input_v) 
{
  T somma = 0 ;
  T sommaSq = 0 ;
  for (int i = 0 ; i < input_v.size () ; ++i) 
    {
      somma += input_v.at (i) ;
      sommaSq += input_v.at (i) * input_v.at (i) ;
    }  
  double N = static_cast<double> (input_v.size ()) ;  
//  return N / (N - 1) *
  return       (sommaSq / N - (somma * somma / (N * N))) ;
}


template <class T>
T sigma (const std::vector<T> & input_v) 
{
  return varianza (input_v) ;
}


#endif