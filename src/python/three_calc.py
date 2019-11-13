#coding:utf-8

import numpy as np
import scipy.integrate as integrate
import matplotlib.pyplot as plt
from scipy.optimize import newton

def f(x):
    s = integrate.quad(lambda t: np.sqrt(1+9*np.power(t, 4)), 0, x)
    return s[0]


print newton(lambda x: f(x) - 1., 1.)

x = np.linspace(0, 10,100)
plt.plot(x, [f(i) for i in x])
plt.show()


