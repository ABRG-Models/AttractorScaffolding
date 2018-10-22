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
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_2026-de4d-f709-64d2-470e.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_22aa-58dd-1b2e-1631-f756.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_2baa-c46d-9f1d-a07d-af52.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_32f0-1677-570c-7351-751a.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_3a35-d047-f609-f2f1-3756.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_48ad-845f-831c-54d1-4d42.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_497f-d3fe-9e1d-93d9-f55f.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_7033-428f-5b0e-7278-f7cb.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_72f6-4917-7f06-915f-f88.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_8877-d86f-e8c-50d0-971c.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_8ae4-49df-5a8e-70f3-d5cc.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_93b8-105-172e-6179-b4c6.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_a33f-9ed6-2e89-1477-6743.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_ab35-40a5-3b06-d075-49c.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_c0fe-307-fb8-75db-9f0a.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_c13e-484d-8708-70f5-96cb.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_c22f-1817-a72e-1054-4719.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_d20b-18a4-aab3-16b8-2100.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_d221-8ddf-7304-b4fd-7685.csv',
    '../data/evolve_withf_a21_p10_ff0_100000_fitness_0.2_genome_e872-4d4f-8390-6271-bc14.csv'
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
    f1.plot (A[:,0],A[:,1],'-',linewidth=1, color=cols1[fcount])
    fcount = fcount + 1

f1.set_xlim([-5000, 0])
f1.set_xlabel('Generation (F=1 achieved at generation 0)');
f1.set_ylabel('Fitness');
plt.savefig ('fitness_evolution.png')
plt.show()
