import numpy as np
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import csv
import sebcolour
col = sebcolour.Colour
cols1 = np.array([col.darkviolet,
                  col.darkorchid1,
                  col.mediumorchid1,
                  col.orchid,
                  col.maroon1,
                  col.violetred1,
                  col.violetred,
                  col.crimson,
                  col.orange,
                  col.goldenrod1,
                  col.darkviolet,
                  col.darkorchid1,
                  col.mediumorchid1,
                  col.orchid,
                  col.maroon1,
                  col.violetred1,
                  col.violetred,
                  col.crimson,
                  col.orange,
                  col.goldenrod1])

# Read csv files with gen/fitness data in
def readDataset (filepath):
    f = np.zeros([1,2])
    with open (filepath, 'r') as csvfile:
            rdr = csv.reader (csvfile)
            for row in rdr:
                f[-1,0] = float(row[0])
                f[-1,1] = float(row[1])
                f = np.append(f, np.zeros([1,2]), 0)
    # Note the -1 as there will be a final, zero line in the array
    return f[:-1,:]

files = [
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_120-55cf-c9af-ff7e-c4da.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_137c-ca0f-3612-319d-a648.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_1ba3-4edd-ab12-f031-6545.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_216a-44e5-b739-85bb-5e41.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_2f-fe-bf8f-f0b1-4d8f.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_4bb2-88b5-429c-91d9-d50c.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_8362-8fb5-deab-30da-1514.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_a0ae-5637-2bbc-d511-eddc.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_a3bc-927f-7b84-f57d-ecdc.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_a3fe-1c4d-ea8e-d0f3-4f8e.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_a936-1547-ae36-561f-9f5e.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_a9eb-c60d-2e9b-6135-3ecf.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_d2fb-4e5e-3382-3235-de18.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_e3f8-17d5-9f24-8693-f00.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_eba4-5cc7-4f92-f65d-f4e.csv',
'../data/evolve_withf_a21_p10_ff4_100000_fitness_0.1_genome_f26a-4935-f312-76ff-ee91.csv'
]

# Font size for plotting
fs=20
fs2=18 # for axes tick labels

# Set a default fontsize for matplotlib:
fnt = {'family' : 'DejaVu Sans',
       'weight' : 'regular',
       'size'   : fs2}
matplotlib.rc('font', **fnt)

F1 = plt.figure (figsize=(12,12))
f1 = F1.add_subplot(1,1,1)

fcount=0
for f in files:
    if fcount > 9:
        break
    A = readDataset(f)
    if A[-5000,1] < 0.2:
        f1.plot (A[:,0],A[:,1],'-',linewidth=3, color=cols1[fcount])
        fcount = fcount + 1

f1.set_xlim([-5000, 0])
f1.set_xlabel('Generation (F=1 achieved at generation 0)');
f1.set_ylabel('Fitness');
plt.savefig ('fitness_evolution_ff4.png')
plt.show()
