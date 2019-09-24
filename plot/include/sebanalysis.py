# Some analysis code, collected into a library.
import numpy

class Analysis():

    def __init__(self):
        print ('init')

    # Sample, with replacement, from range 0, nRows.
    def bootstrap_sample (n):
        indices = numpy.floor((n * numpy.random.ranf([1,n])));
        return indices.astype(int)

    # Compute the mean of the distribution and the standard error by the bootstrap method
    def bootstrap_mean (distribution, nSamp=1024):
        d_arr = numpy.array(distribution)
        bmeans = numpy.zeros (nSamp)
        nRows = len(d_arr)
        for s in range (0, nSamp):
            bs = Analysis.bootstrap_sample (nRows)
            bmeans[s] = numpy.mean (d_arr[bs])
        std_err = numpy.sqrt(numpy.var(bmeans))
        return numpy.mean(d_arr), std_err
