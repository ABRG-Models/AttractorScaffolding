import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

# F=?
net = '1111010001011010011101010100101111000101111011101011110001101101100110111111110011110100010110100111010101001011110001011110111010111100011011011001101111111100'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)
plt.tight_layout()
plt.savefig('png/states_F1.png', dpi=600)
print ('Wrote states_F1.png')


plt.show()
