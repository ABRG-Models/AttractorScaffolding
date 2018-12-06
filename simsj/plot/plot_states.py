import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

# F=1
net = '11110100010110100111010101001011110001011110111010111100011011011001101111111100'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)
plt.tight_layout()
plt.savefig('png/states_F1.png', dpi=600)
print ('Wrote states_F1.png')

# F=1 but one bit flipped                       v--this one
net = '11110100010110100111010101001011110001011010111010111100011011011001101111111100'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)
plt.tight_layout()
plt.savefig('png/states_F1oneflip.png', dpi=600)

# F=0.3
net = '00011110000010010001000111000110111000100001101101101000011101001010110100011101'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)
plt.tight_layout()
plt.savefig('png/states_F0.3.png', dpi=600)

plt.show()
