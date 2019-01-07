import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

# Chosen network for figure....
net = '0010011100010101100111000000101011010111000001010000100100000101000101010000101111110011011010101101110110110011001110010011100111010010100001001101000001100000'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
hexstates = False
kequalsn = True
docolour = False
sl.plot_states (net, f1.axes, hexstates, kequalsn, docolour)
plt.tight_layout()
plt.savefig('png/paper_figB.png', dpi=600)
print ('Wrote paper_figB.png')


plt.show()
