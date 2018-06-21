
### @file mincov.pxi
### @brief mincov の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from CXX_MinCov cimport MinCov as CXX_MinCov


### @brief MinCov の Python バージョン
cdef class MinCov :
    cdef CXX_MinCov _this

    ### @brief 初期化
    def __init__(MinCov self, *args) :
        cdef int col_size
        cdef vector[int] c_cost_array
        if len(args) == 0 :
            pass
        elif len(args) == 2 and type(args[0]) == int :
            if type(args[1]) == int :
                self.resize(args[0], args[1])
            elif type(args[1]) in ( list, tuple ) :
                col_size = len(args[1])
                c_cost_array.resize(col_size)
                for i in range(col_size) :
                    c_cost_array[i] = args[1][i]
                self.resize(args[0], c_cost_array)
            else :
                # 引数がおかしい
                assert False
        else :
            # 引数がおかしい
            assert False

    ### @brief 問題のサイズを設定する．
    ### @param[in] row_size 行数
    ### @param[in] col_size 列数
    def resize(MinCov self, int row_size, int col_size) :
        self._this.resize(row_size, col_size)

    ### @brief 問題のサイズを設定する(コスト配列付き)．
    ### @param[in] row_size 行数
    ### @param[in] col_cost_array 列のコスト配列(リスト)
    def resize(MinCov self, int row_size, col_cost_array) :
        cdef int col_size = len(col_cost_array)
        cdef vector[int] c_col_cost_array
        c_col_cost_array.resize(col_size)
        for i in range(col_size) :
            c_col_cost_array[i] = col_cost_array[i]
        self._this.resize(row_size, c_col_cost_array)

    ### @brief 列のコストを設定する．
    ### @param[in] col_pos 列番号 ( 0 <= col_pos < col_size )
    ### @param[in] cost コスト
    def set_col_cost(MinCov self, int col_pos, int cost) :
        self._this.set_col_cost(col_pos, cost)

    ### @brief 要素を追加する．
    ### @param[in] row_pos 行番号 ( 0 <= row_pos < row_size )
    ### @param[in] col_pos 列番号 ( 0 <= col_pos < col_size )
    def insert_elem(MinCov self, int row_pos, int col_pos) :
        self._this.insert_elem(row_pos, col_pos)

    ### @brief 行数を得る．
    @property
    def row_size(MinCov self) :
        return self._this.row_size()

    ### @brief 列数を得る．
    @property
    def col_size(MinCov self) :
        return self._this.col_size()

    ### @brief 列のコスト配列(リスト)を得る．
    def col_cost_array(MinCov self) :
        return [ self._this.col_cost(col) for col in range(self.col_size) ]

    ### @brief 要素のリストを得る．
    ###
    ### * 実際にはジェネレータを返す．
    ### * 要素は (row_pos, col_pos) のタプルで表す．
    def elem_list(MinCov self) :
        cdef vector[pair[int, int]] c_elem_list = self._this.elem_list()
        cdef int n = c_elem_list.size()
        cdef int row_pos, col_pos
        for i in range(n) :
            row_pos = c_elem_list[i].first
            col_pos = c_elem_list[i].second
            yield row_pos, col_pos

    ### @brief 最小被覆問題の厳密解を求める．
    ### @param[in] option オプション文字列(キーワード引数)
    ### @return 解のコストと解の列のリストのタプル
    def exact(MinCov self, **kwargs) :
        cdef string c_option
        cdef vector[int] c_solution
        cdef int cost
        if kwargs['option'] :
            c_option = kwargs['option'].encode('UTF-8')
        cost = self._this.exact(c_solution, c_option)
        solution = [ c_solution[i] for i in range(c_solution.size()) ]
        return cost, solution

    ### @brief 最小被覆問題の近似解を求める．
    ### @param[in] algorithm アルゴリズムの名前(キーワード引数)
    ### @param[in] option オプション文字列(キーワード引数)
    ### @return 解のコストと解の列のリストのタプル
    def heuristic(MinCov self, **kwargs) :
        pass
