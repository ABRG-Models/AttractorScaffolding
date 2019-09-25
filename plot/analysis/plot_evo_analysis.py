#
# Plots the fitness vs generations "punctuated equilibria figure". The
# data that this script depends upon is generated by running
# scripts/runevolve_c2_withf.sh
#

from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sys
sys.path.insert(0,'../include')
import sebcolour
col = sebcolour.Colour
cols1 = np.array([col.darkviolet,
                  col.darkorchid1,
                  col.mediumorchid1,
                  col.orchid,
                  col.maroon1,
                  col.violetred1,
                  col.violetred,
                  col.crimson,
                  col.orange,
                  col.goldenrod1,
                  col.darkviolet,
                  col.darkorchid1,
                  col.mediumorchid1,
                  col.orchid,
                  col.maroon1,
                  col.violetred1,
                  col.violetred,
                  col.crimson,
                  col.orange,
                  col.goldenrod1])

# Read csv files with gen/fitness data in
def readDataset (filepath):
    f = np.zeros([1,9])
    f_nozeros = np.zeros([1,9])
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            lastgen = 0
            first = True
            for row in rdr:
                if first:
                    lastgen = float(row[0])
                    first = False
                f[-1,0] = float(row[0]) # gen
                f[-1,1] = float(row[1]) # fitness
                #nets.append(row[2])     # genome
                f[-1,2] = float(row[3]) # num basins
                f[-1,3] = float(row[4]) # mean limit cycle length
                f[-1,4] = float(row[5]) # max limit cycle length
                f[-1,5] = float(row[6]) # num changed transitions
                f[-1,6] = float(row[7]) # change in fitness
                f[-1,7] = float(row[0]) - lastgen # interval length
                f[-1,8] = float(row[8]) # change in hamming dist
                if float(row[7]) != 0:
                    f_nozeros[-1,:] = f[-1,:]
                    f_nozeros = np.append(f_nozeros, np.zeros([1,9]), 0)
                f = np.append(f, np.zeros([1,9]), 0)

    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:], f_nozeros[:-1,:]

def readNets (filepath):
    nets = []
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            for row in rdr:
                nets.append(row[2])     # genome
    return nets

def doPlot (pOn, driftnodrift):
    # 10000 or 100000 depending on p
    FromValue = 10000
    print ('pOn = {0}'.format(pOn))

    # Make files from directory listing
    p = Path('../../data/evolutions/')
    if driftnodrift == 'drift':
        ftag = '_'
    else:
        ftag = '_' + driftnodrift + '_'
    globstr = '*evolve'+ftag+'withf*_fitness_'+str(pOn)+'_*.csv'
    print (globstr)
    files = list(p.glob(globstr))

    #print (files)

    # Font size for plotting
    fs2=14 # for axes tick labels

    # Set a default fontsize for matplotlib:
    fnt = {'family' : 'DejaVu Sans',
           'weight' : 'regular',
           'size'   : fs2}
    matplotlib.rc('font', **fnt)

    F1 = plt.figure (figsize=(20,12))
    f1 = F1.add_subplot(2,3,1)
    f2 = F1.add_subplot(2,3,2)
    f3 = F1.add_subplot(2,3,3)
    f4 = F1.add_subplot(2,3,4)
    f5 = F1.add_subplot(2,3,5)
    f6 = F1.add_subplot(2,3,6)

    fcount=0
    numchangedtrans = np.zeros([1])
    fitnesschange = np.zeros([1])
    tsince = np.zeros([1])
    meanlim = np.zeros([1])
    maxlim = np.zeros([1])
    numlc = np.zeros([1])
    genomehamming = np.zeros([1])
    for f in files:
        A, B = readDataset(f)
        numchangedtrans = np.append(numchangedtrans, B[:,5], 0)
        fitnesschange = np.append(fitnesschange, B[:,6], 0)
        tsince = np.append(tsince, B[:,7], 0)
        meanlim = np.append(meanlim, B[:,4], 0)
        maxlim = np.append(maxlim, B[:,5], 0)
        numlc = np.append(numlc, B[:,3], 0)
        genomehamming = np.append(genomehamming, B[:,8], 0)
        if fcount <= 8:
            # Find out where in A we have the -10000K generation & check its value
            From10K=A[np.where(A[:,0]>-(FromValue+1))]
            if From10K[0,1] < 0.2:
                f1.plot (A[:,0]/FromValue,A[:,1],'-',linewidth=2, color=cols1[fcount])
                fcount = fcount + 1

    # To make histos, need to pick out the unique num changed trans, then
    # the fitnesses for each, then histo those.
    nct_unq = np.unique(numchangedtrans)
    nbins = 101
    X, Y = np.meshgrid(np.logspace(-10,0,nbins-1), np.linspace(0,32,33))
    Z = np.zeros([33,100])
    for nc in nct_unq:
        fcsel = fitnesschange[np.where(numchangedtrans==nc)]
        #bins = np.linspace(0, 1.0, nbins)
        bins = np.logspace(-10, 0, nbins)
        h, b = np.histogram (fcsel, bins)
        Z[int(nc),:] = h
    f2.contourf(np.log(X),Y,Z)
    f2.set_xlabel('log(Fitness change)');
    f2.set_ylabel('Number of changed transitions');

    #f3.set_xlabel('Gens since last increment');
    #f3.set_ylabel('Number of changed transitions');
    nbins = 101
    X, Y = np.meshgrid(np.linspace(0,np.max(tsince),nbins-1),
                       np.linspace(0,32,33))
    Z = np.zeros([33,nbins-1])
    for nc in nct_unq:
        tssel = tsince[np.where(numchangedtrans==nc)]
        bins = np.linspace(0, np.max(tsince), nbins)
        h, b = np.histogram (tssel, bins)
        Z[int(nc),:] = h
    #f3.plot (fitnesschange, numchangedtrans, 'o');
    Z_ = np.log(Z)
    Z_[Z_ == -np.inf]=0
    f3.contourf(X,Y,Z_)
    f3.plot (tsince, numchangedtrans, '.', color='k', markersize=2);
    f3.set_xlabel('Time since last transition');
    f3.set_ylabel('Number of changed transitions');

    nbins = 33
    X, Y = np.meshgrid(np.linspace(0,32,33), np.linspace(0,32,33))
    Z = np.zeros([33,33])
    for nc in nct_unq:
        mlsel = meanlim[np.where(numchangedtrans==nc)]
        bins = np.linspace(1, 32, nbins+1)
        h, b = np.histogram (mlsel, bins)
        Z[int(nc),:] = h
    f4.contourf(X,Y,Z)
    f4.plot (meanlim, numchangedtrans, '.', color='k', markersize=2);
    f4.set_xlabel('Mean LC size');
    f4.set_ylabel('Number of changed transitions');

    # This visualisation is for data with integer ranges on both axes
    nc_range = np.linspace(0,32,33)
    gh_range = np.linspace(0,160,161)
    X,Y = np.meshgrid(gh_range, nc_range)
    Z = np.zeros([33,161])
    for nc in nct_unq:
        ghselection = genomehamming[np.where(numchangedtrans==nc)]
        ghs_unq = np.unique (ghselection)
        #print ('ghs_unq: {0}'.format(ghs_unq))
        for gh in ghs_unq:
            if gh != 0 and gh != 160:
                sumat = ghselection[np.where(ghselection == gh)].sum()
                #print ('sum at gh {0}, nc {1} is {2}'.format(gh, nc, sumat))
                Z[int(nc),int(gh)] = sumat
    f5.contourf(X,Y,Z)
    f5.set_xlabel('Genome Hamming change');
    f5.set_ylabel('Number of changed transitions');

    nbins = 101
    #fitbins = np.linspace (0, 1.0, nbins-1)
    fitbins = np.logspace(-10,0,nbins-1)
    X, Y = np.meshgrid (gh_range, fitbins)
    Z = np.zeros([100,161])
    gh_unq = np.unique (genomehamming)
    for gh in gh_unq:
        if gh==0 or gh==160:
            continue
        fcsel = fitnesschange[np.where(genomehamming==gh)]
        #fitbins = np.linspace (0, 1.0, nbins)
        fitbins = np.logspace (-10, 0, nbins)
        h, b = np.histogram (fcsel, fitbins)
        Z[:,int(gh)] = h

    #f6.contourf(X,Y,Z)
    f6.contourf(X,np.log(Y),Z)
    #f6.plot (genomehamming, np.log(fitnesschange), '.', color='k', markersize=2);
    f6.set_xlabel('Genome Hamming change');
    f6.set_ylabel('log(Fitness change)');

    # Plot one of them in bold:
#    specialfile=files[3]
#    A, B = readDataset(specialfile)
#    f1.plot (A[:,0]/FromValue,A[:,1],'-',linewidth=5, color=col.navy)

    f1.set_xlim([-1, 0.01])
    f1.set_xlabel(str(FromValue) + ' generations');
    f1.set_ylabel('fitness');
    f1.set_title (driftnodrift + ' p=' + str(pOn))

    plt.savefig ('figures/evo_analysis_'+driftnodrift+'_'+'{0:.2f}'.format(pOn)+'_ff4.png')

    # Set a default fontsize for matplotlib:
    fs=12
    fnt = {'family' : 'DejaVu Sans',
           'weight' : 'regular',
           'size'   : fs}
    matplotlib.rc('font', **fnt)

    # For the specialfile, also show its states, using Dan's code.
    import states_lib as sl

    plot_states = 1
    if plot_states:
        fcount = 0
        for f in files:
            if fcount > 0:
                break
            A, B  = readDataset(f)
            nets = readNets(f)
            nc = 0
            gc = 0
            last_fitness = -1
            F2 = plt.figure (figsize=(20,12))
            for net in nets:
                if gc > 15:
                    break;
                if (A[nc,1] > last_fitness):
                    last_fitness = A[nc,1]
                    f2 = F2.add_subplot (4,4,gc+1)
                    sl.plot_states (net, f2.axes)
                    f2.axes.set_title('{0}: f={1:.4f}'.format(gc, A[nc,1]))
                    gc = gc + 1
                last_fitness = A[nc,1]
                nc = nc + 1
            fcount = fcount + 1

driftnodrift = 'drift'

#doPlot (0.5, driftnodrift)  # 13 in 1000000 and 119 in 10000000
doPlot (0.4, driftnodrift)  # 26 in 500000
doPlot (0.3, driftnodrift)  # 66 in 500000
doPlot (0.2, driftnodrift)  # 150 in 500000
doPlot (0.15, driftnodrift) # 131 in 500000
doPlot (0.1, driftnodrift)  # 63 in 500000
doPlot (0.05, driftnodrift) # 22 in 1000000
doPlot (0.02, driftnodrift) # 2 full F=1 in 1000000

plt.show()
