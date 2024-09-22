#!/usr/bin/python

from math import sqrt, pow


class stats :
    '''calculator for statistics of a list of numbers'''

    summ = 0.
    sumSq = 0.
    N = 0
    sample = []

    def __init__ (self, sample):
        '''
        reads as input the collection of events,
        which needs to be a list of numbers
        '''
        self.sample = sample
        self.summ = sum (self.sample)
        self.sumSq = sum ([x*x for x in self.sample])
        self.N = len (self.sample)

    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def mean (self) :
        '''
        calculates the mean of the sample present in the object
        '''
        return self.summ / self.N

    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def variance (self, bessel = True) :
        '''
        calculates the variance of the sample present in the object
        '''
        var = self.sumSq / self.N - self.mean () * self.mean ()
        if bessel : var = self.N * var / (self.N - 1)
        return var

    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def sigma (self, bessel = True) :
        '''
        calculates the sigma of the sample present in the object
        '''
        return sqrt (self.variance (bessel))

    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def sigma_mean (self, bessel = True) :
        '''
        calculates the sigma of the sample present in the object
        '''
        return sqrt (self.variance (bessel) / self.N)

    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def skewness (self) :
        '''
        calculate the skewness of the sample present in the object
        '''
        mean = self.mean ()
        asymm = 0.
        for x in self.sample:
            asymm = asymm + pow (x - mean,  3)
        asymm = asymm / (self.N * pow (self.sigma (), 3))
        return asymm


    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def kurtosis (self) :
        '''
        calculate the kurtosis of the sample present in the object
        '''
        mean = self.mean ()
        kurt = 0.
        for x in self.sample:
            kurt = kurt + pow (x - mean,  4)
        kurt = kurt / (self.N * pow (self.variance (), 2)) - 3
        return kurt

    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    def append (self, x):
        '''
        add an element to the sample
        '''
        self.sample.append (x)
        self.summ = self.summ + x
        self.sumSq = self.sumSq + x * x
        self.N = self.N + 1






