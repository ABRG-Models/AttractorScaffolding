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

#maxgens='100000000' # for debug
maxgens='1000000000' # lengthy, best graph
if driftnodrift == 'drift':
    filetag = ''
    files = ['../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.1.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.3.csv',
             '../data/evolve_a21_p10_'+ff+'_'+maxgens+'_gens_0.5.csv']
else:
    filetag = '_nodrift'
    files = ['../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.1.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.3.csv',
             '../data/evolve_nodrift_a21_p10_'+ff+'_'+maxgens+'_gens_0.5.csv']


lbls = ['p=0.10',
        'p=0.30',
        'p=0.50']

# num files
nf = len(files)

# Font size for plotting
fs=20

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs}
matplotlib.rc('font', **fnt)

f1 = plt.figure(figsize=(8,8)) # Figure object
a1 = f1.add_subplot (1,1,1)

M = np.zeros([nf,3])

# Global choice of nbins
nbins_g = 50

# Plot the points only for this version of the script
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
    bins = np.logspace (np.log10(np.min(D)), np.log10(np.max(D)), base=10, num=nbins)
    h,b = np.histogram (D, bins)
    # Plot points

    colo = plt.cm.brg((fcount*0.5)/len(files))
    a1.plot(np.log10(b[:-1]),np.log10(h),'.-',color=colo,marker=mkr[y],markersize=ms[y])

a1.legend(lbls,frameon=False)
a1.set_ylabel(r'log$_{10}$ (evolutions)',fontsize=fs)
a1.set_xlabel('log$_{10}$ (generations)',fontsize=fs)
#a1.set_ylim([0,4])
a1.set_axisbelow(True)

f1.tight_layout()
plt.savefig ('png/paper_figDloglog.png')

plt.show()

# Plus add figure to show mean number of generations vs p.
