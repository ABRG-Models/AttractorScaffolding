import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sebcolour
col = sebcolour.Colour

nevolutions = 50000

# Read csv files
def readDataset (filepath):
    f = np.zeros([1,4])
    #genomes = []
    with open (filepath, 'r') as csvfile:
        rdr = csv.DictReader (csvfile)
        for row in rdr:
            # row[GenomeID] is genome, not used here.
            #genomes.append(row[GenomeID])
            f[-1,0] = float(row['Fitness']) # initial fitness
            f[-1,1] = float(row['NumberEvolvingFitter'])/nevolutions # num evolving fitter
            f[-1,2] = float(row['NumberEvolvingEqual'])/nevolutions # num evolving to equal fitness
            f[-1,3] = float(row['NumberEvolvingLessFit'])/nevolutions # num evolving same

            f = np.append(f, np.zeros([1,4]), 0)

    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]#, nets

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : 14}
matplotlib.rc('font', **fnt)

flipbits = [3,4,5,6,8,10,15,20]
probs_fitter = []
sds_fitter = []
probs_equal = []
sds_equal = []
probs_lessfit = []
sds_lessfit = []

# How many bins to use in histos; controls resolution of 2D histos
nbins = 60

# X, Y for overall graph
X_oa, Y_oa = np.meshgrid(flipbits, np.linspace(0, 1, num=nbins-1))
Z_fitter = np.zeros([nbins-1,len(flipbits)])
print ('X_oa shape: {0}, Z_fitter shape: {1}'.format(np.shape(X_oa), np.shape(Z_fitter)))
Z_equal = np.zeros([nbins-1,len(flipbits)])
Z_lessfit = np.zeros([nbins-1,len(flipbits)])

for ii in range(0,len(flipbits)):

    fb = flipbits[ii];

    F1 = plt.figure (figsize=(16,8))

    readpath = '../data/prob_fitinc_' + str(nevolutions) +'_evolutions_flipbits_' + str(fb) + '_ff4.csv'
    A = readDataset(readpath)

    prob_fitter = np.mean (A[:,1])
    prob_equal = np.mean (A[:,2])
    prob_lessfit = np.mean (A[:,3])
    sd_fitter = np.std (A[:,1])
    sd_equal = np.std (A[:,2])
    sd_lessfit = np.std (A[:,3])

    probs_fitter.append(prob_fitter)
    sds_fitter.append(sd_fitter)
    probs_equal.append(prob_equal)
    sds_equal.append(sd_equal)
    probs_lessfit.append(prob_lessfit)
    sds_lessfit.append(sd_lessfit)

    # Make up data for final 2D histo for Prob/num flips graph
    bins = np.linspace(0, 1, num=nbins)
    h_fitter, b_fitter = np.histogram (A[:, 1], bins)
    h_equal, b_equal = np.histogram (A[:, 2], bins)
    h_lessfit, b_lessfit = np.histogram (A[:, 3], bins)
    Z_fitter[:,ii] = h_fitter
    Z_equal[:,ii] = h_equal
    Z_lessfit[:,ii] = h_lessfit

    # For some graphs, make a histogram of the n_ev_fitter and n_ev_equal.

    f1 = F1.add_subplot(2,5,1)
    f1_low = F1.add_subplot(2,5,6)
    # A[:,0] is initial fitness
    # A[:,1] is prob. of being fitter.
    # create bins in initial fitness.

    # Compute initial fitness from/to; probability fitter from/to etc.
    if_from = np.log(np.min(A[:,0]))
    if_to = np.log(np.max(A[:,0]))
    pf_from = np.log(np.min(A[:,1]))  # prob. fitter
    pf_to = np.log(np.max(A[:,1]))
    pe_from = np.log(np.min(A[:,2]))  # equal
    pe_to = np.log(np.max(A[:,2]))
    plf_from = np.log(np.min(A[:,2])) # less fit
    plf_to = np.log(np.max(A[:,2]))

    if_bins = np.logspace(if_from, if_to, base=2.7183, num=nbins)

    X, Y = np.meshgrid(np.logspace(if_from, if_to, base=2.7183, num=nbins-1),
                       np.logspace(pf_from, pf_to, base=2.7183, num=nbins-1))
    Z = np.zeros([nbins-1,nbins-1])
    colcount = 0
    for bdry in if_bins:
        # If on first bound, need to move on one.
        if bdry == if_bins[0]:
            bdry_last = bdry
            continue
        # This is numpy style multiple selection
        sel = (A[:,0]>bdry_last) & (A[:,0]<=bdry)
        if_sel = A[sel, 1] # selected between two initial fitness boundaries
        bins = np.logspace(pf_from, 0, base=2.7183, num=nbins)
        h, b = np.histogram (if_sel, bins)
        Z[:,colcount] = np.log(h)
        Z[Z == -np.inf] = 0       # sets -Inf to 0 for nicer looking plot
        bdry_last = bdry              # Current upper bound becomes next lower bound
        colcount = colcount + 1   # Increment the column we're on
    f1.contourf(np.log(X),np.log(Y),Z, zorder=0)

    #print ('Z = {0}', Z)
    f1.set_xlabel('log(Initial Fitness)');
    f1.set_ylabel('log(P(fitter))');
    f1.set_xlim([-10,-2])
    f1.set_ylim([-8,0])

    f1_low.plot (np.log(A[:,0]),
                 np.log(A[:,1]),
                 marker='.', markersize=1, linestyle='None', linewidth=2, color=col.darkviolet,
                 zorder=1)
    f1_low.set_xlabel('log(Initial Fitness)');
    f1_low.set_ylabel('log(P(fitter))');
    f1_low.set_xlim([-10,-2])
    f1_low.set_ylim([-8,0])

    f2 = F1.add_subplot(2,5,2)
    f2_low = F1.add_subplot(2,5,7)

    X, Y = np.meshgrid(np.logspace(if_from, if_to, base=2.7183, num=nbins-1),
                       np.linspace(0, 1, num=nbins-1))
    Z = np.zeros([nbins-1,nbins-1])
    colcount = 0
    for bdry in if_bins:
        # If on first bound, need to move on one.
        if bdry == if_bins[0]:
            bdry_last = bdry
            continue
        # This is numpy style multiple selection
        sel = (A[:,0]>bdry_last) & (A[:,0]<=bdry)
        if_sel = A[sel, 1] # selected between two initial fitness boundaries
        bins = np.linspace(0, 1, num=nbins)
        h, b = np.histogram (if_sel, bins)
        Z[:,colcount] = np.log(h)
        Z[Z == -np.inf] = 0       # sets -Inf to 0 for nicer looking plot
        bdry_last = bdry              # Current upper bound becomes next lower bound
        colcount = colcount + 1   # Increment the column we're on
    f2.contourf(np.log(X),Y,Z, zorder=0)
    f2.set_xlabel('log(Initial Fitness)');
    f2.set_ylabel('P(fitter)');
    f2.set_xlim([-10,-2])
    f2.set_ylim([0,1])

    f2_low.plot (np.log(A[:,0]), A[:,1],
                 marker='.', markersize=1, linestyle='None', linewidth=2, color=col.darkviolet)
    f2_low.set_xlabel('log(Initial Fitness)');
    f2_low.set_ylabel('P(fitter)');
    f2_low.set_xlim([-10,-2])
    f2_low.set_ylim([0,1])

    #
    # P(equal) vs log (IF)
    #
    f4_low = F1.add_subplot(2,5,3)
    X, Y = np.meshgrid(np.logspace(if_from, if_to, base=2.7183, num=nbins-1),
                       np.linspace(0, 1, num=nbins-1))
    Z = np.zeros([nbins-1,nbins-1])
    colcount = 0
    for bdry in if_bins:
        # If on first bound, need to move on one.
        if bdry == if_bins[0]:
            bdry_last = bdry
            continue
        # This is numpy style multiple selection
        sel = (A[:,0]>bdry_last) & (A[:,0]<=bdry)
        if_sel = A[sel, 2] # selected between two initial fitness boundaries
        bins = np.linspace(0, 1, num=nbins)
        h, b = np.histogram (if_sel, bins)
        Z[:,colcount] = np.log(h)
        Z[Z == -np.inf] = 0       # sets -Inf to 0 for nicer looking plot
        bdry_last = bdry              # Current upper bound becomes next lower bound
        colcount = colcount + 1   # Increment the column we're on
    f4_low.contourf(np.log(X),Y,Z, zorder=0)
    f4_low.set_xlabel('log(Initial Fitness)');
    f4_low.set_ylabel('P(equal)');
    f4_low.set_xlim([-10,-2]);
    f4_low.set_ylim([0,1]);

    #
    # P(lessfit) vs log (IF)
    #
    f5_low = F1.add_subplot(2,5,4)
    X, Y = np.meshgrid(np.logspace(if_from, if_to, base=2.7183, num=nbins-1),
                       np.linspace(0, 1, num=nbins-1))
    Z = np.zeros([nbins-1,nbins-1])
    colcount = 0
    for bdry in if_bins:
        # If on first bound, need to move on one.
        if bdry == if_bins[0]:
            bdry_last = bdry
            continue
        # This is numpy style multiple selection
        sel = (A[:,0]>bdry_last) & (A[:,0]<=bdry)
        if_sel = A[sel, 3] # selected between two initial fitness boundaries
        bins = np.linspace(0, 1, num=nbins)
        h, b = np.histogram (if_sel, bins)
        Z[:,colcount] = np.log(h)
        Z[Z == -np.inf] = 0       # sets -Inf to 0 for nicer looking plot
        bdry_last = bdry              # Current upper bound becomes next lower bound
        colcount = colcount + 1   # Increment the column we're on
    f5_low.contourf(np.log(X),Y,Z, zorder=0)
    f5_low.set_xlabel('log(Initial Fitness)');
    f5_low.set_ylabel('P(less fit)');
    f5_low.set_xlim([-10,-2]);
    f5_low.set_ylim([0,1]);

    f3 = F1.add_subplot(2,5,9)
    f3_low = F1.add_subplot(2,5,8)
    X, Y = np.meshgrid(np.linspace(0, 1, num=nbins-1),
                       np.linspace(0, 1, num=nbins-1))
    Z = np.zeros([nbins-1,nbins-1])
    colcount = 0
    pf_linbins = np.linspace(0, 1, num=nbins)
    for bdry in pf_linbins:
        # If on first bound, need to move on one.
        if bdry == pf_linbins[0]:
            bdry_last = bdry
            continue
        # This is numpy style multiple selection
        sel = (A[:,1]>bdry_last) & (A[:,1]<=bdry)
        pf_sel = A[sel, 2]
        bins = np.linspace(0, 1, num=nbins)
        h, b = np.histogram (pf_sel, bins)
        #print ('h: {0}'.format(h))
        Z[:,colcount] = np.log(h)
        Z[Z == -np.inf] = 0       # sets -Inf to 0 for nicer looking plot
        bdry_last = bdry              # Current upper bound becomes next lower bound
        colcount = colcount + 1   # Increment the column we're on
    f3.contourf(X,Y,Z, zorder=0)
    f3.set_xlabel('P(fitter)');
    f3.set_ylabel('P(equal fitness)');
    f3.set_xlim([0,0.7]);
    f3.set_ylim([0,0.7]);

    f3_low.plot (A[:,1], A[:,2],
                 marker='o', markersize=1,
                 linestyle='None', linewidth=2, color=col.maroon1)
    f3_low.set_xlabel('P(fitter)');
    f3_low.set_ylabel('P(equal fitness)');
    f3_low.set_xlim([0,0.7]);
    f3_low.set_ylim([0,0.7]);

    # Replaces 0 in A[:,1], so that the log operation works.
    A[np.where(A[:,1]==0),1] = 0.1
    D = np.log(A[:,1])

    print ('D min/max: {0}/{1}'.format(np.min(D), np.max(D)))

    bins = np.linspace (np.min(D), np.max(D), nbins)
    h,b = np.histogram (D, bins)
    f4 = F1.add_subplot(2,5,5)
    f4.plot(b[:-1],h,'.',color=col.darkviolet, marker='o', markersize=8)
    f4.set_xlabel('log (P(fitter))');
    f4.set_ylabel('Freq');
    #f4.set_xlim([-8,0])
    #f4.set_ylim([0,300])

    #f1.set_xlim([-1, 0.01])
    F1.tight_layout()
    figpath = 'png/prob_fitinc_flipbits_' + '{0:02d}'.format(fb) + '.png'
    plt.savefig (figpath)
    #plt.show()
    plt.clf()

F2 = plt.figure(figsize=(10,8))

f2_1 = F2.add_subplot(2,2,1)
f2_2 = F2.add_subplot(2,2,2)
f2_3 = F2.add_subplot(2,2,3)
f2_4 = F2.add_subplot(2,2,4)

f2_1.errorbar (flipbits, probs_fitter, sds_fitter)
f2_1.errorbar (flipbits, probs_equal, sds_equal)
f2_1.errorbar (flipbits, probs_lessfit, sds_lessfit)
f2_1.set_xlabel('Num flips/Hamming distance')
f2_1.set_ylabel('Probability')
f2_1.legend (('Fitter','Equal','Less fit'));

Zout = Z_fitter
f2_2.contourf (X_oa, Y_oa, Zout, zorder=0)
f2_2.set_xlabel('Num flips/Hamming distance')
f2_2.set_ylabel('P(fitter)')

Zout = np.log(Z_equal)
Zout[Zout == -np.inf] = 0
f2_3.contourf (X_oa,Y_oa, Zout, zorder=0)
f2_3.set_xlabel('Num flips/Hamming distance')
f2_3.set_ylabel('P(equal) colour log scale')

Zout = Z_lessfit
f2_4.contourf(X_oa, Y_oa, Zout, zorder=0)
f2_4.set_xlabel('Num flips/Hamming distance')
f2_4.set_ylabel('P(less fit)')

plt.savefig ('png/prob_fitinc_flipbits_prob_vs_bits.png')
print ("See results in png/prob_fitinc_flipbits_*.png")
plt.clf()
