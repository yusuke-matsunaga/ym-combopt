
### @file CXX_UdGraph.pxd
### @brief CXX_UdGraph 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.string cimport string


cdef extern from "ym/UdGraph.h" namespace "nsYm" :

    # UdEdge クラスの cython バージョン
    cdef cppclass UdEdge :
        UdEdge()
        UdEdge(int id1, int id2)
        int id1()
        int id2()

    # UdGraph クラスの cython バージョン
    cdef cppclass UdGraph :
        UdGraph()
        UdGraph(int node_num)
        void resize(int node_num)
        void connect(int id1, int id2)
        int node_num()
        int edge_num()
        bool is_reflective()
        UdEdge& edge(int)
        vector[UdEdge]& edge_list()
        int coloring(vector[int]& color_map)
        int coloring(vector[int]& color_map, const string&)

    bool read_dimacs(const string&, UdGraph&)
    void write_dimacs(const string&, UdGraph&)
    int independent_set(UdGraph&, vector[int]& node_set)
    int independent_set(UdGraph&, vector[int]& node_set, const string&)
    int max_clique(UdGraph&, vector[int]& node_set)
    int max_clique(UdGraph&, vector[int]& node_set, const string&)
