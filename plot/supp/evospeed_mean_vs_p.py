import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
sys.path.insert (0, '../include')
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

expfitstartidx = 2 # index of 0.05 in p, below
p = [0.03, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5]
directry='../../data'
# 2 contexts
maxgens='100000000'
contexttag = 'nc2_I16-0_T21-10'
filesc2 = []
for pp in p:
    filesc2.append ('{4}/evolve_{3}_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))
nfc2 = len(filesc2)

# 3 contexts
contexttag = 'nc3_I16-4-1_T20-5-10'
maxgens='1000000000'
filesc3 = []
for pp in p:
    filesc3.append ('{4}/evolve_{3}_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))
nfc3 = len(filesc3)

# 3 contexts
contexttag = 'nc4_I16-4-1-8_T20-10-5-9'
maxgens='5000000000'
filesc4 = []
p = [0.03, 0.05, 0.1, 0.15, 0.2]
for pp in p:
    filesc4.append ('{4}/evolve_{3}_{0}_{1}_gens_{2}.csv'.format(ff, maxgens, pp, contexttag, directry))
nfc4 = len(filesc4)

# Reset p, hack hack
p = [0.03, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5]

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

f1 = plt.figure(figsize=(10,8)) # Figure object

# Global choice of nbins
nbins_g = 50

def compute_slopes (files, nf):
    M = np.zeros([nf,3])
    # Plot the lines. These are what will appear in the legend
    for y,fil in enumerate(files):
        print ('Processing file: {0}'.format(fil))
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
        if x.size > 0:
            bx = b[x]/scale
            fit = np.polyfit (bx, np.log(h[x]), 1)
            fit_fn = np.poly1d (fit)
            # Slope is fit[0], Record for a later graph.
            M[y,0] = fit[0]     # slope
        else:
            M[y,0] = 0     # no slope known

        M[y,1] = p[y] # p, the flip probability.
        M[y,2] = np.mean(D) # mean generations, in 10K
    return M

#M2 = compute_slopes (filesc2, nfc2)
#M3 = compute_slopes (filesc3, nfc3)
#M4 = compute_slopes (filesc4, nfc4)
#np.save('tmp/M2.npy', M2)
#np.save('tmp/M3.npy', M3)
#np.save('tmp/M4.npy', M4)
M2 = np.load ('tmp/M2.npy')
M3 = np.load ('tmp/M3.npy')
M4 = np.load ('tmp/M4.npy')


# Mean vs p fit. Fit exponential to the mean number of gens, excluding
# the first couple of points
log_means2 = np.log(M2[expfitstartidx:,2])
means_fit2 = np.polyfit (M2[expfitstartidx:,1], log_means2, 1)
print ('Exponential fit to curve: {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit2[1]), means_fit2[0]))
expx2 = np.arange(0.05,0.5,0.001)
expfit2 = np.exp (means_fit2[1]) * np.exp (means_fit2[0]*expx2)

log_means3 = np.log(M3[expfitstartidx:,2])
means_fit3 = np.polyfit (M3[expfitstartidx:,1], log_means3, 1)
print ('Exponential fit to curve: {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit3[1]), means_fit3[0]))
expx3 = np.arange(0.05,0.5,0.001)
expfit3 = np.exp (means_fit3[1]) * np.exp (means_fit3[0]*expx3)

log_means4 = np.log(M4[expfitstartidx:,2])
means_fit4 = np.polyfit (M4[expfitstartidx:,1], log_means4, 1)
print ('Exponential fit to curve: {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit4[1]), means_fit4[0]))
expx4 = np.arange(0.05,0.5,0.001)
expfit4 = np.exp (means_fit4[1]) * np.exp (means_fit4[0]*expx4)


# Mean vs p
scale = 1 # change scale for this subplot
a3 = f1.add_subplot (1,1,1)
a3.plot (M2[:,1], M2[:,2]/scale, marker='o', linestyle='None', color=col.gray20, markersize=12)
a3.plot (expx2, expfit2/scale, marker='None', linestyle='-', color=col.crimson, markersize=9)

a3.plot (M3[:,1], M3[:,2]/scale, marker='o', linestyle='None', color=col.gray20, markersize=12)
a3.plot (expx3, expfit3/scale, marker='None', linestyle='-', color=col.royalblue, markersize=9)

a3.plot (M4[:,1], M4[:,2]/scale, marker='o', linestyle='None', color=col.gray20, markersize=12)
a3.plot (expx4, expfit4/scale, marker='None', linestyle='-', color=col.turquoise3, markersize=9)

# Plot a point showing the mean number of f=1 genomes when randomly sampling
# 2 contexts: 69120
# 3 contexts: ?
# 4 contexts: ?
a3.plot ([0,0.5], [69120,69120], marker='None', linestyle='-', color=col.navy)
a3.legend(['Mean gens to evolve','Fit: {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit2[1]), means_fit2[0]),'Random discovery rate'])

a3.set_xlabel('p')
a3.set_ylabel('$\mu$ (mean gens to evolve $f=1$)')
a3.set_ylim([0,500000])

f1.tight_layout()
plt.savefig ('figures/evospeed_mean_vs_p.png')
#plt.savefig ('figures/evospeed_mean_vs_p.svg')

plt.show()

# Plus add figure to show mean number of generations vs p.
