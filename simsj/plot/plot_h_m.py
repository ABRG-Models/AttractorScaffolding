import numpy as np
import matplotlib
matplotlib.use('TkAgg',warn=False, force=True)
from matplotlib import pyplot as plt
import pylab as pl
import sys
import csv

# Read csv files.
def readDataset (filepath):
    numcols = 3
    f = np.zeros([1,numcols])
    with open (filepath, 'r') as csvfile:
            rdr = csv.DictReader (csvfile)
            for row in rdr:

                f[-1,0] = float(row['k'])
                f[-1,1] = float(row['m'])
                f[-1,2] = float(row['h(m)'])
                f = np.append(f, np.zeros([1,numcols]), 0)
    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]

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

m_h = readDataset("../data/h5_m.csv")

plot_log = False

F1 = pl.figure(figsize=(10,7.5))
f1 = F1.add_subplot(111)

k_max = int(np.max(m_h[:,0]))
print (k_max)
for k in range(1,k_max-1):
    m_tmp = m_h[m_h[:,0]==float(k)]
    if plot_log:
        f1.plot(m_tmp[:,1],
                np.log(m_tmp[:,2]),'-',marker='.',markersize=12,linewidth=1,color=cols[k%10])
    else:
        f1.plot(m_tmp[:,1],
                m_tmp[:,2],'-',marker='.',markersize=12,linewidth=1,color=cols[k%10])


f1.set_xlabel('m',fontsize=fs2)
if plot_log:
    f1.set_ylabel('log(m(h))',fontsize=fs2)
else:
    f1.set_ylabel('m(h)',fontsize=fs2)

#    lbls = ['1','2','3','4','5','6','7','8','9']
#    plt.legend(lbls, title='k')
# plt.savefig('h_m.png');
pl.show()
