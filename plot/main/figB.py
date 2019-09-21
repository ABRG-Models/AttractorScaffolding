import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt
import sys
sys.path.insert(0,'../include') # To get states lib
import states_lib as sl

# Chosen network for figure is 8875517a-5c1e87e1-8eef99d4-1a3c467f-df7235c6 or:
net = '0101111010001010101011100001000110000111111000010111100000111010001010111001100111110111011100011111111001100010001111000101100001100011101011000100111011111011'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
hexstates = False
kequalsn = True
docolour = False
sl.plot_states (net, f1.axes, hexstates, kequalsn, docolour)
plt.tight_layout()
plt.savefig('figures/paper_figB.svg')
plt.savefig('figures/paper_figB.png')
print ('Wrote paper_figB')

plt.show()
