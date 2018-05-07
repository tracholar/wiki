#coding:utf-8

import os
import imp
from argparse import ArgumentParser

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--list', '-l', default=False, action="store_true", help='列出所有模型')
    parser.add_argument('--model', '-m', help='模型名字')

    args = parser.parse_args()
    print args.__dict__

    if args.list is True:
        # list model
        models = [f[:-3] for f in os.listdir('.') if f not in ('.', '..', __file__) and len(f)>3 and f[-3:]=='.py']
        print '  -','\n  - '.join(models)
    elif args.model is not None:
        os.system('python {0}.py'.format(args.model))