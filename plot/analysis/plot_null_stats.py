import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sys
sys.path.insert(0, '../include')
import sebcolour
col = sebcolour.Colour

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : 14}
matplotlib.rc('font', **fnt)

F1 = plt.figure (figsize=(8,8))

FF_NAME = 'ff4'

# NB: Run plot_null_stats_load.py first to generate nullstats_A.npy
A = np.load ('nullstats_A' + FF_NAME + '.npy')
# Could replace above with:
#A2 = np.loadtxt ('../data/null_withf_a21_p10_'+FF_NAME+'_100000_fitness_0.csv', delimiter=',')
#print ('shapes: {0}, {1}'.format(np.shape(A), np.shape(A2)))

f1 = F1.add_subplot(2,2,1)
f1.plot (A[:,0]/10000,A[:,1],marker='o',linestyle='None',linewidth=2,color=col.darkviolet)
f1.set_xlabel('10K generations');
f1.set_ylabel('fitness');

# Num basin vs fitness
f2 = F1.add_subplot(2,2,2)
nbins = 34
X, Y = np.meshgrid(np.linspace(0,32,33), np.linspace(0,1.0,100))
Z = np.zeros([33,100])
fitness = A[:,1]
numcyc = A[:,2]
for mlc in range (0,33): # mean limit cycle (size)
    print ('mlc={0}'.format(mlc))
    print ('fitness: {0}'.format(fitness))
    # a if condition else b
    fitsel = fitness[(numcyc >= mlc) & (numcyc < mlc+1)]
    print ('fitsel.shape(): {0} .size(): {1} Number not 0.5: {2}'
           .format(np.shape(fitsel), np.size(fitsel), np.size(fitsel[fitsel!=0.5])))
    print ('FITSEL: {0}'.format(fitsel))
    bins = np.linspace(0, 1.0, 101)
    h, b = np.histogram (fitsel, bins)
    print ('h shape: {0}'.format(np.shape(h)))
    Z[int(mlc),:] = np.log(h)
#f2.plot (A[:,2],A[:,1],marker='o',linestyle='None',linewidth=2,color=col.maroon1)
Z[Z==-np.inf]=0
f2.contourf(X,Y,Z.T)
f2.set_ylabel('fitness');
f2.set_xlabel('Num limit cycles in system');
f2.set_ylim([0,0.2])

# Mean limit cycle length
f3 = F1.add_subplot(2,2,3)

nbins = 34
X, Y = np.meshgrid(np.linspace(0,32,33), np.linspace(0,1.0,100))
Z = np.zeros([33,100])
fitness = A[:,1]
meanlim = A[:,3]
for mlc in range (0,33): # mean limit cycle (size)
    print ('mlc={0}'.format(mlc))
    print ('fitness: {0}'.format(fitness))
    # a if condition else b
    fitsel = fitness[(meanlim >= mlc) & (meanlim < mlc+1)]
    print ('fitsel.shape(): {0} .size(): {1} Number not 0.5: {2}'
           .format(np.shape(fitsel), np.size(fitsel), np.size(fitsel[fitsel!=0.5])))
    print ('FITSEL: {0}'.format(fitsel))
    bins = np.linspace(0, 1.0, 101)
    h, b = np.histogram (fitsel, bins)
    print ('h shape: {0}'.format(np.shape(h)))
    Z[int(mlc),:] = np.log(h)
#f3.plot (A[:,3],A[:,1],marker='.',markersize=0.5,linestyle='None',linewidth=2,color=col.violetred)
Z[Z==-np.inf]=0
f3.contourf(X,Y,Z.T)
f3.set_ylabel('fitness');
f3.set_xlabel('Mean limit cycle size');
f3.set_ylim([0,0.2])

# Max limit cycle length
f4 = F1.add_subplot(2,2,4)

nbins = 34
X, Y = np.meshgrid(np.linspace(0,32,33), np.linspace(0,1.0,100))
Z = np.zeros([33,100])
#fitness = A[:,1]
maxlim = A[:,4]
for mlc in range (0,33): # mean limit cycle (size)
    print ('mlc={0}'.format(mlc))
    print ('fitness: {0}'.format(fitness))
    # a if condition else b
    fitsel = fitness[(maxlim >= mlc) & (maxlim < mlc+1)]
    print ('fitsel.shape(): {0} .size(): {1} Number not 0.5: {2}'
           .format(np.shape(fitsel), np.size(fitsel), np.size(fitsel[fitsel!=0.5])))
    print ('FITSEL: {0}'.format(fitsel))
    bins = np.linspace(0, 1.0, 101)
    h, b = np.histogram (fitsel, bins)
    print ('h shape: {0}'.format(np.shape(h)))
    Z[int(mlc),:] = np.log(h)
#f4.plot (A[:,4],A[:,1],marker='o',linestyle='None',linewidth=2,color=col.crimson)
Z[Z==-np.inf]=0
f4.contourf(X,Y,Z.T)
f4.set_ylabel('fitness');
f4.set_xlabel('Max limit cycle size');
f4.set_ylim([0,0.2])

#f1.set_xlim([-1, 0.01])
F1.tight_layout()
plt.savefig ('figures/null_model_stats_' + FF_NAME + '.png')

F2 = plt.figure (figsize=(12,8))
# For each unique value in A[:,2] (number of limit cycles), do a
# histogram of the fitnesses in A[:,1]
print ('A col 2 has length {0}'.format(len(A[:,2])))
lcs = np.unique(A[:,2])

print ('{0} unique values in A[:,2]: {1}'.format(len(lcs), lcs))
a1 = []
gcount = 0
nbins = 20
for lc in lcs:
    print ('{0}'.format(lc))
    print ('A shape {0}'.format(np.shape(A)))
    AA = A[np.where(A[:,2]==lc)]
    print ('AA shape {0}'.format(np.shape(AA)))
    D = AA[:,1]
    print ('D shape {0} max {1} min {2}'.format(np.shape(D), max(D), min(D)))
    unq, cnts = np.unique(D, return_counts=True)
    print ('unq: {0}'.format(unq))
    print ('{0}'.format(D))
    bins = np.linspace (min(D), max(D), num=nbins)
    h,b = np.histogram (D, bins)
    ax = F2.add_subplot (2,5,gcount+1)
    a1.append(ax)
    print (b[:-1])
    print (h)
    a1[gcount].plot (b[:-1], h, color='r')
    #a1[gcount].plot (np.log(unq), np.log(cnts), '.', color='r', marker='o')
    a1[gcount].set_title('LC size {0}'.format(lc))
    gcount = gcount + 1

plt.savefig ('figures/null_model_stats_perLC_' + FF_NAME + '.png')
plt.show()
