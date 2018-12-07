import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import numpy as np
import sys

# Get dimensionality of the space to graph from cmd line.
if len(sys.argv) < 2:
    print('Provide n on cmd line please.')
    exit(1)
n = int(sys.argv[1])

F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)

greycol = (0.4,0.4,0.4)

for layer in range(0,n+1):
    print ('Layer {0}'.format(layer))
    o = np.loadtxt ('../data/node_layer_{0}_n{1}.csv'.format(layer,n), delimiter=',')
    #print ('{0} size {1}'.format(o, np.size(o)))
    x = np.array(range(0,np.size(o)))
    xo = np.vstack((x, o))
    nodes_zorder = 2*layer+1
    arrows_zorder = 2*layer-1.1
    print ('nodes zorder: {0}, arrows zorder: {1}'.format(nodes_zorder, arrows_zorder))
    f1.scatter (x, np.ones((1, np.size(o)))*layer, zorder=nodes_zorder)
    if layer > 0:
        t = np.loadtxt ('../data/node_trans_layer_{0}_n{1}.csv'.format(layer,n), delimiter=',')
        # For each row in t, plot an arrow
        #print ('Arrows for layer {0}'.format(layer))
        for ti in t:
            xto = xo[0,xo[1,:]==ti[1]] # Get the x position relating to the state ti[1]
            xfro = xolast[0,xolast[1,:]==ti[0]]
            #print ('Arrow from ({0},{1}) to ({2},{3})'.format(xfro[0], layer-1, xto[0]-xfro[0], 1))
            plt.arrow(xfro[0], layer-1, (xto[0]-xfro[0]), 1,
                      overhang=0, length_includes_head=True,
                      head_width=0.02, head_length=0.2, fc=greycol, ec=greycol, zorder=arrows_zorder)

    xolast = xo

#f1.set_xlim([0,10])
plt.show()
