#coding:utf-8
"""利用维纳过程和伊藤过程仿真股票价格"""
import numpy as np
import matplotlib.pyplot as plt


def wiener_sim(S0, T, a, b):
    """
    维纳过程仿真股票价格
    :param S0: 初始价格
    :param T: 仿真时间长度
    :param a: 连续复利下的年化
    :param b: 年波动率
    :return: array
    """
    S = []
    a /= 250
    b /= np.sqrt(250)
    t = range(T)
    for i in t:
        S.append(S0)
        dx = a + b * np.random.randn()
        S0 += S0 * dx
    return t, S


if __name__ == '__main__':
    t, S = wiener_sim(1, 250*20, 0.000542*250, 0.014151*np.sqrt(250))
    t = [ti/250.0 for ti in t]
    plt.plot(t, S)

    plt.show()
