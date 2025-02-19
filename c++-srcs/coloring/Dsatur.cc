
/// @file Dsatur.cc
/// @brief Dsatur の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Dsatur.h"
#include "DsatNode.h"
#include "ym/UdGraph.h"
#include "ym/HeapTree.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_COLORING

BEGIN_NONAMESPACE

// @brief Dsatur 用の比較関数
class DsatComp
{
public:

  /// @brief コンストラクタ
  DsatComp(
    const Dsatur& dsatur
  ) : mDsatur{dsatur}
  {
  }

  /// @brief ノードの比較関数
  int
  operator()(
    SizeType id1,
    SizeType id2
  ) const
  {
    auto sat1 = mDsatur.sat_degree(id1);
    auto sat2 = mDsatur.sat_degree(id2);
    if ( sat1 < sat2 ) {
      return 1;
    }
    if ( sat1 > sat2 ) {
      return -1;
    }
    auto adj1 = mDsatur.adj_degree(id1);
    auto adj2 = mDsatur.adj_degree(id2);
    if ( adj1 < adj2 ) {
      return 1;
    }
    if ( adj1 > adj2 ) {
      return -1;
    }
    return 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const Dsatur& mDsatur;

};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Dsatur
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Dsatur::Dsatur(
  const UdGraph& graph ///< [in] 対象のグラフ
) : Dsatur{graph, vector<SizeType>(graph.node_num(), 0)}
{
}

// @brief コンストラクタ
Dsatur::Dsatur(
  const UdGraph& graph,
  const vector<SizeType>& color_map
) : ColGraph{graph, color_map},
    mNodeArray(node_num())
{
  if ( node_num() > 0 ) {
    // 部分的に彩色済みの場合には n < node_num()
    auto n = node_list().size() + color_num();
    for ( auto node_id: Range(node_num()) ) {
      auto& node_info = mNodeArray[node_id];
      node_info.mColorSet.resize(n, false);
      node_info.mSatDegree = 0;
    }
    for ( auto node_id: Range(node_num()) ) {
      SizeType c = color(node_id);
      for ( auto node1_id: adj_list(node_id) ) {
	if ( color(node1_id) == 0 ) {
	  // node1 が未着色の場合
	  if ( !check_adj_color(node1_id, c) ) {
	    // node1 にとって color は新規の隣り合う色だった．
	    add_adj_color(node1_id, c);
	  }
	}
      }
    }
  }
}

// @brief 彩色する．
SizeType
Dsatur::coloring(
  vector<SizeType>& color_map
)
{
  DsatComp comp(*this);
  HeapTree<SizeType, DsatComp> node_heap(comp, node_num());
  for ( auto node_id: node_list() ) {
    node_heap.put_item(node_id);
  }

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  auto max_id = node_heap.get_min();
  set_color(max_id, new_color());
  auto node_list = update_sat_degree(max_id);
  for ( auto node_id: node_list ) {
    node_heap.update(node_id);
  }

  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( !node_heap.empty() ) {
    auto max_id = node_heap.get_min();
    // max_node につけることのできる最小の色番号を求める．
    SizeType cnum = 0;
    vector<int> free_list;
    free_list.reserve(adj_list(max_id).size());
    for ( auto node1_id: adj_list(max_id) ) {
      SizeType c = color(node1_id);
      if ( c == 0 ) {
	free_list.push_back(node1_id);
      }
    }
    vector<SizeType> color_list;
    color_list.reserve(color_num());
    for ( auto c: Range(1, color_num() + 1) ) {
      if ( !check_adj_color(max_id, c) ) {
	color_list.push_back(c);
      }
    }
    if ( color_list.empty() ) {
      set_color(max_id, new_color());
    }
    else {
      SizeType min_count = free_list.size() + 1;
      SizeType min_col = 0;
      for ( auto col: color_list ) {
	SizeType n = 0;
	for ( auto node1_id: free_list ) {
	  if ( !check_adj_color(node1_id, col) ) {
	    ++ n;
	  }
	}
	if ( min_count > n ) {
	  min_count = n;
	  min_col = col;
	}
      }
      set_color(max_id, min_col);
    }
    auto node_list = update_sat_degree(max_id);
    for ( auto node_id: node_list ) {
      node_heap.update(node_id);
    }
  }

  // 検証
  // もちろん最小色数の保証はないが，同じ色が隣接していないことを確認する．
  // また，未彩色のノードがないことも確認する．
  // 違反が見つかったら例外を送出する．
  if ( !is_colored() ) {
    throw std::invalid_argument{"not colored"};
  }
  if ( !verify() ) {
    throw std::invalid_argument{"conflicting coloring"};
  }

  // 結果を color_map に入れる．
  return get_color_map(color_map);
}

// @brief SAT degree を返す．
SizeType
Dsatur::sat_degree(
  SizeType node_id
) const
{
  auto& node_info = mNodeArray[node_id];
  return node_info.mSatDegree;
}

// @brief 彩色の結果を SAT degree に反映させる．
vector<SizeType>
Dsatur::update_sat_degree(
  SizeType node_id
)
{
  vector<SizeType> node_list;
  node_list.reserve(adj_list(node_id).size());
  // node に隣接するノードの SAT degree を更新する．
  auto col = color(node_id);
  for ( auto node1_id: adj_list(node_id) ) {
    if ( color(node1_id) == 0 ) {
      // node1 が未着色の場合
      if ( !check_adj_color(node1_id, col) ) {
	// node1 にとって col は新規の隣り合う色だった．
	add_adj_color(node1_id, col);

	// SAT degree が変わったのでヒープ上の位置も更新する．
	node_list.push_back(node1_id);
      }
    }
  }
  return node_list;
}

END_NAMESPACE_YM_COLORING
