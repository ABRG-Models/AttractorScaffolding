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

lbls = ['p=0.10',
        'p=0.20',
        'p=0.30',
        'p=0.40',
        'p=0.50']

# num files
nf = len(files)

# A nice colour array
import sebcolour
col = sebcolour.Colour
cols = np.array([col.black,
                 col.indigo, #
                 col.black,
                 col.purple3, #
                 col.black,
                 col.mediumorchid1, #
                 col.black,
                 col.orange, #
                 col.black,
                 col.crimson]) #

# Font size for plotting
fs=22

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs}
matplotlib.rc('font', **fnt)

scale = 10000.

f1 = plt.figure(figsize=(8,8)) # Figure object

M = np.zeros([nf,3])

# Global choice of nbins
nbins_g = 50

# Plot the lines. These are what will appear in the legend
fcount=0
for y,file in enumerate(files):
    fcount = fcount+1
    nbins = nbins_g
    D = readDataset (file)
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)

    # Do a fit
    x = np.where(np.log(h)>0)[0]

    bx = b[x]/scale

    fit = np.polyfit (bx, np.log(h[x]), 1)
    fit_fn = np.poly1d (fit)

    # Slope is fit[0], Record for a later graph.
    M[y,0] = fit[0]     # slope
    M[y,1] = (y+1)*0.05 # p, the flip probability.
    M[y,2] = np.mean(D) # mean generations, in 10K

    if (fcount%2 == 0):
        print ('Plotting file {0}'.format(file))
        plt.plot(bx,fit_fn(bx),'-',linewidth=2,color=cols[y])

# Plot the points after plotting all the best fit lines
mkr=['.','o',
     '.','s',
     '.','v',
     '.','^',
     '.','h']
ms=[1,6,
    1,6,
    1,7,
    1,7,
    1,6]

fcount=0
for y,file in enumerate(files):
    fcount = fcount+1
    nbins = nbins_g
    D = readDataset (file)
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)
    # Plot points
    if (fcount%2 == 0):
        plt.plot(b[:-1]/scale,np.log(h),'.',color=cols[y],marker=mkr[y],markersize=ms[y])

a1 = f1.axes[0]
a1.legend(lbls,frameon=False)
a1.set_ylabel(r'log (frequency)',fontsize=fs)
a1.set_xlabel('10K generations',fontsize=fs)
#a1.set_aspect(np.diff(a1.get_xlim())/np.diff(a1.get_ylim()))
a1.set_axisbelow(True)
#print (M)
f1.tight_layout()
plt.savefig ('distribution_evolutionary_histo_ff4.png')

# Slope vs p fit. Fit line to most of the points.
slope_fit = np.polyfit (M[1:,1], M[1:,0], 1)
slope_fit_fn = np.poly1d (slope_fit)

# Slope vs p
f2 = plt.figure(figsize=(8,8))
plt.plot (M[0,1], M[0,0], marker='h', linestyle='None', color=col.gray20, markersize=12)
plt.plot (M[1:,1], M[1:,0], marker='o', linestyle='None', color=col.gray20, markersize=12)
plt.plot (M[:,1], slope_fit_fn(M[:,1]), marker='None', linestyle='-', color=col.gray20, markersize=12)
a2 = f2.axes[0]
a2.set_xlabel('p')
a2.set_ylabel('Slope of best fit line')
a2.set_xlim([0,0.55])
a2.set_ylim([-1.8,0])
f2.tight_layout()
plt.savefig ('distribution_evolutionary_slope_vs_p_ff4.png')

# Mean vs p fit. Fit exponential to the mean number of gens, excluding
# the first point.
log_means = np.log(M[1:,2])
means_fit = np.polyfit (M[1:,1], log_means, 1)
print ('Exponential fit to curve: {0:.2f} exp ({1:.2f}*p)'.format(np.exp(means_fit[1]), means_fit[0]))
expx = np.arange(0.01,0.54,0.001)
expfit = np.exp (means_fit[1]) * np.exp (means_fit[0]*expx)


# Mean vs p
f3 = plt.figure(figsize=(8,8))
plt.plot (M[0,1], M[0,2]/10000, marker='h', linestyle='None', color=col.gray20, markersize=12)
plt.plot (M[1:,1], M[1:,2]/10000, marker='o', linestyle='None', color=col.gray20, markersize=12)
plt.plot (expx, expfit/10000, marker='None', linestyle='-', color=col.gray20, markersize=9)
a3 = f3.axes[0]
a3.set_xlabel('p')
a3.set_ylabel('Mean generations to evolve $f=1$ (x10K)')
a3.set_xlim([0,0.55])
a3.set_ylim([0,7.5])
f3.tight_layout()
plt.savefig ('distribution_evolutionary_mean_vs_p_ff4.png')

plt.show()

# Plus add figure to show mean number of generations vs p.
