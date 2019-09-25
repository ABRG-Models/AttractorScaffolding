from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
sys.path.insert(0,'../include')
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

def doPlot (plottype, ff):

    if plottype == 'loglog1':
        graphtag = 'log/log (log hist bins)'
    elif plottype == 'loglog2':
        graphtag = 'log/log (linear hist bins)'
    elif plottype == 'log':
        graphtag = 'lin/log'
    else:
        graphtag = 'lin/lin'

    p = [0.1, 0.2, 0.3, 0.4, 0.5]

    directry = '../../data'
    contexttag = 'nc2_I16-0_T21-10'
    maxgens='100000000'

    # Make file names
    files = []
    filetag = ''
    for pp in p:
        print ('Append file for p={0}'.format(pp))
        files.append ('{4}/evolve_{3}_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))

    # Make labels
    lbls = []
    for pp in p:
        lbls.append ('p=.{0}'.format(int(round(10*pp))))

    mkr=['o','s','v','^','h',  'o','s','v','^','h']
    ms= [ 9,  9,  9,  9,  9,    9,  9,  9,  9,  9 ]

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

        a1[gcount].plot(bx[:,0], bx[:,1], color=colo, linestyle='-', marker=mkr[y], markersize=ms[y])

        print ('gcount={0}'.format(gcount))
        if plottype=='loglog1' and (gcount == 2 or gcount == 7 or gcount == 11):
            ax2.plot(bx[:,0], bx[:,1], color=plt.cm.gnuplot2((gcount*0.6)/11), linestyle='--', marker=mkr[y], markersize=12)
        elif plottype=='log':
            ax2.plot(bx[:,0], bx[:,1], color=plt.cm.gnuplot2((gcount*0.6)/11), linestyle='--', marker=mkr[y], markersize=10)

        # Weights? How to apply right? 1/sigma or 1/sigma^2.
        fit, residuals, rank, singular_values, rcond = np.polyfit (bx[:,0], bx[:,1], 1, full=True)
        print ('fit, residuals, rank, singular_values, rcond: {4} : {0} : {1} : {2} : {3} '
               .format(residuals, rank, singular_values, rcond, fit))
        fit_fn = np.poly1d (fit)
        M[y,0] = fit[0]     # slope

        # Set some common features of the subplot
        a1[gcount].set_title(lbls[gcount])
        a1[gcount].set_axisbelow(True)
        gcount = gcount + 1


    ax2.set_ylabel('log$_{10}$ (F=1 evolutions)',fontsize=fs)
    ax2.set_xlabel('log$_{10}$ (generations to evolve)',fontsize=fs)
    ax2.legend(('p=0.05','p=0.3','p=0.5'))
    plt.savefig ('figures/evolution_histos_' + plottype + filetag + '_'+ff+'_format2.png')

    # rect=[left bottom right top]
    f1.tight_layout(rect=[0.01,0.01,0.99,0.9])
    f1.text (0.5, 0.9, graphtag, fontsize=20)
    plt.savefig ('figures/evolution_histos_' + plottype + filetag + '_'+ff+'.png')

    return M

# Change this to choose which to plot.
fitf = 'ff4'
M3 = doPlot ('loglog1', fitf)

plt.show()
