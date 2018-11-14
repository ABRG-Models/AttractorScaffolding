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
    f = np.zeros([1,3])
    #genomes = []
    with open (filepath, 'r') as csvfile:
        rdr = csv.DictReader (csvfile)
        for row in rdr:
            # row[GenomeID] is genome, not used here.
            #genomes.append(row[GenomeID])
            f[-1,0] = float(row['Fitness']) # initial fitness
            f[-1,1] = float(row['NumberEvolvingFitter'])/nevolutions # num evolving fitter
            f[-1,2] = float(row['NumberEvolvingEqual'])/nevolutions # num evolving to equal fitness

            f = np.append(f, np.zeros([1,3]), 0)

    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]#, nets

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : 14}
matplotlib.rc('font', **fnt)

for ii in range(1,6):

    pOn = ii/10.0;

    F1 = plt.figure (figsize=(8,8))

    readpath = '../data/prob_evolvefitter_' + str(nevolutions) +'_evolutions_pOn_' + str(pOn) + '.csv'
    A = readDataset(readpath)

    # Sort A on col 0.
    B = A[A[:,0].argsort()]

    # For some graphs, make a histogram of the n_ev_fitter and n_ev_equal.

    f1 = F1.add_subplot(2,2,1)
    f1.plot (np.log(A[:,0]), np.log(A[:,1]), marker='o', linestyle='None', linewidth=2, color=col.darkviolet)
    f1.set_xlabel('log(Initial Fitness)');
    f1.set_ylabel('log(P(fitter))');
    f1.set_xlim([-10,-1])
    f1.set_ylim([-8,0])

    f2 = F1.add_subplot(2,2,2)
    f2.plot (np.log(A[:,0]), A[:,1], marker='o', linestyle='None', linewidth=2, color=col.darkviolet)
    f2.set_xlabel('log(Initial Fitness)');
    f2.set_ylabel('P(fitter)');
    f2.set_xlim([-10,-1])
    f2.set_ylim([0,0.6])

    f3 = F1.add_subplot(2,2,3)
    f3.plot (A[:,1], A[:,2], marker='o', linestyle='None', linewidth=2, color=col.maroon1)
    f3.set_xlabel('P(fitter)');
    f3.set_ylabel('P(equal fitness)');
    f3.set_xlim([0,0.6])
    f3.set_ylim([0,0.3])

    nbins=20

    # Replaces 0 in A[:,1], so that the log operation works.
    A[np.where(A[:,1]==0),1] = 0.1
    D = np.log(A[:,1])

    print ('D min/max: {0}/{1}'.format(np.min(D), np.max(D)))

    bins = np.linspace (np.min(D), np.max(D), nbins)
    h,b = np.histogram (D, bins)
    f4 = F1.add_subplot(2,2,4)
    f4.plot(b[:-1],h,'.',color=col.darkviolet, marker='o', markersize=8)
    f4.set_xlabel('log (P(fitter))');
    f4.set_ylabel('Freq');
    f4.set_xlim([-8,0])
    f4.set_ylim([0,300])

    #f1.set_xlim([-1, 0.01])
    F1.tight_layout()
    figpath = 'prob_fitinc_pOn_' + str(pOn) + '.png'
    plt.savefig (figpath)
    plt.show()
    plt.clf()
