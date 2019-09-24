##
## This is analysis/dataset reading code which is specific to this AttractorScaffolding project.
##

import numpy as np
import csv
import sebanalysis as sa

class A:
    # Read csv files.
    def readDataset (filepath):
        f = np.zeros([1,1])
        with open (filepath, 'r') as csvfile:
                rdr = csv.reader (csvfile)
                for row in rdr:
                    f[-1] = float(row[0])
                    f = np.append(f, np.zeros([1,1]), 0)
        # Note the -1 as there will be a final, zero line in the array
        return f[:-1,:]

    # Compute the slope of the log of linearly spaced bins of the data
    # in files. Also compute means and stderr of means
    #
    # @files: The raw data filenames to process; evolve_nc2_I16-0_T21-10_ff4_100000000_gens_0.03.csv, etc
    # @p: The list of probability values for @files
    # @scale: A scale for computing the slope
    # @nbins: How many bins to use when histogramming the data for the slope.
    def compute_slopes (files, p, scale=1, nbins=50):
        nf = len(files)
        S = np.zeros([nf,2]) # Slopes
        M = np.zeros([nf,3]) # Mean/stderr of mean
        H = [] # Histogram counts
        B = [] # Histogram bins
        FF = []
        for y,fil in enumerate(files):
            print ('Processing file: {0}'.format(fil))
            D = A.readDataset (fil)
            print ('D has rank {0}, shape {1} and size {2}'.format(D.ndim, D.shape, D.size))
            if D.size == 0:
                print ('D size 0, continue...')
                continue
            print ('mean of D is {0}, max/min: {1}/{2}'.format(np.mean(D),np.max(D),np.min(D)))
            bins = np.linspace(1,0.5*np.max(D),nbins)
            h,b = np.histogram (D, bins)

            # Do a fit
            x = np.where(np.log(h)>0)[0]
            if x.size == 0:
                print ('x size 0, continue...')
                continue
            if x.size > 0:
                bx = b[x]/scale
                fit = np.polyfit (bx, np.log(h[x]), 1)
                fit_fn = np.poly1d (fit)
                FF.append(fit_fn)
                # Slope is fit[0], Record for a later graph.
                S[y,0] = fit[0]     # slope
            else:
                S[y,0] = 0     # no slope known

            S[y,1] = p[y] # p, the flip probability.
            M[y,0] = p[y] # p, the flip probability.
            M[y,1], M[y,2] = sa.Analysis.bootstrap_mean (D)
            H.append(h)
            B.append(b)
            print ('mean/stderrmean is {0} ({1})'.format(M[y,1], M[y,2]))
        return S, M, H, B

    # Compute the slope of the log of linearly spaced bins of the data
    # in files. Also compute means and stderr of means
    #
    # @files: The raw data filenames to process; evolve_nc2_I16-0_T21-10_ff4_100000000_gens_0.03.csv, etc
    # @p: The list of probability values for @files
    def compute_means (files, p):
        nf = len(files)
        M = np.zeros([nf,3])
        for y,fil in enumerate(files):
            print ('Processing file: {0}'.format(fil))
            D = A.readDataset (fil)
            print ('D has rank {0}, shape {1} and size {2}'.format(D.ndim, D.shape, D.size))
            if D.size == 0:
                print ('D size 0, continue...')
                continue
            print ('mean of D is {0}, max/min: {1}/{2}'.format(np.mean(D),np.max(D),np.min(D)))

            M[y,0] = p[y] # p, the flip probability.
            M[y,1], M[y,2] = sa.Analysis.bootstrap_mean (D)
            print ('mean/stderrmean is {0} ({1})'.format(M[y,1], M[y,2]))
        return M
