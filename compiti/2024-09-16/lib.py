import random
import math
import numpy as np


class additive_recurrence :

    def __init__ (self, alpha = 0.618034) : # (sqrt(5)-1)/2
        self.alpha = alpha
        self.s_0 = 0.5
        self.s_n = 0.5
        
    def get_number (self) :
        self.s_n = (self.s_n + self.alpha) % 1
        return self.s_n

    def set_seed (self, seed) :
        self.s_0 = seed
        self.s_n = seed
   
    def get_numbers (self, N) :
        lista = []
        for i in range (N) : lista.append (self.get_number ())
        return lista


def rand_range (xMin, xMax) :
    return xMin + random.random () * (xMax - xMin)


def MC_mod (N_points) :
    gen_seq = additive_recurrence ()
    sotto = float (0)
    for i in range (N_points):
        x = gen_seq.get_number ()
        y = rand_range (0., 2.)
        if (y < 2 * x * x) : sotto += 1
    frazione = sotto / N_points
    integrale = 2 * frazione
    sigma = 2 * np.sqrt (frazione * (1 - frazione) / N_points)
    return integrale, sigma


def MC_classic (N_points) :
    gen_seq = additive_recurrence ()
    sotto = float (0)
    for i in range (N_points):
        x = rand_range (0., 1.)
        y = rand_range (0., 2.)
        if (y < 2 * x * x) : sotto += 1
    frazione = sotto / N_points
    integrale = 2 * frazione
    sigma = 2 * np.sqrt (frazione * (1 - frazione) / N_points)
    return integrale, sigma


def sturges (N_events) :
    return int( np.ceil( 1 + 3.322 * np.log (N_events) ) )


def integral_CrudeMC (g, xMin, xMax, x_axis) :
    somma  = 0.
    sommaQ = 0.    
    N_rand = len (x_axis)
    for i in range (N_rand) :
       somma += g(x_axis[i])
       sommaQ += g(x_axis[i]) * g(x_axis[i])     
     
    media = somma / float (N_rand)
    varianza = sommaQ /float (N_rand) - media * media 
    varianza = varianza * (N_rand - 1) / N_rand
    lunghezza = (xMax - xMin)
    return media * lunghezza, np.sqrt (varianza / float (N_rand)) * lunghezza
                                         












