import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

#net = '00111101110001011111111001001001001000011101111010111110101011110011101100110111'
net = '01000110101110000010111011111000010010100010110010001001111110001110000110001101' # The net that had different fitness for Stuarts and my code.
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)

plt.show()
plt.savefig('states.png', dpi=600)
print ('Wrote state.png')
