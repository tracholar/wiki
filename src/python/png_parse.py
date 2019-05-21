#coding:utf-8
import os
import struct


def read_int(fp):
    buf = fp.read(4)
    buf = struct.unpack('i',buf)
    return buf
def read_short(fp):
    buf = fp.read(2)
    buf = struct.unpack('h',buf)
    return buf

fp = open('./data/data.png', 'rb')

buf = read_short(fp)

print '%x' % (buf),