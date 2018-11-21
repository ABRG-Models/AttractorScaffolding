from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
import csv

# Read csv files.
def readDataset (filepath):
    f = np.zeros([1,1])
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            for row in rdr:
                f[-1,0] = float(row[0]) # gens since last increment
                f = np.append(f, np.zeros([1,1]), 0)
    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]

def doPlot (driftnodrift, plottype):

    if driftnodrift == 'drift':
        filetag = ''
    else:
        filetag = '_nodrift'
    # Make files from directory listing
    p = Path('../data')
    ##files = list(p.glob('*evolve'+filetag+'_a21_p10_ff4_100000000_gens_*.csv')) # order is wrong
    if driftnodrift=='drift':
        files = ['../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.15.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.2.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.25.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.3.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.35.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.4.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.45.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.5.csv']

        lbls = ['p=0.15',
                'p=0.20',
                'p=0.25',
                'p=0.30',
                'p=0.35',
                'p=0.40',
                'p=0.45',
                'p=0.50']
    else:
        files = ['../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.05.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.1.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.15.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.2.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.25.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.3.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.35.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.4.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.45.csv',
                 '../data/evolve'+filetag+'_a21_p10_ff4_100000000_gensplus_0.5.csv']

        lbls = ['p=0.05',
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

    # A nice colour array
    import sebcolour
    col = sebcolour.Colour

    # Font size for plotting
    fs=16

    # Set a default fontsize for matplotlib:
    fnt = {'family' : 'DejaVu Sans',
           'weight' : 'regular',
           'size'   : fs}
    matplotlib.rc('font', **fnt)

    scale = 1000.

    f1 = plt.figure(figsize=(20,6)) # Figure object

    M = np.zeros([nf,3])

    nbins = 100

    # Some markers
    mkr=['.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h']
    # And marker sizes
    ms=[6, 6, 7, 6, 7, 7, 8, 7, 8, 6]

    gcount=0
    a1 = [] # list of axes
    for y,fil in enumerate(files):
        print ('Processing file: {0}'.format(fil))
        D = readDataset (fil)
        if D.size == 0:
            continue
        bins = np.linspace (0, np.max(D), nbins)
        h,b = np.histogram (D, bins)

        # Plot points
        #print ('b len {0} h len {1}'.format(len(b), len(h)))
        colo = plt.cm.plasma(gcount/10)

        # Create subplot
        ax = f1.add_subplot (2,5,gcount+1)
        a1.append(ax)

        # Plot on it
        if plottype == 'loglog':
            a1[gcount].plot(np.log(b[:-1]/scale), np.log(h), color=colo,linestyle='None',  marker=mkr[y], markersize=ms[y])
            a1[gcount].set_ylabel('log (fitness increments)',fontsize=fs)
            a1[gcount].set_xlabel('log (1000 generations)',fontsize=fs)
        elif plottype == 'log':
            a1[gcount].plot(b[:-1]/scale, np.log(h), color=colo, linestyle='None', marker=mkr[y], markersize=ms[y])
            a1[gcount].set_ylabel('log (fitness increments)',fontsize=fs)
            a1[gcount].set_xlabel('1000 generations',fontsize=fs)
        else:
            a1[gcount].plot(b[:-1]/scale, h, color=colo, linestyle='None', marker=mkr[y], markersize=ms[y])
            a1[gcount].set_ylabel('fitness increments',fontsize=fs)
            a1[gcount].set_xlabel('1000 generations',fontsize=fs)

        # Set some common features of the subplot
        a1[gcount].set_title(lbls[gcount])
        a1[gcount].set_axisbelow(True)
        gcount = gcount + 1

    f1.tight_layout()
    plt.savefig ('png/fitinc_histos_' + plottype + filetag + '_ff4.png')

# Change this to choose which to plot.
driftnodrift = 'nodrift' # 'nodrift' or 'drift'
#doPlot ('nodrift', '')
#doPlot ('drift', '')
#doPlot ('nodrift', 'log')
#doPlot ('drift', 'log')
doPlot ('nodrift', 'loglog')
doPlot ('drift', 'loglog')

plt.show()
