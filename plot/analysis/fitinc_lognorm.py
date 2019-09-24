import sys
sys.path.insert(0,'../include') # To get states lib
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
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


p = [0.1, 0.2, 0.3, 0.4, 0.5]
directry = 'data_fitinc'
contexttag = 'nc2_I16-0_T21-10'
maxgens='10000000'

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

mkr=['o','s','v','^','h',  'o','s','v','^','h']
ms= [ 9,  9,  9,  9,  9,    9,  9,  9,  9,  9 ]

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
fcount=0
for y,fil in enumerate(files):
    print ('Processing file: {0}'.format(fil))
    fcount = fcount+1
    nbins = nbins_g
    D = readDataset (fil)
    if D.size == 0:
        continue
    logD = np.log10(D)
    bins = np.linspace (0, np.max(logD), num=nbins)

    # Save logD and bins
    np.save ('tmp/logD{0}.npy'.format(y), logD)
    np.save ('tmp/bins{0}.npy'.format(y), bins)

    h,b = np.histogram (logD, bins)
    # Plot points

    colo = plt.cm.brg((fcount*0.5)/len(files))
    a1.plot(b[:-1],h,'.-',color=colo,marker=mkr[y],markersize=ms[y])

a1.legend(lbls,frameon=False)
a1.set_ylabel(r'Fitness increments',fontsize=fs)
a1.set_xlabel('log$_{10}$ (generations)',fontsize=fs)
#a1.set_ylim([0,4])
a1.set_axisbelow(True)

f1.tight_layout()
plt.savefig ('figures/fitinc_lognorm.png')
plt.savefig ('figures/fitinc_lognorm.svg')

plt.show()

# Plus add figure to show mean number of generations vs p.
