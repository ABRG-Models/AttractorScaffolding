import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

# F>0
net = '00011110000010010001000111000110111000100001101101101000011101001010110100011101'

F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)
plt.tight_layout()
plt.savefig('png/states_bitflips_flip000.png', dpi=600)

for i in range(0,80):
    netflip = list(net)
    if netflip[i] == '0':
        netflip[i] = '1'
    else:
        netflip[i] = '0'
    netflip="".join(netflip)
    print ("Plotting genome ".format(netflip))

    F1 = plt.figure (figsize=(8,8))
    f1 = F1.add_subplot (111)
    sl.plot_states (netflip, f1.axes)
    plt.tight_layout()
    plt.savefig('png/states_bitflips_flip{0:02d}.png'.format(i), dpi=600)


#plt.show()
