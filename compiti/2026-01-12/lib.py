import random
import numpy as np
from math import sqrt
import matplotlib.pyplot as plt
import iminuit



def rand_range (xMin, xMax) :
    return xMin + random.random () * (xMax - xMin)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def inv_exp (y, lamb = 1) :
    '''
    Inverse of the primitive of the exponential PDF.
    pdf(x) = lambda * exp(-lambda x) x >= 0, 0 otherwise.
    F(x) = int_{0}^{x} pdf(x)dx = 1 - exp(-lambda * x) for x >= 0, 0 otherwise.
    F^{-1}(y) = - (ln(1-y)) / lambda
    '''
    return -1 * np.log (1-y) / lamb


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges (N_events) :
    return int(np.ceil ( 1 + 3.322 * np.log2 (N_events)))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges_c (events) :
    return sturges (len (events))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def dEdx (E) :
    A = 0.15 # Mev^2/cm
    if E < 0.01  : return 100 * A
    elif E < 0.5 : return A / E
    else         : return 2 * A    
        
