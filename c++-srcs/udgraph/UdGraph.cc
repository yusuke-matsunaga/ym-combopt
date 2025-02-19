
/// @file UdGraph.cc
/// @brief UdGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス UdGraph
//////////////////////////////////////////////////////////////////////

// @brief 反射の時に true を返す．
//
// 反射とはすべてのノードに自己ループがあること
bool
UdGraph::is_reflective() const
{
  vector<bool> mark(mNodeNum, false);
  for ( auto edge: mEdgeList ) {
    auto id1 = edge.id1();
    auto id2 = edge.id2();
    if ( id1 == id2 ) {
      mark[id1] = true;
    }
  }

  for ( auto id: Range(node_num()) ) {
    if ( !mark[id] ) {
      return false;
    }
  }
  return true;
}

// @brief グラフの内容を出力する．
void
UdGraph::dump(
  ostream& s
) const
{
  vector<vector<SizeType>> link_list(node_num());
  for ( auto edge: mEdgeList ) {
    auto id1 = edge.id1();
    auto id2 = edge.id2();
    link_list[id1].push_back(id2);
    link_list[id2].push_back(id1);
  }

  for ( auto id1: Range(node_num()) ) {
    cout << id1 << ": ";
    for ( auto id2: link_list[id1] ) {
      cout << " " << id2;
    }
    cout << endl;
  }
  cout << endl;
}

END_NAMESPACE_YM_UDGRAPH
