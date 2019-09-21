import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
sys.path.insert (0, '../include')
import csv

# Read csv files.
def readDataset (filepath):
    numcols = 3
    f = np.zeros([1,numcols])
    with open (filepath, 'r') as csvfile:
            rdr = csv.DictReader (csvfile)
            for row in rdr:
                f[-1,0] = float(row['l'])
                f[-1,1] = float(row['p(!0)'])
                f[-1,2] = float(row['p(0)'])
                f = np.append(f, np.zeros([1,numcols]), 0)
    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]

files = ['../../data/pnot0_n3.csv',
         '../../data/pnot0_n4.csv',
         '../../data/pnot0_n5.csv',
         '../../data/pnot0_n6.csv',
         '../../data/pnot0_n7.csv',
         '../../data/pnot0_n9.csv',
         '../../data/pnot0_n12.csv',
         '../../data/pnot0_n15.csv',
         '../../data/pnot0_n18.csv',
         '../../data/pnot0_n25.csv'
]

lbls = ['n=3',
        'n=4',
        'n=5',
        'n=6',
        'n=7',
        'n=9',
        'n=12',
        'n=15',
        'n=18',
        'n=25'
]

mrkrs = ['o','s','v','^','h','o','s','v','^','h']

# A nice colour array
import sebcolour
col = sebcolour.Colour
cols1 = np.array([col.darkviolet,
                  col.darkviolet,
                  col.mediumorchid1,
                  col.mediumorchid1,
                  col.maroon1,
                  col.maroon1,
                  col.violetred,
                  col.violetred,
                  col.orange,
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

# F1 is a Figure instance
F1 = plt.figure (figsize=(12,12))

# fig1 is: <class 'matplotlib.axes._subplots.AxesSubplot'>
fig1 = F1.add_subplot(121)
fig2 = F1.add_subplot(122)

nc = 0;
for fl in files:
    D = readDataset (fl);
    fig1.plot (D[:,0],D[:,2], marker=mrkrs[nc], linestyle='-', color=cols1[nc], markersize=8)
    fig2.plot (D[:,0],D[:,1], marker=mrkrs[nc], linestyle='-', color=cols1[nc], markersize=8)
    nc = nc + 1

fig1.legend(lbls, frameon=False)
fig1.set_ylabel('Probability',fontsize=fs)
fig1.set_xlabel('Limit cycle length',fontsize=fs)
fig1.set_xlim([0,10])
#fig1.set_ylim([1,9])
fig1.set_axisbelow(True)
fig1.set_title ("a) Probability of score 0");

fig2.legend(lbls, frameon=False)
#fig2.set_ylabel('$p(0)$',fontsize=fs)
fig2.set_xlabel('Limit cycle length',fontsize=fs)
fig2.set_xlim([0,10])
#fig2.set_ylim([1,9])
fig2.set_axisbelow(True)
fig2.set_title ("b) Probability of score > 0");

plt.savefig ('figures/p0.svg')
plt.show()
