import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sebcolour
col = sebcolour.Colour

A = np.load ('nullstats_A.npy')

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
    #a1[gcount].plot (b[:-1], h, '.', color='r', marker='o')
    a1[gcount].plot (np.log(unq), np.log(cnts), '.', color='r', marker='o')
    a1[gcount].set_title('LC size {0}'.format(lc))
    gcount = gcount + 1

plt.show()
