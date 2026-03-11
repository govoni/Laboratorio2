import numpy as np

def integra_tr (f, min_x, max_x, N_bins) :
    estremi = np.linspace (min_x, max_x, N_bins+1)
    f_estremi = f (estremi)
    integrale = 0.
    for i in range (N_bins) :
      integrale += f_estremi[i] + f_estremi[i+1]
    integrale *= 0.5 * (estremi[1] - estremi[0])
    return integrale    


def integra_tr_1 (f, min_x, max_x, N_bins) :
    estremi = np.linspace (min_x, max_x, N_bins+1)
    integrale = 0.
    for i in range (N_bins) :
      integrale += f (estremi[i]) + f (estremi[i+1]) 
    integrale *= 0.5 * (estremi[1] - estremi[0])
    return integrale


def integra_tr_2 (f, min_x, max_x, N_bins) :
    estremi = np.linspace (min_x, max_x, N_bins+1)
    f_estremi = f (estremi)
    integrale = 2 * f_estremi.sum () - f_estremi[0] - f_estremi[-1]
    integrale *= 0.5 * (estremi[1] - estremi[0])
    return integrale        