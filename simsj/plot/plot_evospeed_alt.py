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

filesff0 = ['../data/evolve_ff0_100000000_gens_0.05.csv',
            '../data/evolve_ff0_100000000_gens_0.1.csv',
            '../data/evolve_ff0_100000000_gens_0.15.csv',
            '../data/evolve_ff0_100000000_gens_0.2.csv',
            '../data/evolve_ff0_100000000_gens_0.25.csv',
            '../data/evolve_ff0_100000000_gens_0.3.csv',
            '../data/evolve_ff0_100000000_gens_0.35.csv',
            '../data/evolve_ff0_100000000_gens_0.4.csv',
            '../data/evolve_ff0_100000000_gens_0.45.csv',
            '../data/evolve_ff0_100000000_gens_0.5.csv']

filesff1 = ['../data/evolve_alt_ff0_100000000_gens_0.05.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.1.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.15.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.2.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.25.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.3.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.35.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.4.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.45.csv',
            '../data/evolve_alt_ff0_100000000_gens_0.5.csv']

#driftsff0 = ['../data/drift_ff0_100000000_gens_0.1.csv',
#             '../data/drift_ff0_100000000_gens_0.2.csv',
#             '../data/drift_ff0_100000000_gens_0.3.csv',
#             '../data/drift_ff0_100000000_gens_0.4.csv',
#             '../data/drift_ff0_100000000_gens_0.5.csv']
driftsff0 = ['../data/drift_ff0_100000000_gens_0.5.csv']

#driftsff1 = ['../data/drift_ff1_100000000_gens_0.1.csv',
#             '../data/drift_ff1_100000000_gens_0.2.csv',
#             '../data/drift_ff1_100000000_gens_0.3.csv',
#             '../data/drift_ff1_100000000_gens_0.4.csv',
#             '../data/drift_ff1_100000000_gens_0.5.csv']
driftsff1 = ['../data/drift_ff1_100000000_gens_0.5.csv']


lbls = ['p=0.10',
        'p=0.20',
        'p=0.30',
        'p=0.40',
        'p=0.50']

# A nice colour array
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
                  col.goldenrod1])


cols2 = np.array([col.navy,
                  col.navy,
                  col.royalblue4,
                  col.royalblue4,
                  col.royalblue1,
                  col.royalblue1,
                  col.dodgerblue,
                  col.dodgerblue,
                  col.darkturquoise,
                  col.darkturquoise])

cols3 = np.array([col.black,
                  col.black,
                  col.black,
                  col.black,
                  col.black,
                  col.black,
                  col.black,
                  col.black,
                  col.black,
                  col.black])

# Font size for plotting
fs=20
fs2=18 # for axes tick labels

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

scale = 10000.
# Global choice of nbins
nbins_g = 50

def plot_data (f1, f2, files, mrkr, colr, cols):
    # num files:
    nf = len(files)
    M = np.zeros([nf,2])
    # Plot EVERY OTHER line. These are what will appear in the legend
    fcount = 0
    for y,file in enumerate(files):
        fcount = fcount + 1
        D = readDataset (file)
        bins = np.linspace(1,0.5*np.max(D),nbins_g)
        h,b = np.histogram (D, bins)

        # Do a fit
        x = np.where(np.log(h)>0)[0]

        bx = b[x]/scale

        fit = np.polyfit (bx, np.log(h[x]), 1)
        fit_fn = np.poly1d (fit)

        # Slop is fit[0], Record for a later graph.
        M[y,0] = (y+1)*0.05
        M[y,1] = fit[0]

        # Only actually plot every other one:
        if nf != 10:
            f1.plot(bx,fit_fn(bx),'-',linewidth=1,color=cols[y*2])
        else:
            if (fcount % 2 == 0):
                f1.plot(bx,fit_fn(bx),'-',linewidth=1,color=cols[y])

    # Plot the points after plotting all the best fit lines
    fcount = 0
    for y,file in enumerate(files):
        fcount = fcount + 1
        D = readDataset (file)
        bins = np.linspace(1,0.5*np.max(D),nbins_g)
        h,b = np.histogram (D, bins)
        # Plot points for every other dataset
        if nf != 10:
            f1.plot(b[:-1]/scale,np.log(h),'.',color=cols[y*2],markersize=8)
        else:
            if (fcount % 2 == 0):
                f1.plot(b[:-1]/scale,np.log(h),'.',color=cols[y],markersize=10)

    #print (M)
    # Plot the points on the right hand sub plot
    f2.plot (M[:,0], M[:,1], marker=mrkr, linestyle='None', color=colr, markersize=12)

# F1 is a Figure instance
F1 = plt.figure (figsize=(12,12))

# fig1 is: <class 'matplotlib.axes._subplots.AxesSubplot'>
fig1 = F1.add_subplot(221)
fig2 = F1.add_subplot(222)
fig3 = F1.add_subplot(212)

plot_data (fig1, fig3, filesff0, 'o', col.violetred1, cols1) # ff0
plot_data (fig1, fig3, driftsff0, 'o', col.royalblue2, cols3)

plot_data (fig2, fig3, filesff1, 's', col.royalblue2, cols2) # ff1
plot_data (fig2, fig3, driftsff1, 'o', col.violetred1, cols3)

fig1.legend(lbls, frameon=False)
fig1.set_ylabel(r'$\log(\rm{frequency})$',fontsize=fs)
fig1.set_xlabel('10K generations',fontsize=fs)
#fig1.set_aspect(np.diff(fig1.get_xlim())/np.diff(fig1.get_ylim()))
fig1.set_xlim([0,20])
fig1.set_ylim([1,9])
fig1.set_axisbelow(True)
fig1.set_title ("a) Fitness function 0");

fig2.legend(lbls, frameon=False)
#fig2.set_ylabel(r'$\log(\rm{frequency})$',fontsize=fs)
fig2.set_xlabel('10K generations',fontsize=fs)
#fig3.set_aspect(np.diff(fig3.get_xlim())/np.diff(fig3.get_ylim()))
fig2.set_xlim([0,20])
fig2.set_ylim([1,9])
fig2.set_axisbelow(True)
fig2.set_title ("b) Alternative target");

fig3.set_xlabel('Flip probability, p')
fig3.set_ylabel('Slopes of best fit lines from (a) and (b)')
fig3.legend(['FF0','FF1'], frameon=False)
fig3.set_xlim([0,0.5])

plt.savefig ('evospeed_alttarget.png')
plt.show()
