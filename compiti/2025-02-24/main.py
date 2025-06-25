import matplotlib.pyplot as plt
import numpy as np
from iminuit import Minuit
from iminuit.cost import LeastSquares
from scipy.stats import chi2
from lib import rand_TCL_ms, plot_fit, sturges, calc_probability
import sys


def model_para (x, a, b, c) :
  return a * x * x + b * x + c


def model_expo (x, a, b, c) :
  return a * np.exp (b * x) + c


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def main () :
  '''main program function'''

  # point 1

  x_coord = np.linspace (0., 10., 10)
  theta_a = 0.5
  theta_b = 0
  theta_c = 1.
  sigma = 1.5
  y_coord = [model_para (x, theta_a, theta_b, theta_c) + rand_TCL_ms (0., sigma) for x in x_coord]
  y_sigma = [sigma for x in y_coord]

  fig, ax = plt.subplots ()
  ax.set_title ('parabolic trend', size=14)
  ax.set_xlabel ('x')
  ax.set_ylabel ('y')
  ax.errorbar (x_coord, y_coord, xerr = 0.0, yerr = y_sigma, linestyle = 'None', marker = 'o') 
  plt.show ()

  # point 2

  # generate a least-squares cost function
  least_squares = LeastSquares (x_coord, y_coord, y_sigma, model_para)
  my_minuit = Minuit (least_squares, a = 0, b = 0, c = 0)
  my_minuit.migrad ()  # finds minimum of least_squares function
  my_minuit.hesse ()   # accurately computes uncertainties

  if my_minuit.valid : 
    print ('parabolic fit successful')
    print ('associated p-value: ', 1. - chi2.cdf (my_minuit.fval, df = my_minuit.ndof)) 
  else : 
    print ('parabolic fit failed')
    sys.exit (1)

  plot_fit (x_coord, y_coord, y_sigma, model_para, my_minuit, 'parabolic fit')

  # point 3

  N_toys = 1000
  Q2_list_para = []
  i_toy = 0
  while i_toy < N_toys :
    y_coord = [model_para (x, theta_a, theta_b, theta_c) + rand_TCL_ms (0., sigma) for x in x_coord]
    least_squares = LeastSquares (x_coord, y_coord, y_sigma, model_para)
    my_minuit = Minuit (least_squares, a = 0, b = 0, c = 0)
    my_minuit.migrad ()  # finds minimum of least_squares function
    if not my_minuit.valid : continue
    Q2_list_para.append (my_minuit.fval)
    i_toy += 1

  x_min = 0
  x_max = np.ceil (max (Q2_list_para))
  N_bins = sturges (len (Q2_list_para))

  bin_edges = np.linspace (x_min, x_max, N_bins)
  h_para, edges_para = np.histogram (Q2_list_para, bins = bin_edges)

  fig, ax = plt.subplots ()
  ax.stairs (h_para,
             color = 'red',
             label = 'parabolic',
            )
  ax.set_title ("Q^2 distributions for parabolic fits")
  ax.set_xlabel ('$Q^2$')
  plt.show ()

  # point 4

  least_squares = LeastSquares (x_coord, y_coord, y_sigma, model_expo)
  my_minuit = Minuit (least_squares, a = 0, b = 0, c = 0)
  my_minuit.migrad ()  # finds minimum of least_squares function

  if my_minuit.valid : print ('fit successful')
  else : 
    print ('exponential fit failed')
    sys.exit (1)

  plot_fit (x_coord, y_coord, y_sigma, model_expo, my_minuit, 'exponential fit')

  Q2_list_expo = []
  i_toy = 0
  while i_toy < N_toys :
    y_coord = [model_para (x, theta_a, theta_b, theta_c) + rand_TCL_ms (0., sigma) for x in x_coord]
    least_squares = LeastSquares (x_coord, y_coord, y_sigma, model_expo)
    my_minuit = Minuit (least_squares, a = 0, b = 0, c = 0)
    my_minuit.migrad ()  # finds minimum of least_squares function
    if not my_minuit.valid : continue
    Q2_list_expo.append (my_minuit.fval)
    i_toy += 1

  fig, ax = plt.subplots ()
  h_expo, edges_para = np.histogram (Q2_list_expo, bins = bin_edges)
  ax.stairs (h_para,
             color = 'red',
             label = 'parabolic',
            )
  ax.stairs (h_expo,
             color = 'blue',
             label = 'exponential',
            )
  ax.set_title ("comparing $Q^2$ distributions")
  ax.set_xlabel ('$Q^2$')
  ax.legend ()
  plt.show ()

  # point 5

  Q2_merger = Q2_list_para + Q2_list_expo
  Q2_merger.sort ()

  prob_para = [calc_probability (Q2_list_para, Q2) for Q2 in Q2_merger]
  prob_expo = [calc_probability (Q2_list_expo, Q2) for Q2 in Q2_merger]

  fig, ax = plt.subplots ()
  ax.plot (prob_expo, prob_para,
             color = 'red',
             label = 'ROC curve',
            )
  ax.set_xlabel ('false positives')
  ax.set_ylabel ('true positives')
  plt.show ()


if __name__ == '__main__':
  main ()