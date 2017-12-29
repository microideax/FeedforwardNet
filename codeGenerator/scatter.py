

import numpy as np
import matplotlib.pyplot as plt

# generate data
x1 = [32, 64, 96, 128]
x2 = []
x3 = []
x4 = []
k = []
x = [float(xi * 8 * 16 * 16 * 25) for xi in x1]

Y = []
X = []

for xi in x:
    for si in xrange(1, 6):
        X.append(xi)
        # yi = ((xi * 8 * 25 + 8 * (15 * si + 5) ** 2 + xi * 16 * 16) * 4) / ((16 * 16 * 25 + 16 * 16) * 10) / 100000 * si
        yi = ((xi * 8 * 25 + 8 * (15 * si + 5) ** 2 + xi * 16 * 16) * 4) / ((16 * 16 * 25 + xi*16 * 16) * 10)
        Y.append(yi)

print X
print Y

fig = plt.figure()
ax1 = fig.add_subplot(111)

# set title
ax1.set_title('CNN size with BW requirement')

# set X-axis label
plt.xlabel('CONV ACC size')

# set Y-axis label
plt.ylabel('BW requirement')

# draw figure
ax1.scatter(X, Y, c='red', marker='o')

# # set legend
plt.legend('X')

plt.grid(True)

# show figure
plt.show()