import numpy as np
import pylab as pl
import matplotlib
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

files = ['../data/evolve_ff0_100000000_gens_0.05.csv',
         '../data/evolve_ff0_100000000_gens_0.1.csv',
         '../data/evolve_ff0_100000000_gens_0.15.csv',
         '../data/evolve_ff0_100000000_gens_0.2.csv',
         '../data/evolve_ff0_100000000_gens_0.25.csv',
         '../data/evolve_ff0_100000000_gens_0.3.csv',
         '../data/evolve_ff0_100000000_gens_0.35.csv',
         '../data/evolve_ff0_100000000_gens_0.4.csv',
         '../data/evolve_ff0_100000000_gens_0.45.csv',
         '../data/evolve_ff0_100000000_gens_0.5.csv']

lbls = ['0.05',
        '0.10',
        '0.15',
        '0.20',
        '0.25',
        '0.30',
        '0.35',
        '0.40',
        '0.45',
        '0.50']

# num files
nf = len(files)

# A nice colour array
import sebcolour
col = sebcolour.Colour
cols = np.array([col.darkviolet,
                 col.darkorchid1,
                 col.mediumorchid1,
                 col.orchid,
                 col.maroon1,
                 col.violetred1,
                 col.violetred,
                 col.crimson,
                 col.orange,
                 col.goldenrod1])

# Font size for plotting
fs=20
fs2=18 # for axes tick labels

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

scale = 10000.

F1 = pl.figure(figsize=(15,7.5))
f1 = F1.add_subplot(121)

M = np.zeros([nf,2])

# Global choice of nbins
nbins_g = 20

# Plot the lines. These are what will appear in the legend
for y,file in enumerate(files):
    nbins = nbins_g
    D = readDataset (file)
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)

    # Do a fit
    x = np.where(np.log(h)>0)[0]

    bx = b[x]/scale

    fit = np.polyfit (bx, np.log(h[x]), 1)
    fit_fn = np.poly1d (fit)

    # Slop is fit[0], Record for a later graph.
    M[y,0] = fit[0]
    M[y,1] = (y+1)*0.05

    f1.plot(bx,fit_fn(bx),'-',linewidth=1,color=cols[y])

# Plot the points after plotting all the best fit lines
for y,file in enumerate(files):
    nbins = nbins_g
    D = readDataset (file)
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)
    # Plot points
    f1.plot(b[:-1]/scale,np.log(h),'.',color=cols[y],markersize=10)

f1.legend(lbls, title='Flip probability')

f1.set_ylabel(r'$\log(Histogram frequency)$',fontsize=fs)
f1.set_xlabel('10K generations',fontsize=fs)
#f1.set_aspect(np.diff(f1.get_xlim())/np.diff(f1.get_ylim()))
f1.set_axisbelow(True)

print (M)

f2 = F1.add_subplot(122)
f2.plot (M[:,1], M[:,0], marker='o', linestyle='None', color=col.gray20, markersize=12)
f2.set_xlabel('Flip probability')
f2.set_ylabel('Slope of best fit line from (a)')

F1.tight_layout()
pl.title('Fitness function 0 (SW)')
pl.savefig ('evospeed_ff0.png')
pl.show()
