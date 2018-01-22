
from __future__ import division
import numpy as np
import matplotlib.pyplot as plt

# generate data
# Tr=Tc=64
colors = ['b', 'c', 'y', 'm', 'r']
x1 = [8, 16, 32, 64, 96, 128, 256]
x2 = [4, 5, 6, 7, 8, 16, 32, 64, 128]
x3 = []
x4 = []
k = []
x = []
#x = [float(xi * 8 * 32 * 32 * 25) for xi in x1]
#for tm in x1:
#    for tn in x2:
#        x.append(tm*tn*32*32*25)

Y = []
X = []
Y1 = []
X1 = []
Y2 = []
X2 = []
#for xi in x:
#    for si in xrange(1, 11):
#        X.append(xi)
        #yi = ((xi * 8 * 25 + 8 * (15 * si + 5) ** 2 + xi * 16 * 16) * 4) / ((16 * 16 * 25 + xi*16 * 16) * 10) / 100000 * si
#        yi = ((xi * 8 * 25 + 8 * (54 * si + 5) ** 2 + xi * 55 * 55) * 4) / ((55 * 55 * 25 + (xi/4) * 55 * 55) * 10)
#        #yi = ((xi * 8 * 25 + 8 * (15 * si + 5) ** 2 + xi * 16 * 16) * 4) / ((16 * 16 * 25 + xi*16 * 16) * 10)
#        Y.append(yi)

# Tr = Tc = 55
# Clock = 10ns
for tm in x1:
   for tn in x2:
       if tm * tn * 5 < 6840:
           for si in xrange(1, 6):
               for tr in xrange(8, 128):
                   #for tc in xrange(8, 64):
                   for k in xrange(3, 11):
                       yi = float(tm * tn * tr * tr * k * k) / float(tr * tr * k * k * 10)
                       xi = (yi) / (((tm * tn * k * k + tn * ((tr - 1) * si + k) ** 2 + tm * tr * tr) * 4) / (
                               (tr * tr * k * k + tm * tr * tr) * 10))
                       # xi = float(tm * tn * k * k * tr * tr)/((tm * tn * k*k + tn * ((tr-1) * si + k) * ((tr-1) * si + k) + tm * tr * tr) * 4)
                       if (si == 1) and (k == 5):
                           if (tm == 64) and (tn == 16):
                               Y2.append(yi)
                               X2.append(xi)
                           else:
                               Y1.append(yi)
                               X1.append(xi)
                       else:
                           Y.append(yi)
                           X.append(xi)

#print X
#print Y
#print min(X)

Y_sim = []
X_sim = []
Y1_sim = []
X1_sim = []
Y2_sim = []
X2_sim = []

for yi in Y:
    for count in xrange(0,5):
        Y_sim.append(yi)


fig = plt.figure()
ax1 = fig.add_subplot(111)
#ax2 = fig.add_subplot(111)

# set title
#ax1.set_title('CNN size with BW requirement')

# set X-axis label
plt.xlabel('Computation to Communication ratio (GFLOPS/(GB/S))')
# set Y-axis label
plt.ylabel('CONV ACC performance (GFLOPS)')

# draw figure
p1 = ax1.scatter(X, Y, c='Green', label='possible points', marker='.')
p2 = ax1.scatter(X1, Y1, c='Red', label='S=1,K=5', marker='2')
p3 = ax1.scatter(X2, Y2, c='Blue', label='Tm=64,Tn=16', marker='^')
# # set legend
plt.legend((p1, p2, p3), ('All possible designs', 'Designs with S=1, K=5', 'Designs with Tm=64, Tn=16'), scatterpoints=1, loc='lower right', ncol=1, fontsize=8)

#plt.grid(False)

# show figure
plt.show()
