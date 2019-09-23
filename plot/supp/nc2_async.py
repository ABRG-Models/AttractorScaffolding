import sys
sys.path.insert(0,'../include') # To get states lib and sebcolour
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
import csv

# Change this to choose which to plot.
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

directry = 'data'
contexttag = 'nc2_async_I16-0_T21-10'
maxgens='100000000'

# Make file names
files = []
filetag = ''
for pp in p:
    print ('Append file for p={0}'.format(pp))
    files.append ('../../{4}/evolve_{3}_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))

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
nbins_g = 25

# Holds lines so that legends can be made
lines = []
fcount = 0

# Plot one point only for each set; this is what will give us the
# legend looking sensible, with both a point and a line. This is a
# dirty hack! Complain to matplotlib legend developers.
for y,fil in enumerate(files):
    fcount = fcount+1
    nbins = nbins_g
    D = readDataset (fil)
    if D.size == 0:
        continue
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)
    # Plot points
    colo = plt.cm.brg((fcount*0.5)/len(files))
    pp = a1.plot(b[0]/scale,np.log(h)[0],'.-',color=colo,marker=mkr[y],markersize=ms[y])

fcount = 0
# Plot points proper
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
    pp = a1.plot(b[:-1]/scale,np.log(h),'.',color=colo,marker=mkr[y],markersize=ms[y])

# Plot the best fit lines.
fcount = 0
printlines = 1
for y,fil in enumerate(files):
    print ('Processing best fit for file: {0}'.format(fil))
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
        if printlines:
            print ('Plotting file {0}'.format(fil))
            ll = a1.plot(bx,fit_fn(bx),'-',linewidth=2,color=colo)

    else:
        M[y,0] = 0     # no slope known

    M[y,1] = (y+1)*0.05 # p, the flip probability.
    M[y,2] = np.mean(D) # mean generations, in 10K

a1.legend(lbls,frameon=False)

a1.set_ylabel(r'log (evolutions)',fontsize=fs)
a1.set_xlabel('1000 generations',fontsize=fs)
a1.set_ylim([0,10])
a1.set_xlim([-5,200])
a1.set_axisbelow(True)

# Slope vs p fit. Fit line to most of the points.
slope_fit = np.polyfit (M[1:,1], M[1:,0]/scale, 1)
slope_fit_fn = np.poly1d (slope_fit)

f1.tight_layout()

plt.savefig ('figures/nc2_async.png')
plt.savefig ('figures/nc2_async.svg')

plt.show()

# Plus add figure to show mean number of generations vs p.
