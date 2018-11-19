import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sebcolour
col = sebcolour.Colour

# Read csv files with gen/fitness data in
def readDataset (filepath):
    f = np.zeros([1,7])
    #nets = []
    with open (filepath, 'r') as csvfile:
        rdr = csv.reader (csvfile)
        for row in rdr:
            f[-1,0] = float(row[0])
            f[-1,1] = float(row[1]) # fitness
            # row[2] is genome, not used here.
            #nets.append(row[2])
            f[-1,2] = float(row[3]) # num basins
            f[-1,3] = float(row[4]) # mean limit cycle length
            f[-1,4] = float(row[5]) # max limit cycle length
            f[-1,5] = float(row[6]) # num changed transitions
            f[-1,6] = float(row[7]) # change in fitness

            f = np.append(f, np.zeros([1,7]), 0)

    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]#, nets

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : 14}
matplotlib.rc('font', **fnt)

F1 = plt.figure (figsize=(8,8))

A = readDataset('../data/null_withf_a21_p10_ff4_100000_fitness_0.csv')

f1 = F1.add_subplot(2,2,1)
f1.plot (A[:,0]/10000,A[:,1],marker='o',linestyle='None',linewidth=2,color=col.darkviolet)
f1.set_xlabel('10K generations');
f1.set_ylabel('fitness');

# Num basin vs fitness
f2 = F1.add_subplot(2,2,2)
f2.plot (A[:,2],A[:,1],marker='o',linestyle='None',linewidth=2,color=col.maroon1)
f2.set_ylabel('fitness');
f2.set_xlabel('Num limit cycles in system');

# Mean limit cycle length
f3 = F1.add_subplot(2,2,3)
f3.plot (A[:,3],A[:,1],marker='o',linestyle='None',linewidth=2,color=col.violetred)
f3.set_ylabel('fitness');
f3.set_xlabel('Mean limit cycle size');

# Max limit cycle length
f4 = F1.add_subplot(2,2,4)
f4.plot (A[:,4],A[:,1],marker='o',linestyle='None',linewidth=2,color=col.crimson)
f4.set_ylabel('fitness');
f4.set_xlabel('Max limit cycle size');

#f1.set_xlim([-1, 0.01])
F1.tight_layout()
plt.savefig ('null_model_stats_ff4.png')

F2 = plt.figure (figsize=(8,8))
# For each unique value in A[:,2], do a histogram of the fitnesses in A[:,1]
print ('A col 2 has length {0}'.format(len(A[:,2])))
lcs = np.unique(A[:,2])
print ('{0} unique values in A[:,2]: {1}'.format(len(lcs), lcs))
a1 = []
gcount = 0
nbins = 20
for lc in lcs:
    print ('{0}'.format(lc))
    AA = A[np.where(A[:,2]==lc),:]
    print ('{0}'.format(np.shape(AA)))
    D = AA[0,:,1]
    print ('{0}'.format(D))
    bins = np.linspace(1,0.5*np.max(D),nbins)
    h,b = np.histogram (D, bins)
    ax = F2.add_subplot (2,5,gcount+1)
    a1.append(ax)
    a1[gcount].plot (b[:-1], h, '.', color='r', marker='o')
    gcount = gcount + 1

plt.show()
