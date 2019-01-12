import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

# F>0
net = '0101111010001010101011100001000110000111111000010111100000111010001010111001100111110111011100011111111001100010001111000101100001100011101011000100111011111011'

F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)
plt.tight_layout()
plt.savefig('png/states_bitflips_flip000.png', dpi=600)

for i in range(0,1):
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

print ('See results in png/states_bitflips*.png')
#plt.show()
