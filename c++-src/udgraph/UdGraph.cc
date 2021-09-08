
/// @file UdGraph.cc
/// @brief UdGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス UdGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UdGraph::UdGraph(
  SizeType node_num
) : mNodeNum{node_num}
{
}

// @brief デストラクタ
UdGraph::~UdGraph()
{
}

// @brief ノード数を(再)設定する．
//
// 以前の内容は消去される．
void
UdGraph::resize(
  SizeType node_num
)
{
  mEdgeList.clear();
  mNodeNum = node_num;
}

// @brief 2つのノードを接続する．
void
UdGraph::connect(
  SizeType id1,
  SizeType id2
)
{
  ASSERT_COND( id1 >= 0 && id1 < node_num() );
  ASSERT_COND( id2 >= 0 && id2 < node_num() );

  // 正規化する
  if ( id1 > id2 ) {
    std::swap(id1, id2);
  }

  mEdgeList.push_back(UdEdge(id1, id2));
}

// @brief ノード数を得る．
SizeType
UdGraph::node_num() const
{
  return mNodeNum;
}

// @brief 枝の総数を返す．
SizeType
UdGraph::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝の情報を返す．
// @return 枝を返す．
const UdEdge&
UdGraph::edge(
  SizeType idx
) const
{
  ASSERT_COND( idx >= 0 && idx < edge_num() );

  return mEdgeList[idx];
}

// @brief 全ての枝のリストを返す．
const vector<UdEdge>&
UdGraph::edge_list() const
{
  return mEdgeList;
}

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
