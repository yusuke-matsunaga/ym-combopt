#! /usr/bin/env python3

### @file color-test.py
### @brief ym-udgraph の彩色問題のテストプログラム
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from udgraph import *
import argparse
import os.path
import time

def main() :

    parser = argparse.ArgumentParser()

    parser.add_argument('--algorithm', type = str, metavar = '<algorithm name>',
                        help = 'specify the coloring algorithm')

    parser.add_argument('file_list', metavar = '<filename>', type = str,
                        nargs = '+',
                        help = 'file name')

    args = parser.parse_args()
    if not args :
        exit(1)

    algorithm = args.algorithm

    for file_name in args.file_list :
        graph = UdGraph.read_dimacs(file_name)
        if not graph :
            print('Error, could not read {}'.format(file_name))
            continue

        start = time.process_time()

        if algorithm :
            nc, c_map = coloring(graph, algorithm)
        else :
            nc, c_map = coloring(graph)

        end = time.process_time()
        cpu_time = end - start

        print('file name:       {}'.format(file_name))
        print('# of nodes:      {:8d}'.format(graph.node_num))
        print('# of edges:      {:8d}'.format(graph.edge_num))
        print('# of colors:     {:8d}'.format(nc))
        print('CPU time:        {:8.2f}'.format(cpu_time))


if __name__ == '__main__' :
    main()
