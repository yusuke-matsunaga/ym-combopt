
/// @file ColGraph.cc
/// @brief ColGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "coloring/ColGraph.h"
#include "ym/UdGraph.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス ColGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ColGraph::ColGraph(
  const UdGraph& graph
)
{
  init(graph, vector<SizeType>(graph.node_num(), 0));
}

// @brief コンストラクタ
ColGraph::ColGraph(
  const UdGraph& graph,
  const vector<SizeType>& color_map
)
{
  init(graph, color_map);
}

// @brief デストラクタ
ColGraph::~ColGraph()
{
  delete [] mAdjListArray;
  delete [] mColorMap;
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

// @brief 内容をセットする．
void
ColGraph::init(
  const UdGraph& graph,
  const vector<SizeType>& color_map
)
{
  mNodeNum = graph.node_num();
  mAdjListArray = new AdjList[mNodeNum];
  mColorMap = new SizeType[mNodeNum];

  // mColorMap の初期化を行う．
  // 同時に使用されている色番号の最大値を求める．
  // ここでは「抜け」はチェックしていない．
  ASSERT_COND( color_map.size() == mNodeNum );
  mNodeNum1 = 0;
  mColNum = 0;
  for ( auto node_id: Range(mNodeNum) ) {
    SizeType c = color_map[node_id];
    mColorMap[node_id] = c;
    if ( c == 0 ) {
      ++ mNodeNum1;
    }
    else {
      if ( mColNum < c ) {
	mColNum = c;
      }
    }
  }

  // 未彩色のノードのリストを作る．
  mNodeList = new SizeType[mNodeNum1];
  SizeType wpos = 0;
  for ( auto node_id: Range(mNodeNum) ) {
    if ( color(node_id) == 0 ) {
      mNodeList[wpos] = node_id;
      ++ wpos;
    }
  }
  ASSERT_COND( wpos == mNodeNum1 );

  // 各ノードに接続する枝数を数える．
  mEdgeNum = 0;
  for ( auto edge: graph.edge_list() ) {
    SizeType id1 = edge.id1();
    SizeType id2 = edge.id2();
    if ( id1 == id2 ) {
      // そもそもセルフループは彩色不可なので無視する．
      continue;
    }
    if ( mColorMap[id1] > 0 && mColorMap[id2] > 0 ) {
      // すでに彩色済みのノードに関する枝も無視する．
      continue;
    }
    ++ mEdgeNum;
    ++ mAdjListArray[id1].mNum;
    ++ mAdjListArray[id2].mNum;
  }

  // 隣接リストの領域を確保する．
  for ( auto i: Range(mNodeNum) ) {
    auto& adj_list = mAdjListArray[i];
    adj_list.mBody = new SizeType[adj_list.mNum];
    // ちょっとしたギミックで mNum を 0 に戻す．
    adj_list.mNum = 0;
  }

  // 隣接リストの設定を行う．
  for ( auto edge: graph.edge_list() ) {
    SizeType id1 = edge.id1();
    SizeType id2 = edge.id2();
    if ( id1 == id2 ) {
      // そもそもセルフループは彩色不可なので無視する．
      continue;
    }
    if ( mColorMap[id1] > 0 && mColorMap[id2] > 0 ) {
      // すでに彩色済みのノードに関する枝も無視する．
      continue;
    }
    AdjList& adj_list1 = mAdjListArray[id1];
    AdjList& adj_list2 = mAdjListArray[id2];
    adj_list1.mBody[adj_list1.mNum] = id2; ++ adj_list1.mNum;
    adj_list2.mBody[adj_list2.mNum] = id1; ++ adj_list2.mNum;
  }
}

END_NAMESPACE_YM_UDGRAPH
