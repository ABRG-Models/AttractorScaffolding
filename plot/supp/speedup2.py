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
fs=16
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs}
matplotlib.rc('font', **fnt)

f1 = plt.figure(figsize=(10,8)) # Figure object

print ('Be sure to run evospeed_mean_vs_p.py with compute=True first')
M2 = np.load ('tmp/M2.npy')
M3 = np.load ('tmp/M3.npy')
M4 = np.load ('tmp/M4.npy')

c2randommean = M2[-1,2] # 6.21e4
print ('c2randommean: {0}'.format(c2randommean))

c3randommean = M3[-1,2] # 2.93e7 or 163 solutions in 5x10^9 generatiions => 3e7
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

print ('min gens to evolve: {0}, {1}, {2} (c2, c3, c4)'.format(np.min(M2[:,2]),np.min(M3[:,2]),np.min(M4[:,2])))
a1 = f1.add_subplot (1,1,1)
#a2 = a1.twinx()

ctxt = np.array([2,3,4])
speedup = np.array([0,0,0])
speedup[0] = c2randommean/np.min(M2[:,2])
speedup[1] = c3randommean/np.min(M3[:,2])
speedup[2] = c4randommean/np.min(M4[:,2])

pmin2 = M2[np.where(np.min(M2[:,2]) == M2[:,2]),1][0][0]
pmin3 = M3[np.where(np.min(M3[:,2]) == M3[:,2]),1][0][0]
pmin4 = M4[np.where(np.min(M4[:,2]) == M4[:,2]),1][0][0]

timetosoln = np.array([np.min(M2[:,2]), np.min(M3[:,2]), np.min(M4[:,2])])

lns1=a1.plot(ctxt, np.log10(timetosoln), marker='^', linestyle='-', color=col.black, markersize=12, label='$a = \mu$ (p=.05)')

rmean = np.array([c2randommean, c3randommean, c4randommean])
lns2=a1.plot(ctxt, np.log10(rmean), marker='s', linestyle='-', color=col.black, markersize=12, label='$b = \mu$ (p=.5)')


lns3=a1.plot(ctxt, np.log10(speedup), marker='o', linestyle='-', color=col.black, markersize=12, linewidth=4, label='$b/a$')

lns = lns1+lns2+lns3
labs = [l.get_label() for l in lns]
a1.legend(lns, labs, loc='upper left', frameon=False)

a1.set_xlabel('contexts ($X$)')
a1.set_ylabel('log(generations)')
#a2.set_ylabel('log$_{10}$(Random genomes)')
a1.set_xticks([2,3,4])

#a1.set_ylim([1,8])
#a2.set_ylim([4,11])

#[i.set_color(col.royalblue) for i in a1.get_yticklabels()]
#[i.set_color(col.crimson) for i in a2.get_yticklabels()]

f1.tight_layout()
plt.savefig ('figures/speedup2.png')

plt.show()
