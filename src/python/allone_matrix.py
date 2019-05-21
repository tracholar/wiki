#coding:utf-8
import numpy as np

def random_matrix(n=10, t = 0.5):
    X = np.random.rand(n, n)
    X = X + X.T
    return (X>t).astype(int)

def all_one_sub_matrix(X):
    pass

if __name__ == '__main__':
    print random_matrix()
