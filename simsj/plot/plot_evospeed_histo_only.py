import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
import csv

# Change this to choose which to plot.
driftnodrift = 'nodrift' # 'nodrift' or 'drift'

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

if driftnodrift == 'drift':
    filetag = ''
    files = ['../data/evolve_a21_p10_ff4_100000000_gens_0.05.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.1.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.15.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.2.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.25.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.3.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.35.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.4.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.45.csv',
             '../data/evolve_a21_p10_ff4_100000000_gens_0.5.csv']
else:
    filetag = '_nodrift'
    files = ['../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.05.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.1.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.15.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.2.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.25.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.3.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.35.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.4.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.45.csv',
             '../data/evolve_nodrift_a21_p10_ff4_100000000_gens_0.5.csv']


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

print ('2nd element {0}'.format(lbls[2]))

# num files
nf = len(files)

# A nice colour array
import sebcolour
col = sebcolour.Colour

graphskip = 1 # or 2 to skip every other.

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

# Global choice of nbins
nbins_g = 50

# Plot the lines. These are what will appear in the legend
fcount=0
gcount=0
a1 = []
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
        fit = np.polyfit (np.log(bx), np.log(h[x]), 1)
        fit_fn = np.poly1d (fit)

        # Slope is fit[0], Record for a later graph.
        M[y,0] = fit[0]     # slope
        colo = plt.cm.plasma(fcount/10)
        if (fcount%graphskip == 0):
            print ('Plotting file {0}'.format(fil))
            ax = f1.add_subplot (2,5,gcount+1)
            a1.append(ax)
            a1[gcount].plot(np.log(bx),fit_fn(np.log(bx)),'-',linewidth=2,color=colo)
            gcount = gcount + 1
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
gcount=0
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

    colo = plt.cm.plasma(fcount/10)
    if (fcount%graphskip == 0):
        a1[gcount].plot(np.log(b[:-1]/scale),np.log(h),'.',color=colo,marker=mkr[y],markersize=ms[y])
        a1[gcount].set_title(lbls[gcount])
        a1[gcount].set_ylabel(r'log (evolutions)',fontsize=fs)
        a1[gcount].set_xlabel('1000 generations',fontsize=fs)
        a1[gcount].set_axisbelow(True)
        gcount = gcount + 1

####a1.set_xlim([0,260])
####a1.set_ylim([-2,10])
#a1.set_aspect(np.diff(a1.get_xlim())/np.diff(a1.get_ylim()))
#print (M)
#f1.tight_layout()
#plt.savefig ('distribution_evolutionary_histo_' + filetag + 'ff4.png')


f1.tight_layout()
plt.savefig ('evolution_histo_only' + filetag + '_ff4.png')

plt.show()

# Plus add figure to show mean number of generations vs p.
