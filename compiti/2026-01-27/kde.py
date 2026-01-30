from numpy import exp, log

def k_norm (x) :
  return exp (-x**2/2) * 0.3989422804


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def k_unif (x) :
  if abs (x) < 1 : return 0.5
  return 0.


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def k_para (x) : 
  if abs (x) < 1 : return 0.75 * (1-x**2)
  return 0.


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def kde (x, sample , kernel = k_norm, h = 1) :
  if h <=0 : return 0.
  if len (sample) == 0 : return 0.
  total = 0.
  for xi in sample :
    total += kernel ((x-xi)/h) 
  return total / (h * len (sample))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def kde_loglikelihood (sample, kernel = k_norm, h = 1) :
  loglikelihood = 0.
  for i, xi in enumerate (sample):
    # Slice from start to i, and from i+1 to the end
    remaining = sample[:i] + sample[i+1:]
    loglikelihood += log (kde (xi, remaining, kernel, h))
  return loglikelihood  
