import numpy as np
from lib import rand_range

def calc_integral (N_tot, N_under, area):
    prob = (N_under / N_tot)
    result = area * prob
    varianza = area **2 * prob * (1-prob) / N_tot
    errore = np.sqrt (varianza)
    return (result, errore)


def integral (func, x_m, x_M, N_tot):
    y_M = func (x_m)
    x_s = np.array ([rand_range (x_m, x_M) for i in range (N_tot)])
    y_s = np.array ([rand_range (0., y_M) for i in range (N_tot)])
    mask = y_s < func (x_s)
    return calc_integral (N_tot, len (x_s[mask]), (x_M - x_m) * y_M)
    
