
/// @file Dsatur.cc
/// @brief Dsatur の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "Dsatur.h"
#include "DsatNode.h"
#include "NodeHeap.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

BEGIN_NONAMESPACE

// @brief Dsatur 用の比較関数
class DsatComp
{
public:

  /// @brief ノードの比較関数
  int
  operator()(DsatNode* node1,
	     DsatNode* node2);

};

// @brief ノードの比較関数
int
DsatComp::operator()(DsatNode* node1,
		     DsatNode* node2)
{
  if ( node1->sat_degree() < node2->sat_degree() ) {
    return 1;
  }
  if ( node1->sat_degree() > node2->sat_degree() ) {
    return -1;
  }
  if ( node1->adj_degree() < node2->adj_degree() ) {
    return 1;
  }
  if ( node1->adj_degree() > node2->adj_degree() ) {
    return -1;
  }
  return 0;
}


// @brief ノードに彩色して情報を更新する．
void
update_sat_degree(DsatNode* node,
		  ColGraph& graph,
		  DsatNode* node_array,
		  NodeHeap<DsatNode, DsatComp>& node_heap)
{
  // node に隣接するノードの SAT degree を更新する．
  int color = graph.color(node->id());
  for ( auto node1_id: graph.adj_list(node->id()) ) {
    DsatNode* node1 = &node_array[node1_id];
    if ( graph.color(node1_id) == 0 ) {
      // node1 が未着色の場合
      if ( !node1->check_adj_color(color) ) {
	// node1 にとって color は新規の隣り合う色だった．
	node1->add_adj_color(color);

	// SAT degree が変わったのでヒープ上の位置も更新する．
	node_heap.update(node1);
      }
    }
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Dsatur
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
Dsatur::Dsatur(const UdGraph& graph) :
  ColGraph(graph)
{
  init();
}

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
// @param[in] color_map 部分的な彩色結果
Dsatur::Dsatur(const UdGraph& graph,
	       const vector<int>& color_map) :
  ColGraph(graph, color_map)
{
  init();
}

// @brief 初期化する．
void
Dsatur::init()
{
  if ( node_num() > 0 ) {
    // 部分的に彩色済みの場合には n < node_num()
    int n = node_list().num() + color_num();
    int vectlen = (n + 63) / 64;
    mNodeArray = new DsatNode[node_num()];
    for ( auto node_id: Range<>(node_num()) ) {
      DsatNode* node = &mNodeArray[node_id];
      node->init(node_id, adj_list(node_id).num(), vectlen);
    }
    for ( auto node_id: Range<>(node_num()) ) {
      int c = color(node_id);
      for ( auto node1_id: adj_list(node_id) ) {
	if ( color(node1_id) == 0 ) {
	  DsatNode* node1 = &mNodeArray[node1_id];
	  // node1 が未着色の場合
	  if ( !node1->check_adj_color(c) ) {
	    // node1 にとって color は新規の隣り合う色だった．
	    node1->add_adj_color(c);
	  }
	}
      }
    }
  }
  else {
    mNodeArray = nullptr;
  }
}

// @brief デストラクタ
Dsatur::~Dsatur()
{
  delete [] mNodeArray;
}

// @brief 彩色する．
// @param[out] color_map ノードに対する彩色結果(=int)を収める配列
// @return 彩色数を返す．
int
Dsatur::coloring(vector<int>& color_map)
{
  // dsatur アルゴリズムを用いる．

  NodeHeap<DsatNode, DsatComp>  node_heap(node_num());

  for ( auto node_id: node_list() ) {
    node_heap.put_node(&mNodeArray[node_id]);
  }

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  DsatNode* max_node = node_heap.get_min();
  set_color(max_node->id(), new_color());
  update_sat_degree(max_node, *this, mNodeArray, node_heap);

  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( !node_heap.empty() ) {
    DsatNode* max_node = node_heap.get_min();
    // max_node につけることのできる最小の色番号を求める．
    int cnum = 0;
    vector<int> free_list;
    free_list.reserve(adj_list(max_node->id()).num());
    for ( auto node1_id: adj_list(max_node->id()) ) {
      int c = color(node1_id);
      if ( c == 0 ) {
	free_list.push_back(node1_id);
      }
    }
    vector<int> color_list;
    color_list.reserve(color_num());
    for ( auto c: Range<>(1, color_num() + 1) ) {
      if ( !max_node->check_adj_color(c) ) {
	color_list.push_back(c);
      }
    }
    if ( color_list.empty() ) {
      set_color(max_node->id(), new_color());
    }
    else {
      int min_count = free_list.size() + 1;
      int min_col = 0;
      for ( auto col: color_list ) {
	int n = 0;
	for ( auto node1_id: free_list ) {
	  DsatNode* node1 = &mNodeArray[node1_id];
	  if ( !node1->check_adj_color(col) ) {
	    ++ n;
	  }
	}
	if ( min_count > n ) {
	  min_count = n;
	  min_col = col;
	}
      }
      set_color(max_node->id(), min_col);
    }
    update_sat_degree(max_node, *this, mNodeArray, node_heap);
  }

  // 検証
  // もちろん最小色数の保証はないが，同じ色が隣接していないことを確認する．
  // また，未彩色のノードがないことも確認する．
  // 違反が見つかったら例外を送出する．
  if ( true ) {
    ASSERT_COND( is_colored() );
    ASSERT_COND( verify() );
  }
  if ( !is_colored() || !verify() ) {
    cout << "Error" << endl;
  }

  // 結果を color_map に入れる．
  return get_color_map(color_map);
}


//////////////////////////////////////////////////////////////////////
// クラス DsatNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DsatNode::DsatNode()
{
  mColorSet = nullptr;
  mSatDegree = 0;
}

// @brief デストラクタ
DsatNode::~DsatNode()
{
  delete [] mColorSet;
}

// @brief 初期化する．
void
DsatNode::init(int id,
	       int adj_degree,
	       int vectlen)
{
  mId = id;
  mColorSet = new ymuint64[vectlen];
  for ( int i = 0; i < vectlen; ++ i ) {
    mColorSet[i] = 0ULL;
  }
  mAdjDegree = adj_degree;
}

END_NAMESPACE_YM_UDGRAPH
