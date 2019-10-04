#coding:utf-8
"""期权期货及其他衍生品 书籍中的代码"""
import numpy as np
from math import exp

def coupon_duration(c_array, y):
    """ 计算带息债券的久期
    :param c_array: 现金流 array<double> or array<(t:double, c:double)>
    :param y: 连续复利利率
    :return: 久期
    """
    if type(c_array) is not list:
        raise Exception("c_array should be array, but found {0}".format(type(c_array)))
    if type(c_array[0]) is not tuple:
        c_array = [(ti+1, ci) for ti, ci in enumerate(c_array)]
    ### 计算债券的折现值
    B = sum(ci*exp(-y*ti) for ti,ci in c_array)

    ### 计算久期, 用折现值加权的平均时间
    D = sum(ti*ci*exp(-y*ti) for ti,ci in c_array)/B
    return D

def continue_interest(Rm, m):
    """
    连续复利计算
    :param Rm: 利率
    :param m: 每年复利次数
    :return:
    """
    return m*np.log(1+Rm/m)

def current_value(A, r, n, ci=True):
    """
    折现值计算
    :param A: 到期现金流
    :param r: 折现率
    :param n: 期数
    :param ci: 折现率是否为连续利率
    :return: 现值
    """
    if ci:
        k = exp(-r*n)
    else:
        k = 1.0/pow(1+r, n)
    return A*k


if __name__ == '__main__':
    c_array = [(0.5*i,5) for i in range(1,7)]
    ti, ci = c_array[-1]
    c_array[-1] = (ti, ci+100)
    print coupon_duration(c_array, 0.12)

    print current_value(405, 0.1-0.04, 4/12.)

