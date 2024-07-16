import random
import math
import numpy as np


def rand_range (xMin, xMax) :
    return xMin + random.random () * (xMax - xMin)


def try_and_catch_step (mean, sigma):
    x_left = max (mean - 3 * sigma, 0)
    x = rand_range (x_left, mean + 3 * sigma)
    y = rand_range (0., 1.)
    while (y > math.exp (-0.5 * ( (x - mean)/sigma)**2)):
      x = rand_range (x_left, mean + 3 * sigma)
      y = rand_range (0, 1.)
    return x


def norm (position):
    return np.sqrt (position[0]**2 + position[1]**2)


def delta (dopo, prima=[0,0]):
    return norm ([d - p for d, p in zip (dopo, prima)])


def walk (N_steps = 10, start = [0,0], r_cost = False):
    # end = start identificare end e start come associati alla stessa cella di memoria
    end = [start[0], start[1]] # creare una nuova cella di memoria per end e metterci dentro gli stessi
                               # valori di start  
    angle = 0.
    step = 1.
    for i in range (N_steps):
        angle = rand_range (0, 2*np.pi)
        if not r_cost: step = try_and_catch_step (1, 0.2)
        end[0] += step * np.cos (angle)
        end[1] += step * np.sin (angle)
    return end


def sturges (N_events) :
    return int( np.ceil( 1 + 3.322 * np.log (N_events) ) )

