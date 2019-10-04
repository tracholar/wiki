#coding:utf-8
"""生成常用对数表和反对数表"""

from math import *
import numpy as np



f = open('log10.txt', 'w')
for x in np.arange(1.0001, 10.0, 0.0001):
    if int(x) == x:
        f.write('\n{0:.4f} '.format(x))
    y = log10(x)
    f.write('{0:.4f} '.format(y))
f.close()