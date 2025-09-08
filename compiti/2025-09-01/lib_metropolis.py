import numpy as np
from iminuit import Minuit
from iminuit.cost import BinnedNLL
from lib import gauss_ort_2D, rand_range_2D, sturges_c, gauss, extract_param_values, covariance
from math import floor, ceil
import random
from scipy.stats import norm
import statistics
import matplotlib.pyplot as plt



def sampler_2D (N_points, func, unif_delta) :

    points = []

    x = rand_range_2D (np.zeros (2), unif_delta)
    points.append (x)
    while len (points) < N_points :
        x = rand_range_2D (points[-1], unif_delta)
        alpha_ratio = func (x) / func (points[-1])
        u = random.random ()
        # print (x, alpha_ratio, u)
        if u > alpha_ratio : continue
        points.append (x)

    return points    


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


def fit_sample (sample, name = 'default') :

    x_min   = floor (min (sample))
    x_max   = ceil (max (sample))
    x_range = (x_min, x_max)
    bin_content, bin_edges = np.histogram (
        sample,
        bins = sturges_c (sample), 
        range = x_range
    ) 

    def fit_model (bin_edges, mu, sigma) :
        return norm.cdf (bin_edges, mu, sigma)

    # # the cost function for the fit
    my_cost_func = BinnedNLL (bin_content, bin_edges, fit_model)

    # the fitting algoritm
    my_minuit = Minuit (
        my_cost_func, 
        mu = statistics.mean (sample), sigma = statistics.stdev (sample),
    )    

    my_minuit.migrad ()
    my_minuit.hesse () 

    
    if not my_minuit.valid : print ('ATTENZIONE fit fallito')

    print ('\nFITTING with label ', name)
    print (my_minuit)

    plt.rc ('figure', figsize = (8, 8))
    fig, ax = plt.subplots ()
    ax.hist (sample, bins = sturges_c (sample), histtype='step', density = True, label = 'sample')
    ax.plot (
        np.linspace (x_min, x_max, 1000),
        gauss (np.linspace (x_min, x_max, 1000), my_minuit.values['mu'], my_minuit.values['sigma']),
        label = 'fit',
    )    
    ax.legend ()
 
    plt.savefig ('fit_' + name + '.png')
    print (my_minuit.values)
    return extract_param_values (my_minuit)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
