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
    p = Path('../data')
    ##files = list(p.glob('*evolve'+filetag+'_a21_p10_ff4_100000000_gens_*.csv')) # order is wrong
    files = ['../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.01.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.02.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.05.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.1.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.15.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.2.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.25.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.3.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.35.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.4.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.45.csv',
             '../data/evolve'+filetag+'_a21_p10_'+ff+'_100000000_gens_0.5.csv']

    if plottype == 'loglog1':
        graphtag = driftnodrift + ', log/log (log hist bins)'
    elif plottype == 'loglog2':
        graphtag = driftnodrift + ', log/log (linear hist bins)'
    elif plottype == 'log':
        graphtag = driftnodrift + ', lin/log'
    else:
        graphtag = driftnodrift + ', lin/lin'

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

    nbins = 30

    # Some markers
    mkr=['.', 'o', 'v', 's', 's', 'v', 'o', '^', '^', 'h','s', 's']
    # And marker sizes
    ms=[6, 6, 7, 6, 7, 7, 8, 7, 8, 6, 7, 8]

    gcount=0
    a1 = [] # list of axes
    for y,fil in enumerate(files):
        print ('Processing file: {0}'.format(fil))
        D = readDataset (fil)
        if D.size == 0:
            continue

        print ('D min: {0}, D max: {1}'.format (np.min(D), np.max(D)))
        if plottype == 'loglog1':
            #bins = np.logspace (np.log(np.min(D)), np.log(np.max(D)), base=2.7183, num=nbins)
            bins = np.logspace (np.log10(np.min(D)), np.log10(np.max(D)), base=10, num=nbins)
            print ('bins {0}'.format(bins[:-1]))
        elif plottype == 'loglog2':
            bins = np.linspace (0, np.max(D), nbins)
        elif plottype == 'log':
            bins = np.linspace (0, np.max(D), nbins)
        else:
            bins = np.linspace (0, np.max(D), nbins)

        #print ('D min: {0} occurs {1} times, D max: {2} occurs {3} times.'
        #       .format (np.min(D), (D==np.min(D)).sum(), np.max(D), (D==np.max(D)).sum()))
        h,b = np.histogram (D, bins)
        print ('h: {0}'.format(h))
        print ('b: {0}'.format(b))

        # Plot points
        #print ('b len {0} h len {1}'.format(len(b), len(h)))
        colo = plt.cm.plasma(gcount/10)

        # Create subplot
        ax = f1.add_subplot (2,6,gcount+1)
        a1.append(ax)

        # Plot on it
        if plottype == 'loglog1':
            #bx = np.log(b[:-1]/scale)
            #bx = np.vstack ((bx, np.log(h))).T
            bx = np.log10(b[:-1]/scale) # logginess of x axis captured by np.logspace bins
            bx = np.vstack ((bx, np.log10(h))).T
            a1[gcount].set_ylabel('log$_{10}$ (evolutions)',fontsize=fs)
            a1[gcount].set_xlabel('log$_{10}$ (generations)',fontsize=fs)
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
        if plottype=='loglog1' and (gcount == 2 or gcount == 7 or gcount == 11):
            ax2.plot(bx[:,0], bx[:,1], color=plt.cm.gnuplot2((gcount*0.6)/11), linestyle='--', marker=mkr[y], markersize=12)
            #ax2.bar(bx[:,0], bx[:,1], width=0.1, edgecolor=plt.cm.gnuplot2((gcount*0.6)/(len(files)-1)), color=plt.cm.gnuplot2((gcount*0.6)/(len(files)-1)), alpha=0.2)
        elif plottype=='log':
            ax2.plot(bx[:,0], bx[:,1], color=plt.cm.gnuplot2((gcount*0.6)/11), linestyle='--', marker=mkr[y], markersize=10)

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
    #ax2.set_xlim([1,6])
    #ax2.set_ylim([0,5])
    ax2.legend(('p=0.05','p=0.3','p=0.5'))
    ##ax2.legend(lbls)
    f2.suptitle (driftnodrift);
    #f2.tight_layout()
    plt.savefig ('png/evolution_histos_' + plottype + filetag + '_'+ff+'_format2.png')

    # rect=[left bottom right top]
    f1.tight_layout(rect=[0.01,0.01,0.99,0.9])
    f1.text (0.5, 0.9, graphtag, fontsize=20)
    plt.savefig ('png/evolution_histos_' + plottype + filetag + '_'+ff+'.png')

    return M

# Change this to choose which to plot.
driftnodrift = 'nodrift' # 'nodrift' or 'drift'
fitf = 'ff4'
#doPlot ('nodrift', '', fitf)
#doPlot ('drift', '', fitf)
#M1 = doPlot ('nodrift', 'log', fitf)
#doPlot ('drift', 'log', fitf)
#M2 = doPlot ('nodrift', 'loglog1', fitf)
M3 = doPlot ('drift', 'loglog1', fitf)
#doPlot ('drift', 'loglog', fitf)

#P = np.array([0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5])
#f1 = plt.figure(figsize=(6,6))
#ax = f1.add_subplot (1,1,1)
#ax.plot (P, M1[:,1])
#ax.plot (P, M2[:,1])

plt.show()
