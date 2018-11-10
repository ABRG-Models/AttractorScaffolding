from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
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
                  col.goldenrod1,
                  col.darkviolet,
                  col.darkorchid1,
                  col.mediumorchid1,
                  col.orchid,
                  col.maroon1,
                  col.violetred1,
                  col.violetred,
                  col.crimson,
                  col.orange,
                  col.goldenrod1])

# Read csv files with gen/fitness data in
def readDataset (filepath):
    f = np.zeros([1,7])
    nets = []
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            for row in rdr:
                f[-1,0] = float(row[0])
                f[-1,1] = float(row[1])
                f[-1,2] = float(row[3]) # num basins
                f[-1,3] = float(row[4]) # mean limit cycle length
                f[-1,4] = float(row[5]) # max limit cycle length
                f[-1,5] = float(row[6]) # num changed transitions
                f[-1,6] = float(row[7]) # change in fitness

                f = np.append(f, np.zeros([1,7]), 0)
                nets.append(row[2])

    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:], nets

# Make files from directory listing
p = Path('../data')
files = list(p.glob('*evolve_withf*_fitness_*.csv'))

# Font size for plotting
fs2=22 # for axes tick labels

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot(1,1,1)

fcount=0
for f in files:
    if fcount > 8:
        break
    A, nets = readDataset(f)
    # Find out where in A we have the -10000K generation & check its value
    From10K=A[np.where(A[:,0]>-10001)]
    if From10K[0,1] < 0.2:
        f1.plot (A[:,0]/10000,A[:,1],'-',linewidth=2, color=cols1[fcount])
        fcount = fcount + 1

# Plot one of them in bold:
specialfile=files[8]
A, nets = readDataset(specialfile)
f1.plot (A[:,0]/10000,A[:,1],'-',linewidth=5, color=col.navy)

f1.set_xlim([-1, 0.01])
f1.set_xlabel('10K generations');
f1.set_ylabel('fitness');
#F1.tight_layout()
plt.savefig ('evolution_boolean_ff4.png')
#plt.show()

# Set a default fontsize for matplotlib:
fs=12
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs}
matplotlib.rc('font', **fnt)

# For the specialfile, also show its states, using Dan's code.
import states_lib as sl

fcount = 0
for f in files:
    if fcount > 0:
        break
    A, nets = readDataset(f)
    nc = 0
    gc = 0
    last_fitness = -1
    F2 = plt.figure (figsize=(25,15))
    for net in nets:
        if (A[nc,1] > last_fitness):
            last_fitness = A[nc,1]
            f2 = F2.add_subplot (5,6,gc+1)
            sl.plot_states (net, f2.axes)
            f2.axes.set_title('{0}: f={1:.4f}'.format(gc, A[nc,1]))
            gc = gc + 1
        last_fitness = A[nc,1]
        nc = nc + 1
    fcount = fcount + 1

plt.show()
