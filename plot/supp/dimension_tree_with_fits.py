from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import numpy as np
import sys

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)

# Get dimensionality of the space to graph from cmd line.
if len(sys.argv) < 2:
    print('Provide n on cmd line please.')
    exit(1)
n = int(sys.argv[1])

font = {'family' : 'normal',
        'weight' : 'normal',
        'size'   : 16}

matplotlib.rc('font', **font)

F1 = plt.figure (figsize=(18,18))
f1 = F1.add_subplot (111, projection='3d')

greycol = (0.4, 0.4, 0.4)

for layer in range(0,n+1):
    print ('Layer {0}'.format(layer))
    o = np.loadtxt ('../../data/node_layer_{0}_n{1}.csv'.format(layer,n), delimiter=',')
    #print ('{0} size {1}'.format(o, np.size(o)))
    x = np.array(range(0,np.size(o)))

    ## Generate a random fitness landscape
    z = np.random.rand(np.size(o)) < 0.01

    xo = np.vstack((x, o))
    nodes_zorder = 2*layer+1
    arrows_zorder = 2*layer-1.1
    print ('nodes zorder: {0}, arrows zorder: {1}'.format(nodes_zorder, arrows_zorder))
    y = np.ones(np.size(o))*layer
    f1.scatter (x, y, z, zorder=nodes_zorder)
    print ('x/y/z shapes: {0}/{1}/{2}'.format(np.shape(x), np.shape(y), np.shape(z)))
    ii = 0
    for xx in x:
        print ('x/y/z posns: {0}/{1}/{2}'.format(xx, y[ii], z[ii]))
        b = Arrow3D([xx, xx], [y[ii], y[ii]],
                    [0, z[ii]], mutation_scale=10,
                    lw=0.5, arrowstyle="-|>", color='r')
        f1.add_artist(b)
        ii = ii + 1

    if layer > 0:
        t = np.loadtxt ('../../data/node_trans_layer_{0}_n{1}.csv'.format(layer,n), delimiter=',')
        # For each row in t, plot an arrow
        #print ('Arrows for layer {0}'.format(layer))
        for ti in t:
            xto = xo[0,xo[1,:]==ti[1]] # Get the x position relating to the state ti[1]
            xfro = xolast[0,xolast[1,:]==ti[0]]
            #print ('Arrow from ({0},{1}) to ({2},{3})'.format(xfro[0], layer-1, xto[0]-xfro[0], 1))
            #plt.arrow(xfro[0], layer-1, (xto[0]-xfro[0]), 1,
            #          overhang=0, length_includes_head=True,
            #          head_width=0.02, head_length=0.2, fc=greycol, ec=greycol, zorder=arrows_zorder)
            a = Arrow3D([xfro[0], xto[0]], [layer-1, layer],
                        [0, 0], mutation_scale=10,
                        lw=0.2, arrowstyle="-|>", color=greycol)
            f1.add_artist(a)
    xolast = xo

#f1.set_xlim([0,10])
f1.set_xlabel ('Gene space position x')
f1.set_ylabel ('Gene space position y')
f1.set_zlabel ('Fitness')
fname = './figures/dimtree{0}.png'.format(n)
plt.savefig(fname);
plt.show()
