#coding:utf-8
"""
贷款计算器
"""

import argparse


def calc_fp(lv, i, n):
    r = sum(1/pow(1 + i, k) for k in range(1, n+1))
    return lv / r

def calc_fp2(lv, i, n, ni):
    s = lv / n
    return s + (lv - s * (ni - 1)) * i

if __name__ == '__main__':
    ## 等额本息
    print calc_fp(726000, 0.0325/12, 12*30) # 实际值 3159.60
    print calc_fp(940000, 0.0539/12, 12*30) # 实际值 5272.48

    ## 等额本金
    print calc_fp2(726000, 0.0325/12, 12*30, 1)
    print calc_fp2(940000, 0.0539/12, 12*30, 1)