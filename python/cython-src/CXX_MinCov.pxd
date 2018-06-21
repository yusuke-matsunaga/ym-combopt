
### @file CXX_MinCov.pxd
### @brief CXX_MinCov 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.string cimport string


cdef extern from "ym/MinCov.h" namespace "nsYm" :

    ### @brief MinCov クラスの cython バージョン
    cdef cppclass MinCov :
        MinCov()
        void set_size(int, int)
        void set_col_cost(int, int)
        void insert_elem(int, int)
        int exact(vector[int]&)
        int heuristic(vector[int]&)
        int heuristic(const string&, vector[int]&)
        void set_partition_flag(bool)
        void set_debug_flag(bool)
        void set_max_depth(int)
