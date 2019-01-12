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

FF_NAME = 'ff4'
A = readDataset('../data/null_withf_a21_p10_'+FF_NAME+'_100000_fitness_0.csv')
np.save ('nullstats_A'+FF_NAME, A)
