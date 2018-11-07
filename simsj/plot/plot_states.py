import matplotlib
matplotlib.use ('TKAgg', warn=False, force=True)
import matplotlib.pyplot as plt

import states_lib as sl

net = '00111101110001011111111001001001001000011101111010111110101011110011101100110111'
F1 = plt.figure (figsize=(8,8))
f1 = F1.add_subplot (111)
sl.plot_states (net, f1.axes)

plt.show()
plt.savefig('states.png', dpi=600)
print ('Wrote state.png')
