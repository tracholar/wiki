#coding:utf-8
"""
基金定投计算器
"""

import requests as rq
import time
import json

def get_fund_data(code, start='2019-01-01', days=2000):
    api = 'http://api.fund.eastmoney.com/f10/lsjz?fundCode={code}&pageIndex=1&pageSize={days}&startDate={start}&endDate=&_={ts}'.format(ts=int(time.time()), code=code, start=start, days=days)
    headers = {
        "Referer" : "http://fundf10.eastmoney.com/jjjz_{0}.html".format(code)
    }
    rsp = rq.get(api, headers=headers)
    f = open('{0}.json'.format(code), 'w')
    f.write(rsp.content)
    f.close()

def get_hs300():
    get_fund_data('110020', start='2014-01-01', days=5000)

def get_zz500():
    get_fund_data('160119', start='2014-01-01', days=5000)

def get_zzxf():
    get_fund_data('000248', start='2014-01-01', days=5000)

def get_zzhl():
    pass
def sim_calc(index_value):
    pass

if __name__ == '__main__':
    get_zz500()