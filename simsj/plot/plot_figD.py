import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
import csv

# Change this to choose which to plot.
driftnodrift = 'drift' # 'nodrift' or 'drift'
ff='ff4'

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

#maxgens='100000000'
maxgens='1000000000'
if driftnodrift == 'drift':
    filetag = ''
    files = ['../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.05.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.1.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.15.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.2.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.25.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.3.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.35.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.4.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.45.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.5.csv']
else:
    filetag = '_nodrift'
    files = ['../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.05.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.1.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.15.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.2.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.25.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.3.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.35.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.4.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.45.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.5.csv']


lbls = ['p=0.10',
        'p=0.20',
        'p=0.30',
        'p=0.40',
        'p=0.50']

lbls2 = ['p=0.05',
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

graphskip = 2 # or 2 to skip every other.

# Font size for plotting
fs=20

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs}
matplotlib.rc('font', **fnt)

scale = 1000.

f1 = plt.figure(figsize=(8,8)) # Figure object
a1 = f1.add_subplot (1,1,1)

M = np.zeros([nf,3])

# Global choice of nbins
nbins_g = 50

# Plot the lines. These are what will appear in the legend
printlines = 1
fcount=0
for y,fil in enumerate(files):
    print ('Processing file: {0}'.format(fil))
    fcount = fcount+1
    nbins = nbins_g
    D = readDataset (fil)
    print ('D has rank {0}, shape {1} and size {2}'.format(D.ndim, D.shape, D.size))
    if D.size == 0:
        continue
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)

    # Do a fit
    x = np.where(np.log(h)>0)[0]
    print ('x: {0}'.format(x))
    print ('x size: {0}'.format(x.size))
    if x.size > 0:
        bx = b[x]/scale
        print ('b: {0}'.format(b))
        print ('h: {0}'.format(h))
        fit = np.polyfit (bx, np.log(h[x]), 1)
        fit_fn = np.poly1d (fit)

        # Slope is fit[0], Record for a later graph.
        M[y,0] = fit[0]     # slope
        colo = plt.cm.brg((fcount*0.5)/len(files))
        if (fcount%graphskip == 0) and printlines:
            print ('Plotting file {0}'.format(fil))
            a1.plot(bx,fit_fn(bx),'-',linewidth=2,color=colo)
    else:
        M[y,0] = 0     # no slope known

    M[y,1] = (y+1)*0.05 # p, the flip probability.
    M[y,2] = np.mean(D) # mean generations, in 10K


# Plot the points after plotting all the best fit lines
mkr=['.','o',
     'v','s',
     's','v',
     'o','^',
     '^','h']
ms=[6,6,
    7,6,
    7,7,
    8,7,
    8,6]

fcount=0
for y,fil in enumerate(files):
    print ('Processing file: {0}'.format(fil))
    fcount = fcount+1
    nbins = nbins_g
    D = readDataset (fil)
    if D.size == 0:
        continue
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)
    # Plot points

    colo = plt.cm.brg((fcount*0.5)/len(files))
    if (fcount%graphskip == 0):
        a1.plot(b[:-1]/scale,np.log(h),'.',color=colo,marker=mkr[y],markersize=ms[y])

a1.legend(lbls,frameon=False)
a1.set_ylabel(r'log (evolutions)',fontsize=fs)
a1.set_xlabel('1000 generations',fontsize=fs)
a1.set_ylim([0,9])
a1.set_axisbelow(True)

# Slope vs p fit. Fit line to most of the points.
slope_fit = np.polyfit (M[1:,1], M[1:,0]/scale, 1)
slope_fit_fn = np.poly1d (slope_fit)

f1.tight_layout()
plt.savefig ('png/paper_figD.png')

plt.show()

# Plus add figure to show mean number of generations vs p.
