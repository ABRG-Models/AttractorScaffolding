#
# Plot mutant fitness vs. Hamming distance from a perfectly fit genome.
#

import numpy as np
import matplotlib

# This should avoid use of WebAgg:
gui_env = ['gtkagg','qt5','wxagg','TKAgg']
for gui in gui_env:
    try:
        print ("testing {0}".format(gui))
        matplotlib.use(gui,warn=False, force=True)
        from matplotlib import pyplot as plt
        break
    except:
        continue
print ("Using: {0}".format(matplotlib.get_backend()))

import sys
import csv

def readDataset (genes, m):
    rn = 0 # rownum - accumulates through all 10 files that are read
    for i in range(1,11):
        filepath = '../data/mutations_ff0_n{:1d}_{:02d}.csv'.format(genes,i)
        with open (filepath, 'r') as csvfile:
            rdr = csv.DictReader (csvfile)
            for row in rdr:
                # Read into m. HammingDist. All states measured for this datum had this Hamming distance from the fit genome.
                m[rn, 0] = float(row['HammingDist'])
                # The number of states measured that had fitness > 0
                m[rn, 1] = float(row['NumFit'])
                # Number of states measured: either all possible Hamming states (if ExhaustiveSearch==1) or a sample thereof.
                m[rn, 2] = float(row['SampledStates'])
                # Number of fit states / number of states measured, i.e. NumFit/SampledStates
                m[rn, 3] = float(row['ProportionFit'])
                # 1 if an exhaustive search was performed, 0 otherwise.
                m[rn, 4] = float(row['ExhaustiveSearch'])
                # The sum of fitness for all fit states measured.
                m[rn, 5] = float(row['TotalFit'])
                # TotalFit / SampledStates
                m[rn, 6] = float(row['FitPerState'])
                # Last is deprecated.
                if float(row['NumFit']) > 0:
                    m[rn, 7] = float(row['TotalFit'])/float(row['NumFit'])
                else :
                    m[rn, 7] = 0
                # Add new row to m, and inc rn
                m = np.append(m, np.zeros([1,8]), 0)
                rn = rn + 1
    print ('Read {0} rows from the genes={1} csv files'.format(rn, genes))
    # Note the -1 as there will be a final, zero line in the array from the last np.append(m, np.zeros...
    return m[:-1,:]

# Read all the data into m.
m4 = np.zeros([1,8])
genes = 4
m4 = readDataset (genes, m4)
m5 = np.zeros([1,8])
genes = 5
m5 = readDataset (genes, m5)
#m6 = np.zeros([1,8])
#genes = 6
#m6 = readDataset (genes, m6)

def bootstrap_sample (n):
    # Sample, with replacement, from range 0, nRows.
    indices = np.floor((n * np.random.ranf([1,n])));
    return indices.astype(int)

# Now go through m4,m5,m6, selecting results for each Hamming distance
# and compute mean and bootstrapped std error.
def comp_bootstrap (genes, m):
    # For each Hamming distance:
    m_final = np.zeros([1, 10])
    # Add Hamming 0 manually
    m_final[0,0] = 0
    m_final[0,1] = 1
    m_final[0,2] = 0
    m_final[0,3] = 0
    m_final[0,4] = 1
    m_final[0,5] = 0
    m_final[0,6] = 0
    m_final[0,7] = 1
    m_final[0,8] = 0
    m_final[0,9] = 0
    m_final = np.append (m_final, np.zeros([1, 10]), 0)

    for h in range (1, 1+genes*pow(2,genes-1)):
        # Get all from m for which col 0 is equal to h.
        m_h = m[m[:,0]==h] # not right
        #themean = np.mean(m_h[:,3])
        #print ('Hamming {0} has mean {1}'.format (h, themean))
        # Compute bootstrapped estimate of the standard error of the mean
        sz = np.shape(m_h)
        #print ('m_h shape {0}'.format(sz))
        nRows = sz[1]
        nSamp = 1000
        bmeans_3 = np.zeros ([nSamp,1]) # means of num with >0 fit
        bmeans_6 = np.zeros ([nSamp,1]) # means of total fitness/numstates
        bmeans_7 = np.zeros ([nSamp,1]) # means of total fitness/numfit
        #print ('bmeans shape: {0}'.format (np.shape(bmeans_3)))
        for s in range (0, nSamp):
            bs = bootstrap_sample (nRows)
            bmeans_3[s] = np.mean (m_h[bs,3])
            #print ('For Hamming {0}, the mean is {1}'.format(h, bmeans_3[s]))
            bmeans_6[s] = np.mean (m_h[bs,6])
            bmeans_7[s] = np.mean (m_h[bs,7])

        std_err_3 = np.sqrt(np.var(bmeans_3))
        #print ('For Hamming {0}, the std err for col 3 is {1} var is {2}'.format(h, std_err_3, np.var(bmeans_3)))
        std_err_6 = np.sqrt(np.var(bmeans_6))
        std_err_7 = np.sqrt(np.var(bmeans_7)) # Sum of fitness divided by number of states sampled (or divided by all states at Hamming=h for exhaustive)

        m_final[-1,0] = h
        m_final[-1,1] = np.mean(m_h[:,3])
        m_final[-1,2] = std_err_3
        m_final[-1,3] = np.std(m_h[:,3])
        m_final[-1,4] = np.mean(m_h[:,6])
        m_final[-1,5] = std_err_6
        m_final[-1,6] = np.std(m_h[:,6])
        m_final[-1,7] = np.mean(m_h[:,7])
        m_final[-1,8] = std_err_7
        m_final[-1,9] = np.std(m_h[:,7])
        #print ('Size m_final: {0}'.format(np.shape(m_final)))
        m_final = np.append (m_final, np.zeros([1, 10]), 0)
        #print ('After append, size m_final: {0}'.format(np.shape(m_final)))

    return m_final[:-1,:]

# Finally plot graphs.

#b = bootstrap_sample (10)
#print ('b is {0}'.format(b))
m4_final = comp_bootstrap (4, m4)
m5_final = comp_bootstrap (5, m5)
#m6_final = comp_bootstrap (6, m6)

import sebcolour
c = sebcolour.Colour
fs = 28
fs2 = 18

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

# What's the poosible number of states for Hamming = h away from a fit state in a genome for n genes? It's:
# (genes * (2^ins))^h
#hamstates = pow (genes * pow(2,genes-1), ham)

f0 = plt.figure(figsize=(8,8))
plt.errorbar (m4_final[:,0], m4_final[:,1], yerr=m4_final[:,2], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0], m5_final[:,1], yerr=m5_final[:,2], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0], m6_final[:,1], yerr=m6_final[:,2], fmt='.', linestyle='-', color=c.magenta)
f0.axes[0].set_xlabel('Hamming distance from F=1 genome',fontsize=fs2)
f0.axes[0].set_ylabel('Proportion of possible states that are fit',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness0.png')

f1 = plt.figure(figsize=(8,8))
#f1, axs = pl1.subplots(nrows=2, ncols=2, sharex=True)
#ax1 = axs[0]
plt.errorbar (m4_final[:,0]/32, m4_final[:,1], yerr=m4_final[:,2], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0]/80, m5_final[:,1], yerr=m5_final[:,2], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0]/192, m6_final[:,1], yerr=m6_final[:,2], fmt='.', linestyle='-', color=c.magenta)
#f1.axes[0].set_title('Proportion of states that are fit',fontsize=fs)
f1.axes[0].set_xlabel('Proportional Hamming distance from F=1 genome',fontsize=fs2)
f1.axes[0].set_ylabel('Proportion of possible states that are fit',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness1.png')

f2 = plt.figure(figsize=(8,8))
plt.errorbar (m4_final[:,0]/32, m4_final[:,4], yerr=m4_final[:,5], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0]/80, m5_final[:,4], yerr=m5_final[:,5], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0]/192, m6_final[:,4], yerr=m6_final[:,5], fmt='.', linestyle='-', color=c.magenta)
f2.axes[0].set_xlabel('Proportional Hamming distance from F=1 genome',fontsize=fs2)
f2.axes[0].set_ylabel('Mean fitness per possible state',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness2.png')

f3 = plt.figure(figsize=(8,8))
plt.errorbar (m4_final[:,0]/32, m4_final[:,7], yerr=m4_final[:,8], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0]/80, m5_final[:,7], yerr=m5_final[:,8], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0]/192, m6_final[:,7], yerr=m6_final[:,8], fmt='.', linestyle='-', color=c.magenta)
# Try fitting one of these.
px = m5_final[0:-5,0]/80
py = m5_final[0:-5:,7]
fit_fn1 = np.poly1d (np.polyfit (px, py, 4))
fit_fn2 = np.poly1d (np.polyfit (px, py, 5))
fit_fn3 = np.poly1d (np.polyfit (px, py, 6))
plt.plot (px, fit_fn1(px), linestyle='-.', color=c.palevioletred1)
plt.plot (px, fit_fn2(px), linestyle='-.', color=c.palevioletred2)
plt.plot (px, fit_fn3(px), linestyle='-.', color=c.palevioletred3)
f3.axes[0].set_xlabel('Proportional Hamming distance from F=1 genome',fontsize=fs2)
f3.axes[0].set_ylabel('Sum of fitness/number of fit states',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness3.png')

f4 = plt.figure(figsize=(8,8))
plt.errorbar (m4_final[:,0], m4_final[:,7], yerr=m4_final[:,8], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0], m5_final[:,7], yerr=m5_final[:,8], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0], m6_final[:,7], yerr=m6_final[:,8], fmt='.', linestyle='-', color=c.magenta)
f4.axes[0].set_xlabel('Hamming distance from F=1 genome',fontsize=fs2)
f4.axes[0].set_ylabel('Sum of fitness/number of fit states',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness4.png')

f5 = plt.figure(figsize=(8,8))
plt.errorbar (m4_final[:,0]/32, np.log(m4_final[:,7]), yerr=m4_final[:,8], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0]/80, np.log(m5_final[:,7]), yerr=m5_final[:,8], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0]/192, np.log(m6_final[:,7]), yerr=m6_final[:,8], fmt='.', linestyle='-', color=c.magenta)
f5.axes[0].set_xlabel('Proportional Hamming distance from F=1 genome',fontsize=fs2)
f5.axes[0].set_ylabel('log(Sum of fitness/number of fit states)',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness5.png')

f6 = plt.figure(figsize=(8,8))
plt.errorbar (m4_final[:,0], np.log(m4_final[:,7]), yerr=m4_final[:,8], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0], np.log(m5_final[:,7]), yerr=m5_final[:,8], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0], np.log(m6_final[:,7]), yerr=m6_final[:,8], fmt='.', linestyle='-', color=c.magenta)
px = m5_final[0:-5,0]
py = np.log(m5_final[0:-5:,7])
fit_fn1 = np.poly1d (np.polyfit (px, py, 1))
fit_fn2 = np.poly1d (np.polyfit (px, py, 2))
fit_fn3 = np.poly1d (np.polyfit (px, py, 3))
plt.plot (px, fit_fn1(px), linestyle='-.', color=c.palevioletred1)
plt.plot (px, fit_fn2(px), linestyle='-.', color=c.palevioletred2)
plt.plot (px, fit_fn3(px), linestyle='-.', color=c.palevioletred3)
f6.axes[0].set_xlabel('Hamming distance from F=1 genome',fontsize=fs2)
f6.axes[0].set_ylabel('log(Sum of fitness/number of fit states)',fontsize=fs2)
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness6.png')

f7 = plt.figure(figsize=(8,8))
#f1, axs = pl1.subplots(nrows=2, ncols=2, sharex=True)
#ax1 = axs[0]
plt.errorbar (m4_final[:,0]/32, m4_final[:,1], yerr=m4_final[:,2], fmt='.', linestyle='-', color=c.slateblue2)
plt.errorbar (m5_final[:,0]/80, m5_final[:,1], yerr=m5_final[:,2], fmt='.', linestyle='-', color=c.darkviolet)
#plt.errorbar (m6_final[:,0]/192, m6_final[:,1], yerr=m6_final[:,2], fmt='.', linestyle='-', color=c.magenta)
#f1.axes[0].set_title('Proportion of states that are fit',fontsize=fs)
f7.axes[0].set_xlabel('Proportional Hamming distance from F=1 genome',fontsize=fs2)
f7.axes[0].set_ylabel('Proportion of possible states that are fit',fontsize=fs2)
f7.axes[0].set_xlim([0,0.5])
plt.legend(('4 genes','5 genes','6 genes'))
plt.savefig('png/mutantfitness7.png')

plt.show()
