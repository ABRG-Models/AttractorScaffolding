#
# Bitflip version
#
from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
sys.path.insert(0, '../include')
import csv

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

def doPlot (driftnodrift, plottype, ff):

    if driftnodrift == 'drift':
        filetag = ''
    else:
        filetag = '_nodrift'
    # Make files from directory listing
    p = Path('../../data')
    ##files = list(p.glob('*evolve'+filetag+'_a21_p10_ff4_100000000_gens_*.csv')) # order is wrong
    files = ['../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_4.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_5.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_6.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_8.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_10.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_12.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_16.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_20.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_24.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_28.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_32.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_36.csv',
             '../../data/evolveb'+filetag+'_a21_p10_'+ff+'_100000000_gens_40.csv']

    if plottype == 'loglog1':
        graphtag = driftnodrift + ', log/log (log hist bins)'
    elif plottype == 'loglog2':
        graphtag = driftnodrift + ', log/log (linear hist bins)'
    elif plottype == 'log':
        graphtag = driftnodrift + ', lin/log'
    else:
        graphtag = driftnodrift + ', lin/lin'

    lbls = [#'bf=2',
            #'bf=3',
            'bf=4',
            'bf=5',
            'bf=6',
            'bf=8',
            'bf=10',
            'bf=12',
            'bf=16',
            'bf=20',
            'bf=24',
            'bf=28',
            'bf=32',
            'bf=36',
            'bf=40']

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

    scale = 1.

    f1 = plt.figure(figsize=(20,6)) # Figure object

    f2 = plt.figure(figsize=(8,8))
    ax2 = f2.add_subplot (1,1,1)

    M = np.zeros([nf,2])

    nbins = 20

    # Some markers
    mkr=['.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h','.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h', '.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h','.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h']
    # And marker sizes
    ms=[6, 6, 7, 6, 7, 7, 8, 7, 8, 6, 6, 6, 7, 6, 7, 7, 8, 7, 8, 6, 6, 6, 7, 6, 7, 7, 8, 7, 8, 6, 6, 6, 7, 6, 7, 7, 8, 7, 8, 6]

    gcount=0
    a1 = [] # list of axes
    for y,fil in enumerate(files):
        print ('Processing file: {0}'.format(fil))
        D = readDataset (fil)
        if D.size == 0:
            continue

        print ('D min: {0}, D max: {1} plottype: {2}'.format (np.min(D), np.max(D), plottype))
        if plottype == 'loglog1':
            if (np.min(D) == np.max(D)):
                continue
            bins = np.logspace (np.log(np.min(D)), np.log(np.max(D)), base=2.7183, num=nbins)
            #bins = np.logspace (np.log(np.min(D)), np.log(np.max(D)), base=6, num=nbins)
            print ('bins {0} min(D): {1}, max(D): {2}'.format(bins[:-1], np.min(D), np.max(D)))
        elif plottype == 'loglog2':
            bins = np.linspace (0, np.max(D), nbins)
        elif plottype == 'log':
            bins = np.linspace (0, np.max(D), nbins)
        else:
            bins = np.linspace (0, np.max(D), nbins)

        #print ('D min: {0} occurs {1} times, D max: {2} occurs {3} times.'
        #       .format (np.min(D), (D==np.min(D)).sum(), np.max(D), (D==np.max(D)).sum()))
        print ('bins: {0}'.format(bins))
        h,b = np.histogram (D, bins)
        print ('h: {0}'.format(h))
        print ('b: {0}'.format(b))

        # Plot points
        #print ('b len {0} h len {1}'.format(len(b), len(h)))
        colo = plt.cm.plasma(gcount/len(lbls))

        # Create subplot
        ax = f1.add_subplot (5,5,gcount+1)
        a1.append(ax)

        # Plot on it
        if plottype == 'loglog1':
            #bx = np.log(b[:-1]/scale)
            #bx = np.vstack ((bx, np.log(h))).T
            bx = np.log10(b[:-1]/scale) # logginess of x axis captured by np.logspace bins
            bx = np.vstack ((bx, np.log10(h))).T
            a1[gcount].set_ylabel('log (evolutions)',fontsize=fs)
            a1[gcount].set_xlabel('log (generations)',fontsize=fs)
            a1[gcount].set_xlim([1,6])
            a1[gcount].set_ylim([0,5])

        elif plottype == 'loglog2':
            bx = np.log(b[:-1]/scale)
            bx = np.vstack ((bx, np.log(h))).T
            a1[gcount].set_ylabel('log (evolutions)',fontsize=fs)
            a1[gcount].set_xlabel('log (generations)',fontsize=fs)
            a1[gcount].set_xlim([1,16])

        elif plottype == 'log':
            bx = b[:-1]/scale
            bx = np.vstack ((bx, np.log(h))).T
            a1[gcount].set_ylabel('log (evolutions)',fontsize=fs)
            a1[gcount].set_xlabel('generations',fontsize=fs)

        else:
            bx = b[:-1]/scale
            bx = np.vstack ((bx, h)).T
            a1[gcount].set_ylabel('evolutions',fontsize=fs)
            a1[gcount].set_xlabel('generations',fontsize=fs)

        bx = bx[np.where(np.isfinite(bx[:,1]))]
        bx = bx[np.where(np.isfinite(bx[:,0]))]
        #print ('bx: {0}'.format(bx))
        a1[gcount].plot(bx[:,0], bx[:,1], color=colo, linestyle='-', marker=mkr[y], markersize=ms[y])

        print ('gcount={0}'.format(gcount))
        #if gcount == 1 or gcount == 6 or gcount == 9:
        ax2.plot(bx[:,0], bx[:,1], color=colo, linestyle='-', marker=mkr[y], markersize=ms[y])

        # Weights? How to apply right? 1/sigma or 1/sigma^2.
        fit, residuals, rank, singular_values, rcond = np.polyfit (bx[:,0], bx[:,1], 1, full=True)
        print ('fit, residuals, rank, singular_values, rcond: {4} : {0} : {1} : {2} : {3} '
               .format(residuals, rank, singular_values, rcond, fit))
        fit_fn = np.poly1d (fit)
        #print (fit_fn)
        M[y,0] = fit[0]     # slope
        #M[y,1] = residuals
        #a1[gcount].plot (bx[:,0], fit_fn(bx[:,0]), '-', linewidth=2, color=colo)

        # Set some common features of the subplot
        a1[gcount].set_title(lbls[gcount])
        a1[gcount].set_axisbelow(True)
        #a1[gcount].text (1, 1, graphtag)
        gcount = gcount + 1


    ax2.set_ylabel('log$_{10}$ (F=1 evolutions)',fontsize=fs)
    ax2.set_xlabel('log$_{10}$ (generations to evolve)',fontsize=fs)
    ax2.set_xlim([1,6])
    ax2.set_ylim([0,5])
    ax2.legend(lbls, fontsize=10)
    f2.suptitle (driftnodrift);
    #f2.tight_layout()
    plt.savefig ('figures/evolution_histos_' + plottype + filetag + '_'+ff+'_format2_fb.png')

    # rect=[left bottom right top]
    #f1.tight_layout(rect=[0.01,0.01,0.99,0.9])
    f1.text (0.5, 0.9, graphtag, fontsize=20)
    plt.savefig ('figures/evolution_histos_' + plottype + filetag + '_'+ff+'_fb.png')

    return M

# Change this to choose which to plot.
driftnodrift = 'drift' # 'nodrift' or 'drift'
fitf = 'ff4'
#doPlot ('nodrift', '', fitf)
#doPlot ('drift', '', fitf)
#M1 = doPlot ('nodrift', 'log', fitf)
#doPlot ('drift', 'log', fitf)
M3 = doPlot ('drift', 'loglog1', fitf)
#doPlot ('drift', 'loglog', fitf)

#P = np.array([0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5])
#f1 = plt.figure(figsize=(6,6))
#ax = f1.add_subplot (1,1,1)
#ax.plot (P, M1[:,1])
#ax.plot (P, M2[:,1])

plt.show()
