import numpy as np
import matplotlib.pyplot as plt
from lib import pdf, genera, sturges, rand_TCL
from stats import stats


if __name__ == '__main__':

  # draw the pdf
  # ---- ---- ---- ---- ---- ---- ---- 

  fig, ax = plt.subplots (nrows = 1, ncols = 1)

  # preparing the set of points to be drawn 
  x_coord = np.linspace (0, 1.5 * np.pi, 10000)
  y_coord_1 = pdf (x_coord)

  # visualisation of the image
  ax.plot (x_coord, y_coord_1, label='pdf')
  ax.set_title ('probability density function', size=14)
  ax.set_xlabel ('x')
  ax.set_ylabel ('y')
  plt.savefig ('pdf.png')
#  plt.show ()

  # generate the sample and calculate the integral
  # ---- ---- ---- ---- ---- ---- ---- 

  campione, area = genera (10000)

  # draw the histogram of the sample
  # ---- ---- ---- ---- ---- ---- ---- 

  ax.set_title ('generated sample', size=14)
  print ('generati', len (campione),'eventi')
  print ("l'area della pdf prima della normalizzazione è",area)
  print ('il fattore di normalizzazione è', 1./area)
  N_bins = sturges (len (campione))
  bin_edges = np.linspace (0, 1.5 * np.pi, N_bins)
  ax.hist (campione,
           bin_edges,
           color = 'orange',
          )
  plt.savefig ('histo.png')

  # calculate moments
  # ---- ---- ---- ---- ---- ---- ---- 

  my_stats = stats (campione)
  print ('mean    :', my_stats.mean ())
  print ('sigma   :', my_stats.sigma ())
  print ('skewness:', my_stats.skewness ())
  print ('kurtosis:', my_stats.kurtosis ())

  # study the Gaussian behaviour
  # ---- ---- ---- ---- ---- ---- ---- 

  N_events = 10000
  means = []
  sigmas = []
  skews = []
  kurts = []
  x_axis = [2**j for j in range(0,6)]
  for N_sum in x_axis:
    campione_loc = [rand_TCL (N_sum) for j in range (N_events)]
    my_stats = stats (campione_loc)
    means.append (my_stats.mean ())
    sigmas.append (my_stats.sigma ())
    skews.append (my_stats.skewness ())
    kurts.append (my_stats.kurtosis ())

  fig, ax = plt.subplots (nrows = 4, ncols = 1)
  ax[0].plot (x_axis, means, label='mean')
  ax[1].plot (x_axis, sigmas, label='sigma')
  ax[2].plot (x_axis, skews, label='skewness')
  ax[3].plot (x_axis, kurts, label='kurtosis')
  plt.savefig ('stats.png')

  campione_gaus = [rand_TCL (32) for j in range (N_events)]

  fig, ax = plt.subplots (nrows = 1, ncols = 1)
  N_bins = sturges (len (campione_gaus))
  bin_edges = np.linspace (0, 1.5 * np.pi, N_bins)
  bin_content, _, _ = ax.hist (campione_gaus,
           bin_edges,
           color = 'orange',
          )
  plt.savefig ('gauss.png')

  from iminuit import Minuit
  from iminuit.cost import BinnedNLL
  from scipy.stats import norm
  from lib import mod_gaus

  my_stats_gaus = stats (campione_gaus)

  # the cost function for the fit
#  my_cost_func = BinnedNLL (bin_content, bin_edges, gaus_model)
  my_cost_func = BinnedNLL (bin_content, bin_edges, mod_gaus)

  my_minuit = Minuit (my_cost_func, 
                      mu = my_stats_gaus.mean (), 
                      sigma = my_stats_gaus.sigma ())

  my_minuit.migrad ()
  my_minuit.minos ()
  print (my_minuit.valid)
  from scipy.stats import chi2
  print ('associated p-value: ', 1. - chi2.cdf (my_minuit.fval, df = my_minuit.ndof))
  if 1. - chi2.cdf (my_minuit.fval, df = my_minuit.ndof) > 0.10:
    print ('the event sample is compatible with a Gaussian distribution')

