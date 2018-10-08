#! /usr/bin/env python3

### @file dimacs_test.py
### @brief
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
from pym_combopt import UdGraph

# UdGraph のテスト用クラス
class UdGraphTest(unittest.TestCase) :

    def runTest(self) :
        self.test_dimacs()


    def test_dimacs(self) :
        filename = 'anna.col'

        graph = UdGraph.read_dimacs(filename)

        # graph.node_num == 138
        # graph.edge_num == 986

        ofilename = '??'
        graph.write_dimacs(ofilename)

        # filename == ofilename


if __name__ == '__main__' :

    unittiest.main()
