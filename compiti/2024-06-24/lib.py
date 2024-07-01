import random
import math
import numpy as np


def rand_range (xMin, xMax) :
    return xMin + random.random () * (xMax - xMin)


def try_and_catch_exp (lamb, N):
    events = []
    x_max = 3/lamb
    for i in range (N):
      x = rand_range (0., x_max)
      y = rand_range (0., lamb)
      while (y > lamb * math.exp (-lamb * x)):
        x = rand_range (0., x_max)
        y = rand_range (0., lamb)
      events.append (x)
    return events


def try_and_catch_gau (mean, sigma, N):
    events = []
    for i in range (N):
      x = rand_range (mean - 3 * sigma, mean + 3 * sigma)
      y = rand_range (0., 1.)
      while (y > math.exp (-0.5 * ( (x - mean)/sigma)**2)):
        x = rand_range (mean - 3 * sigma, mean + 3 * sigma)
        y = rand_range (0, 1.)
      events.append (x)
    return events


def sturges (N_events) :
     return int( np.ceil( 1 + 3.322 * np.log (N_events) ) )

