import numpy as np
import matplotlib.pyplot as plt
from collections import Iterable

#FUNCTION TO TURN NESTED LIST INTO 1D LIST

def flatten(lis):
    for item in lis:
        if isinstance(item, Iterable) and not isinstance(item, str):
            for x in flatten(item):
                yield x
        else:
            yield item

#FUNCTION TO DRAW TREES

def tree(base,graph,cycle,bias):
    #find parents
    parents = graph[base][0]
    for each in cycle:
        if each in parents:
            parents.remove(each)


    #add parents to visits
    for a in parents:
        visits.append(a)

    #add co-ordinates to graph array
    l = len(parents)
    count = 0
    amp = graph[base][2][0]
    min_ang = graph[base][2][1]
    max_ang = graph[base][2][2]

    for b in parents:
        graph[b][2][0] = amp + 1
        graph[b][2][1] = min_ang + count*(max_ang-min_ang)/l
        graph[b][2][2] = min_ang + (count+1)*(max_ang-min_ang)/l
        count = count + 1


    #draw
    for c in parents:
            mid = (graph[c][2][1] + graph[c][2][2])/2
            xco = graph[c][2][0]*np.cos(np.radians(mid))
            yco = graph[c][2][0]*np.sin(np.radians(mid)) + bias
            graph[c][2][3] = xco
            graph[c][2][4] = yco


            plt.plot(xco,yco,'o',markersize=4, color=(1,1,1))
            plt.arrow(xco, yco, graph[base][2][3]-xco, graph[base][2][4]-yco,overhang=0.1 ,length_includes_head=True, head_width=0.2, head_length=0.2, fc='k', ec='k')


    for z in parents:
        tree(z,graph,parents,bias)

#net = '01101100000001011111100010100010000100001111111000111100000001000100101011111011'
net = '00111101110001011111111001001001001000011101111010111110101011110011101100110111'

#Find where each state leads
targets = []

for i in range(2**5):
    state = np.binary_repr(i,5)

    effect = net[int(state[1:],2)] + net[int(state[:1]+state[2:],2)+16] + net[int(state[:2]+state[3:],2) + 32] + net[int(state[:3]+state[4],2)+48] + net[int(state[:4],2)+64]

    #in decimal form
    targets.append(int(effect[4]) + 2*int(effect[3]) + 4*int(effect[2]) + 8*int(effect[1]) + 16*int(effect[0]))


#graph[n] gives the parent nodes, child nodes and co-ordinates for the nth node.
#graph[n][2][0] gives polar amplitude, [1] is min angle, [2] is max angle, [3] is x, [4] is y
graph = [[[],[],[0,0,0,0,0]] for x in range(1024)]

targets = [int(z) for z in targets]

for y in range(32):
    graph[y][1] = targets[y]            #add child
    graph[targets[y]][0].append(y)        #add parent

visits = []

fig = plt.figure()
ax = fig.gca()
fig.set_size_inches(8, 11)

plt.xticks([])
plt.yticks([])




bases = []

for x in range(len(targets)):
    visits = []
    while not x in visits:
        visits.append(x)
        x = targets[x]

    base = visits[visits.index(x):]
    if not base[0] in list(flatten(bases)):
        bases.append(base)

for base in bases:

    #find co-ordinates of base nodes

    tot = len(base)
    count = 0

    for x in base:
        graph[x][2][0] = 1
        graph[x][2][1] = count*180/tot
        graph[x][2][2] = (count+1)*180/tot
        count = count + 1

    #find max y-co for bias for next tree

    bias = graph[0][2][4]

    for node in graph:
        if node[2][4]>bias:
            bias = node[2][4]

    bias = bias + 5



    #draw
    plt.text(0,bias-2,base, ha='center')
    circle = plt.Circle((0,bias),1, color='k', fill=False)
    ax.add_artist(circle)
    for x in base:
        mid = (graph[x][2][1] + graph[x][2][2])/2.
        graph[x][2][3] = graph[x][2][0]*np.cos(np.radians(mid))
        graph[x][2][4] = graph[x][2][0]*np.sin(np.radians(mid)) + bias
        plt.plot(graph[x][2][3], graph[x][2][4],'o',color=(1,1,1))


    for x in base:
        tree(x,graph,base,bias)

    #do it again for the next set



#find max y and x to get axis right
max_x = graph[0][2][3]
max_y = graph[0][2][4]
min_x = max_x

for node in graph:
    if node[2][4]>max_y:
        max_y = node[2][4]
    if node[2][3]>max_x:
        max_x = node[2][3]

plt.plot(graph[21][2][3], graph[21][2][4],'^',color='r', markersize=15) #final ant
plt.plot(graph[10][2][3], graph[10][2][4],'^',color='b', markersize=15) #final post
plt.plot(graph[16][2][3], graph[16][2][4],'*',color='r', markersize=15) #initial ant
plt.plot(graph[0][2][3], graph[0][2][4],'*',color='b', markersize=15) #initial post

#save
ymin,ymax = plt.ylim()
plt.ylim(ymin-4,ymax+1)
xmin,xmax = plt.xlim()
plt.xlim(xmin-1,xmax+1)
plt.savefig('states.png', dpi=200)
print ('Wrote state.png')
