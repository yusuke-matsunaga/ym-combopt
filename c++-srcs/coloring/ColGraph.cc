
/// @file ColGraph.cc
/// @brief ColGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "coloring/ColGraph.h"
#include "ym/UdGraph.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_COLORING

//////////////////////////////////////////////////////////////////////
// クラス ColGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ColGraph::ColGraph(
  const UdGraph& graph
) : ColGraph(graph, vector<SizeType>(graph.node_num(), 0))
{
}

// @brief コンストラクタ
ColGraph::ColGraph(
  const UdGraph& graph,
  const vector<SizeType>& color_map
) : mNodeNum{graph.node_num()},
    mAdjListArray(mNodeNum),
    mColorMap(mNodeNum, 0)
{
  // mColorMap の初期化を行う．
  // 同時に使用されている色番号の最大値を求める．
  // ここでは「抜け」はチェックしていない．
  // さらに未彩色のノードのリストを作る．
  if ( color_map.size() != mNodeNum ) {
    throw std::invalid_argument{"color_map.size() != graph.node_num()"};
  }
  mNodeList.reserve(mNodeNum);
  mColNum = 0;
  for ( auto node_id: Range(mNodeNum) ) {
    SizeType c = color_map[node_id];
    mColorMap[node_id] = c;
    if ( c == 0 ) {
      mNodeList.push_back(node_id);
    }
    else {
      mColNum = std::max(mColNum, c);
    }
  }

  // 隣接リストの設定を行う．
  mEdgeNum = 0;
  for ( auto edge: graph.edge_list() ) {
    auto id1 = edge.id1();
    auto id2 = edge.id2();
    if ( id1 == id2 ) {
      // そもそもセルフループは彩色不可なので無視する．
      continue;
    }
    if ( mColorMap[id1] > 0 && mColorMap[id2] > 0 ) {
      // すでに彩色済みのノードに関する枝も無視する．
      continue;
    }
    ++ mEdgeNum;
    mAdjListArray[id1].push_back(id2);
    mAdjListArray[id2].push_back(id1);
  }
}

// @brief 彩色結果を得る．
SizeType
ColGraph::get_color_map(
  vector<SizeType>& color_map
) const
{
  color_map.clear();
  color_map.resize(mNodeNum);
  for ( auto i: Range(mNodeNum) ) {
    color_map[i] = mColorMap[i];
  }
  return mColNum;
}

// @brief 全てのノードが彩色されていたら true を返す．
bool
ColGraph::is_colored() const
{
  for ( auto id: Range(mNodeNum) ) {
    if ( color(id) == 0 ) {
      return false;
    }
  }
  return true;
}

// @brief 彩色結果が妥当か検証する．
// @retval true 妥当な彩色結果だった．
// @retval false 隣接ノード間で同じ色が割り当てられていた．
//
// ここでは彩色されていないノードの有無はチェックしない．
bool
ColGraph::verify() const
{
  for ( auto id1: Range(mNodeNum) ) {
    for ( auto id2: adj_list(id1) ) {
      if ( color(id1) == color(id2) ) {
	return false;
      }
    }
  }
  return true;
}

END_NAMESPACE_YM_COLORING
