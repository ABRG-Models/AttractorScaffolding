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

    p = [0.1, 0.2, 0.3, 0.4, 0.5]
    directry = 'data_fitinc'
    contexttag = 'nc2_I16-0_T21-10'
    maxgens='1000000'
    ff='ff4'

    # Make file names
    files = []
    if driftnodrift == 'drift':
        filetag = ''
        for pp in p:
            print ('Append file for p={0}'.format(pp))
            files.append ('../{4}/evolve_{3}_{0}_{1}_gensplus_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))
    else:
        filetag = '_nodrift'
        for pp in p:
            files.append ('../{4}/evolve_nodrift_{3}_{0}_{1}_gensplus_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))

    # Make labels
    lbls = []
    for pp in p:
        lbls.append ('p={0}'.format(pp))

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
        ax = f1.add_subplot (1,5,gcount+1)
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

#doPlot ('drift', '')
#doPlot ('drift', 'log')
doPlot ('drift', 'loglog')

plt.show()
