#coding:utf-8
import numpy as np
import matplotlib.pyplot as plt


x = 1000*(1/np.random.power(10, size=10000) - 1)

median = np.median(x)
plt.subplot(141)
plt.hist(x, bins=100)
plt.title('origin data median = %.2f' % median)

plt.subplot(142)
plt.hist(np.log1p(x), bins=100)
plt.title('$\log(1 + x)$')

plt.subplot(143)
plt.hist(np.log((1 + x)/(1 + median)), bins=100)
plt.title(r'$\log(\frac{1 + x}{1 + median})$')

plt.subplot(144)
def cdf_trans(sort_arr, xi):
    """
    :param sort_arr:  排好序的数据
    :param xi: 变换的值
    :return: 分位点
    """
    return np.searchsorted(sort_arr, xi) * 1.0 / len(sort_arr)
print cdf_trans(np.sort(x), x)
plt.hist(cdf_trans(np.sort(x), x), bins=100)
plt.title('cdf(x)')

plt.show()

