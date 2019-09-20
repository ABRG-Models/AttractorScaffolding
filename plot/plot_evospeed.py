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

expfitstartidx = 4 # index of 0.05 in p, below
p = [0.03, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5]

maxgens='100000000'

# Make file names
files = []
if driftnodrift == 'drift':
    filetag = ''
    for pp in p:
        files.append ('../data/evolve_nc2_I16-0_T21-10_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp))
else:
    filetag = '_nodrift'
    for pp in p:
        files.append ('../data/evolve_nodrift_a21_p10_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp))

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
a1 = f1.add_subplot (1,3,1)

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
        print ('D size 0, continue...')
        continue
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)

    # Do a fit
    x = np.where(np.log(h)>0)[0]
    if x.size == 0:
        print ('x size 0, continue...')
        continue
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
        colo = plt.cm.brg((fcount)/len(files))
        if printlines:
            print ('Plotting file {0}'.format(fil))
            a1.plot(bx,fit_fn(bx),'-',linewidth=2,color=colo)
    else:
        M[y,0] = 0     # no slope known

    M[y,1] = p[y] # p, the flip probability.
    M[y,2] = np.mean(D) # mean generations, in 10K


# Plot the points after plotting all the best fit lines
mkr=['.','o',
     'v','s',
     's','v',
     'o','^',
     '^','h',
     '.','o',
     'v','s',
     's','v',
     'o','^',
     '^','h']
ms=[6,6,
    7,6,
    7,7,
    8,7,
    8,6,
    6,6,
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
        print ('D empty for file {0}'.format(fil))
        continue
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)

    # Do a fit just to see if we should include this one.
    x = np.where(np.log(h)>0)[0]
    if x.size == 0:
        print ('x size 0, continue...')
        continue

    # Plot points
    colo = plt.cm.brg((fcount)/len(files))
    a1.plot(b[:-1]/scale,np.log(h),'.',color=colo,marker=mkr[y],markersize=ms[y])

a1.legend(lbls,frameon=False)
a1.set_ylabel(r'log (evolutions) [' + driftnodrift + ']',fontsize=fs)
a1.set_xlabel('1000 generations',fontsize=fs)
a1.set_axisbelow(True)

# Slope vs p fit. Fit line to most of the points.
slope_fit = np.polyfit (M[1:,1], M[1:,0]/scale, 1)
slope_fit_fn = np.poly1d (slope_fit)

# Slope vs p
a2 = f1.add_subplot (1,3,2)
#a2.plot (M[0,1], (M[0,0]/scale), marker='h', linestyle='None', color=col.gray20, markersize=12)
#a2.plot (M[1:,1], (M[1:,0]/scale), marker='o', linestyle='None', color=col.gray20, markersize=12)
a2.plot (M[:,1], (M[:,0]/scale), marker='o', linestyle='None', color=col.gray20, markersize=12)
##a2.plot (M[:,1], slope_fit_fn(M[:,1]), marker='None', linestyle='-', color=col.gray20, markersize=12)
a2.set_xlabel('p')
a2.set_ylabel('m (slope of best fit) [' + driftnodrift + ']')
#a2.set_xlim([0,0.55])
#a2.set_ylim([-0.001,0])

# Mean vs p fit. Fit exponential to the mean number of gens, excluding
# the first couple of points
log_means = np.log(M[expfitstartidx:,2])
means_fit = np.polyfit (M[expfitstartidx:,1], log_means, 1)
print ('Exponential fit to curve ({2}): {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit[1]), means_fit[0], driftnodrift))
expx = np.arange(0.05,0.5,0.001)
expfit = np.exp (means_fit[1]) * np.exp (means_fit[0]*expx)


# Mean vs p
scale = 1 # change scale for this subplot
a3 = f1.add_subplot (1,3,3)
#a3.plot (M[0,1], M[0,2]/scale, marker='h', linestyle='None', color=col.gray20, markersize=12)
a3.plot (M[:,1], M[:,2]/scale, marker='o', linestyle='None', color=col.gray20, markersize=12)
a3.plot (expx, expfit/scale, marker='None', linestyle='-', color=col.crimson, markersize=9)
# Plot a point showing the mean number of f=1 genomes when randomly sampling
# 1451/1 x 10^8 gives 68918
# 5516/4 x 10^8 gives 72516
# 5787/4 x 10^8 gives 69120 (repeat)
a3.plot ([0,0.8], [69120,69120], marker='None', linestyle='-', color=col.navy)
a3.legend(['Mean gens to evolve','Fit: {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit[1]), means_fit[0], driftnodrift),'Random discovery rate'])
a3.set_xlabel('p')
a3.set_ylabel('$\mu$ (mean gens to evolve [' + driftnodrift + '] $f=1$)')
a3.set_xlim([0,0.62])
#a3.set_ylim([0,180000])

f1.tight_layout()
plt.savefig ('png/evospeed' + filetag + '_'+ff+'.svg')

plt.show()

# Plus add figure to show mean number of generations vs p.
