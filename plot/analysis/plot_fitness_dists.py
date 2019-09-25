#
# Plot fitness distributions for various fitness functions
#

from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sys
sys.path.insert(0,'../include')
import sebcolour
col = sebcolour.Colour
from scipy import stats

# Read csv files with gen/fitness data in

def doPlot (ff, f1):

    D = np.loadtxt('../../data/fitness_dist_{0}_n10000000.csv'.format(ff), delimiter=',')

    # Prepare data for the histograms

    bin_means, bin_edges, binnumber = stats.binned_statistic (D[:,0], D[:,1], statistic='mean', bins=100)

    #f1.scatter (D[:,0], np.log(D[:,1]))
    print(np.size(bin_edges))
    print(np.size(bin_means))
    f1.plot (bin_edges[:-1], np.log(bin_means))

    f1.set_xlabel('Fitness');
    f1.set_ylabel('log(Count)');
    f1.set_title ('{0}'.format(ff))

# Font size for plotting
fs2=14 # for axes tick labels

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

F1 = plt.figure (figsize=(9,16))
f1 = F1.add_subplot(4,1,1)
doPlot ('ff4', f1)
# Other fitness distributions are possible:
#f2 = F1.add_subplot(4,1,2)
#doPlot ('ff5', f2)
#f3 = F1.add_subplot(4,1,3)
#doPlot ('ff6', f3)
#f4 = F1.add_subplot(4,1,4)
#doPlot ('ff8', f4)
F1.tight_layout()
plt.savefig ('figures/fitness_dists.png')

plt.show()
