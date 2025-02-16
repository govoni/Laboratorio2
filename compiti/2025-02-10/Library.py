import numpy as np
import math
from scipy.special import factorial


def fattoriale (x):
  if (int (x) <= 2): return 2
  return int (x) * fattoriale (x-1)


def Poisson(x,Lambda):
  """
  Function returning poisson distribution
  Works for single number only

  Parameters:
  - x: number of expected events
  - lambda: expectation value of poisson
  """
  #x must be integer
#  return (Lambda**int(x))*math.exp(-Lambda)/math.factorial(x)
  return (Lambda**int(x))*math.exp(-Lambda)/fattoriale(x)


def LogLikelihoodPoisson (Lambda, Counts) :
  return np.sum (np.log (np.array ([Poisson (int (k),Lambda) for k in Counts])))


def ExpDistribution(x,Lambda):
  return Lambda*np.exp(-x*Lambda)

def InvExp(y, Lambda = 1) :
  '''
  Inverse of the primitive of the exponential PDF.
  pdf(x) = lambda * exp(-lambda x) x >= 0, 0 otherwise.
  F(x) = int_{0}^{x} pdf(x)dx = 1 - exp(-lambda * x) for x >= 0, 0 otherwise.
  F^{-1}(y) = - (ln(1-y)) / lambda
  '''
  return -1 * np.log (1-y) / Lambda

def GenExp(Lambda,howmanynumbers=1):
  '''
  Generate a uniform number
  pipe it to the inverse exponential cumulant
  '''
  return InvExp (np.random.random (howmanynumbers),Lambda)


def GeneratePoisson(Lambda,nevents):
  Counts = []
  t0=0
  for evento_i in range(nevents):
    #controllo quanti intervalli ci sono 
    t = t0+GenExp(Lambda,1)
    count = 0
    while t<1: 
      count = count+1
      t=t+GenExp(Lambda,1)
    Counts.append(count)
  return np.array(Counts)



def GetEventToy(NumeroDiFinestre,Rate,Finestra):
  Counts = []
  t0=0
  for finestra_i in range(NumeroDiFinestre):
    #controllo quanti intervalli ci sono 
    t = t0+GenExp(Rate*Finestra,1)
    count = 0
    while t<Finestra: 
      count = count+1
      t=t+GenExp(Rate*Finestra,1)
    Counts.append(count)
  return np.array(Counts)


def sezioneAureaMax (
    g,              # funzione di cui trovare lo zero
    x0,             # estremo dell'intervallo          
    x1,             # altro estremo dell'intervallo         
    prec = 0.0001): # precisione della funzione        
    '''
    Funzione che calcola estremanti
    con il metodo della sezione aurea
    '''

    r = 0.618
    x2 = 0.
    x3 = 0. 
    larghezza = abs (x1 - x0)
    x2 = x0 + r * (x1 - x0) 
     
    while (larghezza > prec):
        x3 = x0 + (1. - r) * (x1 - x0)  
      
        # si restringe l'intervallo tenendo fisso uno dei due estremi,
        # spostando l'altro e riciclando uno dei due punti generati
        if (g (x3) < g (x2)): 
            x0 = x1
            x1 = x3
        else :
            x1 = x2
            x2 = x3            
        larghezza = abs (x1-x0)             
                                   
    return (x0 + x1) / 2. 



def bisezione (
    g,              # funzione di cui trovare lo zero
    xMin,           # minimo dell'intervallo          
    xMax,           # massimo dell'intervallo         
    prec = 0.0001): # precisione della funzione        
    '''
    Funzione che calcola zeri
    con il metodo della bisezione
    '''
    xAve = xMin 
    while ((xMax - xMin) > prec) :
        xAve = 0.5 * (xMax + xMin) 
        if (g (xAve) * g (xMin) > 0.): xMin = xAve 
        else                         : xMax = xAve 
    return xAve 


def sturges (N_events) :
     return int (np.ceil (1 + np.log2 (N_events)))
