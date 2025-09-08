import random
import numpy as np
from math import sqrt
import matplotlib.pyplot as plt
import iminuit


def gauss (x, mean, sigma) :
    return 0.3989422804014327 * np.exp (-0.5 * ((x-mean)/sigma)**2) / sigma


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def rand_range (xMin, xMax) :
    return xMin + random.random () * (xMax - xMin)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def gauss_ort_2D (x, mean, sigma) :
    '''
    works w/ np.arrays
    '''
    return gauss (x[0], mean[0], sigma[0]) * \
           gauss (x[1], mean[1], sigma[1])


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def rand_range_2D (center, delta) :
    '''
    works w/ np.arrays
    '''
    return np.array ([
        rand_range (center[0]-delta[0], center[0]+delta[0]),
        rand_range (center[1]-delta[1], center[1]+delta[1])
    ])

    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def plot_fit (x_coord, y_coord, y_sigma, fit_function, minuit_object, title = '') :
    """
    Plots the data and the best-fit function using iminuit fit results.
    
    Parameters:
    - x_coord: array of x values (data)
    - y_coord: array of y values (data)
    - y_sigma: aray of y uncertainties (data)
    - fit_function: the model function used for fitting
    - minuit_object: the iminuit Minuit instance after fitting
    """
    fig, ax = plt.subplots ()
    ax.errorbar (x_coord, y_coord, xerr = 0.0, yerr = y_sigma, linestyle = 'None', marker = 'o') 

    x_fit = np.linspace (x_coord[0], x_coord[-1], 1000)
    best_params = {key: minuit_object.values[key] for key in minuit_object.parameters}
    y_fit = fit_function (x_fit, **best_params)  # Evaluate function with best-fit params
    ax.plot (x_fit, y_fit, label="Best Fit", color="red", linewidth=2)
    ax.set_xlabel ("x")
    ax.set_ylabel ("y")
    ax.legend ()
    ax.set_title (title)

    plt.show ()


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges (N_events) :
    return int(np.ceil ( 1 + 3.322 * np.log (N_events)))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def sturges_c (events) :
    return int (np.ceil ( 1 + 3.322 * np.log (len (events))))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def extract_param_values (my_minuit) :
    result = {}
    for name in my_minuit.parameters:
        val = my_minuit.values[name]   # best‑fit value
        err = my_minuit.errors[name]   # 1‑sigma uncertainty
        result[name] = (val, err)
    return result


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def covariance (sample) :
    sum_x = 0
    sum_y = 0
    sum_xy = 0
    for e in sample :
        sum_x  += e[0]
        sum_y  += e[1]
        sum_xy += e[0] * e[1]

    return sum_xy / len (sample) - sum_x * sum_y / (len (sample) **2)



