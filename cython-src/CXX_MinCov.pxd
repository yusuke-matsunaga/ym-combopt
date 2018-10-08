
### @file CXX_MinCov.pxd
### @brief CXX_MinCov 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.pair cimport pair
from libcpp.string cimport string


cdef extern from "ym/MinCov.h" namespace "nsYm" :

    ### @brief MinCov クラスの cython バージョン
    cdef cppclass MinCov :
        MinCov()
        void resize(int, int)
        void resize(int, const vector[int]&)
        void set_col_cost(int, int)
        void insert_elem(int, int)
        int row_size()
        int col_size()
        int col_cost(int)
        const vector[pair[int, int]]& elem_list()
        int exact(vector[int]&, const string&)
        int heuristic(vector[int]&, const string&, const string&)
