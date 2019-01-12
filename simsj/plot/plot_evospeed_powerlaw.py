from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
import csv
import powerlaw
# A nice colour array
import sebcolour
col = sebcolour.Colour

# Neither threading approaches work for python.
#import threading
# or use multiprocessing.dummy, which wraps threading
#from multiprocessing.dummy import Pool as ThreadPool
#
# Using multiprocessing is more complex:
#from multiprocessing import Process

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

def doPlot (driftnodrift, ff, evotype):

    if driftnodrift == 'drift':
        filetag = ''
    else:
        filetag = '_nodrift'

    # Make files from directory listing
    p = Path('../data')
    ##files = list(p.glob('*evolve'+filetag+'_a21_p10_ff4_100000000_gens_*.csv')) # order is wrong
    numgens = '100000000'
    #numgens = '1000000000'
    targs = '_a21_p10_' # n=5
    #targs = '_a42_p21_' # n=6
    files = [
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.01.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.02.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.05.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.1.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.15.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.2.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.25.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.3.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.35.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.4.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.45.csv',
        '../data/evolve'+filetag+targs+ff+'_'+numgens+'_'+evotype+'_0.5.csv']

    graphtag = driftnodrift + ', powerlaw. Red: data, Blue: powerlaw fit, Green: lognormal, Pink: exponential'

    lbls = ['p=0.01',
            'p=0.02',
            'p=0.05',
            'p=0.10',
            'p=0.15',
            'p=0.20',
            'p=0.25',
            'p=0.30',
            'p=0.35',
            'p=0.40',
            'p=0.45',
            'p=0.50']

    # num files
    nf = len(files)

    # Font size for plotting
    fs=16

    # Set a default fontsize for matplotlib:
    fnt = {'family' : 'DejaVu Sans',
           'weight' : 'regular',
           'size'   : fs}
    matplotlib.rc('font', **fnt)

    scale = 1.

    f1 = plt.figure(figsize=(24,8)) # Figure object

    M = np.zeros([nf,2])

    nbins = 100

    # Some markers
    mkr=['.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h']
    # And marker sizes
    ms=[6, 6, 7, 6, 7, 7, 8, 7, 8, 6]

    gcount=0
    a1 = [] # list of axes
    for y,fil in enumerate(files):
        print ('----------------\nProcessing file: {0}'.format(fil))
        D = readDataset (fil)
        if D.size == 0:
            continue

        print ('D min: {0}, D max: {1}'.format (np.min(D), np.max(D)))

        # Powerlaw fitting
        fit = powerlaw.Fit(D[:,0]) #, discrete=True, estimate_discrete=True)
        print('powerlaw.alpha: {0}'.format(fit.power_law.alpha))
        print('powerlaw.sigma: {0}'.format(fit.power_law.sigma))
        print('powerlaw.xmin: {0}'.format(fit.power_law.xmin))

        # Bootstrap test to see if powerlaw is apparently sensible
        #largerCount = 0
        #dataD = fit.power_law.D
        #numSimThreads = 6
        #ns = np.zeros(numSimThreads)
        #simsInThread = 5
        #numSims = numSimThreads * simsInThread

        #print ('Simulating power law {0} times...'.format(numSims))

        # Can multi-thread this, writing into array size numSims and then sum that array at the end.
        #def doSimFit (counts):
        #for it in range (0, simsInThread):
        #    simdat = fit.power_law.generate_random(np.size(D))
        #    fit2 = powerlaw.Fit (simdat)
        #    print ('Simulated KS statistic: {0} vs data-based KS: {1} (fit2.xmin: {2})'.format(fit2.power_law.D, dataD, fit2.power_law.xmin))
        #    if fit2.power_law.D > dataD:
        #        counts = counts + 1

        #pool = ThreadPool(numSimThreads)
        #results = pool.map(doSimFit, ns)
        #pool.close()
        #pool.join()
        #print ('ns: {0}'.format(ns))

        #powerp = float(largerCount) / float(numSims)
        #print ('Simulated power law {0} times. p={1}'.format(numSims, powerp))

        # Comparative tests
        if fit.power_law.sigma < 0.05:
            # then assume powerlaw is an ok fit
            bestfitter = 1
        else:
            bestfitter = 0 # 0 unknown, 1 powerlaw better than exp, 2 lognormal, 3 exponential, 4 powerlaw better than lognormal

        if bestfitter == 1 or bestfitter == 0:
            Rex, pex = fit.distribution_compare('power_law', 'exponential')
            print('Comparison with exponential: R={0}, p={1}'.format(Rex,pex))
            if pex < 0.05:
                if Rex < 0:
                    bestfitter = 3
                else:
                    bestfitter = 1

            Rln, pln = fit.distribution_compare('power_law', 'lognormal')
            print('Comparison with lognormal: R={0}, p={1}'.format(Rln,pln))
            if pln < 0.05:
                if Rln < 0:
                    bestfitter = 2
                else:
                    print ('Power to the law!')
                    bestfitter = 4

        # Create subplot
        ax = f1.add_subplot (2,6,gcount+1)
        a1.append(ax)

        bestfitter = 6 # hack

        # Plot the data
        powerlaw.plot_pdf (D[:,0], color=col.darkviolet, ax=a1[gcount])
        # or
        fit.plot_pdf (color=col.red2, ax=a1[gcount], linewidth=3)

        if bestfitter == 1:
            # Plot the powerlaw fit
            fit.power_law.plot_pdf (color=col.blue3, linestyle='-.', ax=a1[gcount])
        elif bestfitter == 2:
            # Plot the lognormal fit
            fit.lognormal.plot_pdf (color=col.mediumspringgreen, linestyle='-.', ax=a1[gcount])
        elif bestfitter == 3:
            # Plot exponential
            fit.truncated_power_law.plot_pdf (color=col.deeppink2, linestyle='-.', ax=a1[gcount])
        elif bestfitter == 4:
            fit.power_law.plot_pdf (color=col.blue3, linestyle='-.', linewidth=4, ax=a1[gcount])
        else:
            # Unknown which fit is best.
            #print ('No good fit...')
            fit.power_law.plot_pdf (color=col.blue3, linestyle='-.', linewidth=1, ax=a1[gcount])
            fit.lognormal.plot_pdf (color=col.mediumspringgreen, linestyle='-.', linewidth=1, ax=a1[gcount])
            fit.truncated_power_law.plot_pdf (color=col.deeppink2, linestyle='-.', linewidth=1, ax=a1[gcount])

        # powerlaw.plot_cdf(D[:,0], color='b')
        a1[gcount].set_ylabel('log (evolutions)',fontsize=fs)
        a1[gcount].set_xlabel('log (generations)',fontsize=fs)
        a1[gcount].set_title('{0}, al={1:.2f}, xmin={3:.2f} D={2:.2f}'
                             .format(lbls[gcount], fit.power_law.alpha, fit.power_law.D, fit.power_law.xmin), fontsize=9)
        gcount = gcount + 1

    # rect=[left bottom right top]
    f1.tight_layout(rect=[0.01,0.01,0.99,0.9])

    f1.text (0.2, 0.9, graphtag, fontsize=20)
    plt.savefig ('png/evospeed' + filetag + '_' + evotype + '_' + ff + targs + '.png')

    return M

# Change this to choose which to plot.
driftnodrift = 'drift' # 'nodrift' or 'drift'
fitf = 'ff4'
evotype = 'gens'     # for stats on evolutions of F=1 genomes
#evotype = 'gensplus' # for stats on every fitness increment

M1 = doPlot ('drift', fitf, evotype)

plt.show()
