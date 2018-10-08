
### @file udgraph.pxi
### @brief udgraph の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.string cimport string
from libcpp.vector cimport vector
from CXX_UdGraph cimport UdEdge as CXX_UdEdge, UdGraph as CXX_UdGraph
from CXX_UdGraph cimport read_dimacs as c_read_dimacs, write_dimacs as c_write_dimacs


### @brief UdGraph の Python バージョン
cdef class UdGraph :
    cdef CXX_UdGraph _this

    ### @brief 初期化
    def __init__(self, *args) :
        if len(args) == 0 :
            pass
        elif len(args) == 1 :
            node_num = int(args[0])
            self._this.resize(node_num)
        else :
            assert False

    ### @brief ノード数を再設定する．
    def resize(self, int node_num) :
        self._this.resize(node_num)

    ### @brief 枝を追加する．
    def connect(self, int id1, int id2) :
        self._this.connect(id1, id2)

    ### @brief ノード数を返す．
    @property
    def node_num(self) :
        return self._this.node_num()

    ### @brief 枝数を返す．
    @property
    def edge_num(self) :
        return self._this.edge_num()

    ### @brief 枝のリストを返す．
    def edge_list(self) :
        cdef CXX_UdEdge c_edge
        for i in range(self._this.edge_num()) :
            c_edge = self._this.edge(i)
            yield c_edge.id1(), c_edge.id2()

    ### @brief DIMACS 形式のファイルを読み込むクラスメソッド
    @staticmethod
    def read_dimacs(str filename) :
        cdef string c_str = filename.encode('UTF-8')
        cdef UdGraph graph = UdGraph()
        stat = c_read_dimacs(c_str, graph._this)
        if stat :
            return graph
        else :
            return None

    ### @brief DIMACS 形式でファイルに書き出す．
    def write_dimacs(self, str filename) :
        cdef string c_str = filename.encode('UTF-8')
        c_write_dimacs(c_str, self._this)

    ### @brief 彩色問題を解く
    def coloring(self, *args) :
        cdef vector[int] c_color_map
        cdef string c_algorithm
        cdef int nc
        if len(args) == 1 and type(args[0]) == str :
            c_algorithm = args[0].encode('UTF-8')
            nc = self._this.coloring(c_algorithm, c_color_map)
        else :
            nc = self._this.coloring(c_color_map)
        return nc, [ c_color_map[i] for i in range(self.node_num) ]
