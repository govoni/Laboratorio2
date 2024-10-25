import random
import math
import numpy as np



# def inv_exp (y, lamb = 1) :
#     '''
#     Inverse of the primitive of the exponential PDF.
#     pdf(x) = lambda * exp(-lambda x) x >= 0, 0 otherwise.
#     F(x) = int_{0}^{x} pdf(x)dx = 1 - exp(-lambda * x) for x >= 0, 0 otherwise.
#     F^{-1}(y) = - (ln(1-y)) / lambda
#     '''
#     return -1 * np.log (1-y) / lamb


# def generate_exp (lamb = 1, N = 1) :
#     randlist = []
#     for i in range (N):
#         randlist.append (inv_exp (random.random (), lamb))
#     return randlist


def generate_gaus_bm () :
    X1 = random.random ()
    X2 = random.random ()
    G1 = np.sqrt (-2 * np.log (X1)) * np.cos (2 * np.pi * X2)
    G2 = np.sqrt (-2 * np.log (X1)) * np.sin (2 * np.pi * X2)
    return G1, G2


# def rand_range (xMin, xMax) :
#     return xMin + random.random () * (xMax - xMin)


def sturges (N_events) :
    return int( np.ceil( 1 + 3.322 * np.log (N_events) ) )















