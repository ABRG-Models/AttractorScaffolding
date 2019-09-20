#
# Plots the fitness vs generations "punctuated equilibria figure". The
# data that this script depends upon is generated by running
# evolve_nodrift_withf_ff4 (or evolve_withf_ff4 for drift) (which are
# both compiled from evolve.cpp).
#

from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import re
import subprocess
import states_lib as sl
import sebcolour
col = sebcolour.Colour

# Read csv files with gen/fitness data in
def readDataset (filepath):
    f = np.zeros([1,9])
    f_nozeros = np.zeros([1,9])
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            lastgen = 0
            first = True
            for row in rdr:
                if first:
                    lastgen = float(row[0])
                    first = False
                f[-1,0] = float(row[0]) # gen
                f[-1,1] = float(row[1]) # fitness
                #nets.append(row[2])     # genome
                f[-1,2] = float(row[3]) # num basins
                f[-1,3] = float(row[4]) # mean limit cycle length
                f[-1,4] = float(row[5]) # max limit cycle length
                f[-1,5] = float(row[6]) # num changed transitions
                f[-1,6] = float(row[7]) # change in fitness
                f[-1,7] = float(row[0]) - lastgen # interval length
                f[-1,8] = float(row[8]) # change in hamming dist
                if float(row[7]) != 0:
                    f_nozeros[-1,:] = f[-1,:]
                    f_nozeros = np.append(f_nozeros, np.zeros([1,9]), 0)
                f = np.append(f, np.zeros([1,9]), 0)

    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:], f_nozeros[:-1,:]

def readNets (filepath):
    nets = []
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            for row in rdr:
                nets.append(row[2])     # genome
    return nets

# 10000 or 100000 depending on p
FromValue = 1000
pOn = 0.1
driftnodrift = 'drift'

# Make files from directory listing
p = Path('../data/evolutions/')
if driftnodrift == 'drift':
    ftag = '_'
else:
    ftag = '_' + driftnodrift + '_'
globstr = '*evolve_withf'+ftag+'a21_p10_ff4_*_fitness_'+str(pOn)+'*.csv'
print (globstr)
files = list(p.glob(globstr))

# Font size for plotting
fs2=22 # for axes tick labels

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot(1,1,1)

# Set plot_states to 1 when deciding which of the state transition
# diagrams is most interesting and should be the "chosen genome" for
# the paper.
plot_states = 0

if plot_states:
    F2 = plt.figure (figsize=(24,16))

# Used when state plotting:
hexstates = False
kequalsn = True
docolour = False

fcount=0
maxtoshow=9
for f in files:
    A, B = readDataset(f)
    if fcount <= maxtoshow:
        # Find out where in A we have the -10000K generation & check its value
        From10K=A[np.where(A[:,0]>-(FromValue+1))]
        if From10K[0,1] < 0.2:
            #print ('file {0}'.format(f))
            f1.plot (A[:,0]/FromValue,A[:,1],'-',linewidth=2, color=plt.cm.plasma((fcount*0.8)/maxtoshow), label='{0}'.format(fcount))
            fcount = fcount + 1
            if plot_states:
                nethex_re = re.search('(.*)_genome_(.*).csv', '{0}'.format(f))
                cmd = '../build/sim/genome2str'
                args = nethex_re.group(2)
                net = subprocess.check_output ([cmd, args]).decode("utf-8")
                f2 = F2.add_subplot(3,4,fcount)
                sl.plot_states (net, f2.axes, hexstates, kequalsn, docolour)
                f2.set_title('{0} - {1}'.format(fcount-1, args), fontsize=10)


# Plot one of them in bold:
sfnum = 0
specialfile=files[sfnum]
print ('Special file: {0}'.format(specialfile))
A, B = readDataset(specialfile)
f1.plot (A[:,0]/FromValue,A[:,1],'-',linewidth=5, color=col.navy, label='{0}'.format(sfnum))
if plot_states:
    f1.legend()
f1.set_xlabel(str(FromValue) + ' generations');
f1.set_ylabel('fitness');
plt.savefig ('png/paper_figC.svg')

plt.show()
