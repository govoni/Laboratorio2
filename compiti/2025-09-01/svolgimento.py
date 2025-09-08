import numpy as np
from lib import gauss_ort_2D, rand_range_2D, sturges_c, gauss, extract_param_values, covariance
from lib_metropolis import fit_sample, sampler_2D
import matplotlib.pyplot as plt


def main () :

    N_points = 100000
    mean = np.array ([1., 0.5])
    sigma = np.array ([2., 1.2])
    unif_delta = 8 * sigma
   
    func = lambda x : gauss_ort_2D (x, mean, sigma)
    points = sampler_2D (N_points, func, unif_delta)

    plt.rc ('figure', figsize = (16, 12))
    fig, ax = plt.subplots (3,1)

    ax[0].hist2d (
        [x[0] for x in points], 
        [x[1] for x in points], 
        bins = sturges_c (points)
    )
  
    sample_x = [x[0] for x in points]
    sample_y = [x[1] for x in points]

    ax[1].hist (sample_x, bins = sturges_c (points))
    ax[2].hist (sample_y, bins = sturges_c (points))

    plt.savefig ('distribution.png')

    values_x = fit_sample (sample_x, 'x')
    values_y = fit_sample (sample_y, 'y')

    print (values_x)
    print (values_y)

    print (covariance (points))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == '__main__' :

    main ()  