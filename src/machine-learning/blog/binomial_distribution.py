#coding:utf-8

import numpy as np
from scipy.special import comb, perm
import matplotlib.pyplot as plt

n = 100
p = 0.1
x = range(101)
y = [comb(n, k) * np.power(p, k) * np.power(1-p, n-k) for k in x]
x2 = np.linspace(0,25, 100)
z = [1/np.sqrt(2*np.pi*n*p*(1-p))*np.exp(-(k - n*p)**2 / 2/(n*p*(1-p))) for k in x2]

plt.bar(x, y)
plt.plot(x2,z, 'r-')
plt.xlim([0,25])
plt.xlabel(u'红球的次数 m')
plt.ylabel(u'概率')

plt.text(15, 0.08, r'$\frac{1}{\sqrt{2\pi np(1-p)}} e^{-\frac{(m - np)^2}{2 np(1-p)}}$')
plt.show()