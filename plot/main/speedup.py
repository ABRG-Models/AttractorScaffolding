import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
sys.path.insert (0, '../include')
import csv
import sebcolour
col = sebcolour.Colour

# Font size for plotting
fs=20
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs}
matplotlib.rc('font', **fnt)

f1 = plt.figure(figsize=(8,8)) # Figure object

print ('Be sure to run evospeed_mean_vs_p.py with compute=True first')
M2 = np.load ('tmp/M2.npy')
M3 = np.load ('tmp/M3.npy')
M4 = np.load ('tmp/M4.npy')

c2randommean = M2[-1,1] # 6.21e4
print ('c2randommean: {0}'.format(c2randommean))

c3randommean = M3[-1,1] # 2.93e7 or 163 solutions in 5x10^9 generatiions => 3e7
print ('c3randommean: {0}'.format(c3randommean))

c4randommean = 1.667e10 # estimate, single generation found in
# 5000000000 gens Better estimate found from 2x10^11 generations (i.e
# 2e11 random genomes tested); for this number of attempts, the
# program found 12 solutions so 1.6x10^10 is the best estimate.
#
# See file
# data/evolve_nc4_I16-4-1-8_T20-10-5-9_ff4_200000000000_gens_0.5.csv
# HOWEVER, you can't use the mean of that set of numbers, because the
# number of generations between solutions means that the generations
# at the end of each file aren't counted, and neither are the
# generations of those files that were completely empty (remembering
# that I computed that solution in 1.5e9 generation chunks. The
# reliable number is the total number of solutions / the total number
# of random genomes tried, which is the 1.667e10 result.
print ('c4randommean: {0}'.format(c4randommean))

print ('min gens to evolve: {0}, {1}, {2} (c2, c3, c4)'.format(np.min(M2[:,1]),np.min(M3[:,1]),np.min(M4[:,1])))
a1 = f1.add_subplot (1,1,1)
#a2 = a1.twinx()

# Time to solution abs. fastest:
use_absfastest=0
if use_absfastest:
    timetosoln = np.array([np.min(M2[:,1]), np.min(M3[:,1]), np.min(M4[:,1])])
    # M matrix is col 0: p 1: mean 2: stderr
    pmin2 = M2[np.where(np.min(M2[:,1]) == M2[:,1]),0][0][0]
    pmin3 = M3[np.where(np.min(M3[:,1]) == M3[:,1]),0][0][0]
    pmin4 = M4[np.where(np.min(M4[:,1]) == M4[:,1]),0][0][0]
else:
    # Time to solution for p=0.05:
    print ('{0}'.format(np.where(M2[:,0]==0.05)[0][0] ) )
    print ('{0}'.format(np.where(M3[:,0]==0.05)[0][0] ) )
    print ('{0}'.format(np.where(M4[:,0]==0.05)[0][0] ) )
    m2i = np.where(M2[:,0]==0.05)[0][0]
    print ('{0}'.format(M2[m2i, 1]))
    timetosoln = np.array([M2[np.where(M2[:,0]==0.05)[0][0] ,1],
                           M3[np.where(M3[:,0]==0.05)[0][0] ,1],
                           M4[np.where(M4[:,0]==0.05)[0][0] ,1]])
    pmin2 = 0.05
    pmin3 = 0.05
    pmin4 = 0.05

print ('pmin for 2, 3, 4 contexts: {0}, {1}, {2}'.format (pmin2, pmin3, pmin4))
print ('time to solutions are {0}'.format(timetosoln))

ctxt = np.array([2,3,4])
speedup = np.array([0,0,0])
speedup[0] = c2randommean/timetosoln[0]
speedup[1] = c3randommean/timetosoln[1]
speedup[2] = c4randommean/timetosoln[2]

if use_absfastest:
    lns1=a1.plot(ctxt, np.log(timetosoln), marker='^', linestyle='-', linewidth=2, color=col.black, markersize=12, label='$a = \mu$ (p=.04 or .03)')
else:
    lns1=a1.plot(ctxt, np.log(timetosoln), marker='^', linestyle='-', linewidth=2, color=col.black, markersize=12, label='$a = \mu$ (p=.05)')

rmean = np.array([c2randommean, c3randommean, c4randommean])
lns2=a1.plot(ctxt, np.log(rmean), marker='s', linestyle='-', linewidth=2, color=col.black, markersize=12, label='$b = \mu$ (p=.5)')


lns3=a1.plot(ctxt, np.log(speedup), marker='o', linestyle='-', linewidth=4, color=col.black, markersize=12, label='$b/a$')

lns = lns1+lns2+lns3
labs = [l.get_label() for l in lns]
a1.legend(lns, labs, loc='upper left', frameon=False)

a1.set_xlabel('contexts ($X$)')
a1.set_ylabel('log(generations)')
#a2.set_ylabel('log$_{10}$(Random genomes)')
a1.set_xticks([2,3,4])
a1.set_yticks([5, 10, 15, 20])
#a1.set_ylim([1,8])
#a2.set_ylim([4,11])

#[i.set_color(col.royalblue) for i in a1.get_yticklabels()]
#[i.set_color(col.crimson) for i in a2.get_yticklabels()]

f1.tight_layout()
plt.savefig ('figures/speedup.png')

plt.show()
