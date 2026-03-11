import random
import numpy as np
from math import sqrt
import matplotlib.pyplot as plt
import iminuit



def rand_range (xMin, xMax) :
    return xMin + random.random () * (xMax - xMin)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def generate_TAC (f, min_x, max_x, max_y, N) :
    campione = []
    count = 0
    while count < N :
        x = rand_range (min_x, max_x)
        y = rand_range (0., max_y)
        while y > f (x) : 
            x = rand_range (min_x, max_x)
            y = rand_range (0., max_y)
        campione.append (x)
        count += 1
    return campione    


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges (N_events) :
    return int(np.ceil ( 1 + 3.322 * np.log2 (N_events)))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges_c (events) :
    return sturges (len (events))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def set_range (m, M, f) :
    d = M-m
    minimo = m - f * d
    massimo = M + f * d
    return (minimo, massimo)    


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sezioneAureaMax (
    g,              # funzione di cui trovare lo zero
    x0,             # estremo dell'intervallo          
    x1,             # altro estremo dell'intervallo         
    prec = 0.0001): # precisione della funzione        
    '''
    Funzione che calcola estremanti
    con il metodo della sezione aurea
    --- x0 ---- x2 --- x3 ---- x1 --->
    '''

    r = 0.618 # = 2/(1+sqrt(5))
    larghezza = abs (x1 - x0)
    x2 = x1 + r * (x0 - x1) 
    g_x2 = g (x2)
    x3 = x0 + r * (x1 - x0)
    g_x3 = g (x3)

    while (larghezza > prec):

        # inspection of the algorithm      
        # print (f'{x0:.2f} {x2:.2f} {x3:.2f} {x1:.2f} > {g_x2:.6g} {g_x3:.6g} {(g_x2>g_x3)}')

        # si restringe l'intervallo tenendo fisso uno dei due estremi 
        # e spostando l'altro, riciclando più punti e valutazioni 
        # di funzione possibile
        if g_x2 > g_x3 : 
            # x0 = x0
            x1 = x3
            x3 = x2
            g_x3 = g_x2
            x2 = x1 + r * (x0 - x1) 
            g_x2 = g (x2)
        else :
            # x1 = x1
            x0 = x2
            x2 = x3
            g_x2 = g_x3
            x3 = x0 + r * (x1 - x0)
            g_x3 = g (x3)
        larghezza = abs (x1 - x0)             
                                   
    return (x0 + x1) / 2. 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def bisezione (f, x_min, x_max, precisione = 0.001) :
    media = 0.5 * (x_min + x_max)
    while x_max - x_min > precisione :
        if f (media) * f (x_min) > 0 : x_min = media
        else                         : x_max = media
        media = 0.5 * (x_min + x_max)
    return media
