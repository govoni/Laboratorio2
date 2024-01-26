import math
import numpy as np
from random import uniform
from scipy.stats import norm


def pdf (x) :
  # if (x < 0) : return 0
  # if (x > 1.5 * math.pi) : return 0
  return np.cos (x)**2


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def genera_pdf (xMin, xMax, yMin, yMax):
  num = 0
  x = uniform (xMin, xMax)
  y = uniform (0, yMax)
  num += 1
  while (y > pdf (x)) :
    x = uniform (xMin, xMax)
    y = uniform (0, yMax)
    num += 1
  return x, num  


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def genera (N) :
  campione = []
  xMin = 0 
  xMax = 1.5 * np.pi
  yMin = 0
  yMax = 1
  num = 0
  while (len (campione) < N):
    x, count = genera_pdf (xMin, xMax, yMin, yMax)
    campione.append (x)
    num += count
  area = (yMax - yMin) * (xMax - xMin) * len (campione) / num
  return campione, area


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges (N) :
     return int( np.ceil( 1 + 3.322 * np.log(N) ) )


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def rand_TCL (N_sum = 10) :
  '''
  generazione di un numero pseudo-casuale 
  con il metodo del teorema centrale del limite
  su un intervallo fissato
  '''
  xMin = 0 
  xMax = 1.5 * np.pi
  yMin = 0
  yMax = 1
  y = 0.
  for i in range (N_sum) :
      y = y + genera_pdf (xMin, xMax, yMin, yMax)[0]
  y /= N_sum ;
  return y ;


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


# the fitting function
def mod_gaus (bin_edges, mu, sigma):
    return norm.cdf (bin_edges, mu, sigma)


