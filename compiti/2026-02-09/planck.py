import numpy as np
from iminuit import Minuit
from iminuit.cost import ExtendedBinnedNLL
from scipy.stats import norm
from stats import stats
from lib import sturges_c



def u_lam (lam, T) :
    k_B = 1.38e-23 # J/k
    c = 3e8 # m/s
    h = 6.63e-34 #Js
    return 2 * h * c**2 / (lam**5 * (np.exp (h*c/(lam*k_B*T))-1))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


# the fitting function
def mod_total (bin_edges, N_signal, mu, sigma):
    return N_signal * norm.cdf (bin_edges, mu, sigma) 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def find_maximum (sample, model, lam_min, lam_max) :

    N_bins = sturges_c (sample)
    plank_stats = stats (sample)
    media = plank_stats.mean ()
    sigma = plank_stats.sigma ()
    # x_range = (media - 1.5 * sigma, media)
    x_range = (media - 1.5 * sigma, media)
    N_bins_fit = int (N_bins * (x_range[1] - x_range[0]) / (lam_max - lam_min))
    bin_content_fit, bin_edges_fit = np.histogram (sample, bins = N_bins_fit, range = x_range)
    my_cost_func = ExtendedBinnedNLL (bin_content_fit, bin_edges_fit, model)
    my_minuit = Minuit (my_cost_func, 
                        N_signal = len (sample), mu = media, sigma = sigma, # signal input parameters
                       )
    my_minuit.migrad ()
    # my_minuit.minos ()

    return my_minuit.valid, my_minuit.values['mu'], my_minuit.errors['mu'] 