#coding:utf-8
"""验证股票价格变动量服从正态分布"""

import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


data = json.load(open('110020.json'))
data = {x['FSRQ'] : float(x['DWJZ']) for x in data['Data']['LSJZList']}
data = sorted(data.items(), key=lambda x:x[0])
ds = [y[1]/x[1]-1 for y,x in zip(data[1:], data[:-1])]
print data
print ds

print pd.Series(ds).describe()
f = plt.hist(ds, bins=50)

f = plt.gcf()
f.savefig('hs300_inc_ratio.svg')

plt.show()
