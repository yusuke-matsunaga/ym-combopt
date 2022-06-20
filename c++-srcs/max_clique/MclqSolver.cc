
/// @file MclqSolver.cc
/// @brief MclqSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "MclqSolver.h"
#include "MclqNode.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_UDGRAPH

// @brief 最大クリークを求める．
vector<SizeType>
UdGraph::max_clique(
  const string& algorithm
) const
{
  MclqSolver solver{*this};

  if ( algorithm == "exact" ) {
    return solver.exact();
  }
  else if ( algorithm == "greedy" ) {
    return solver.greedy();
  }
  else {
    // デフォルトフォールバック
    return solver.greedy();
  }
}

// @brief コンストラクタ
MclqSolver::MclqSolver(
  const UdGraph& graph
) : mNodeNum{graph.node_num()}
{
  mNodeArray = new MclqNode[mNodeNum];
  for ( SizeType i = 0; i < mNodeNum; ++ i ) {
    auto node = &mNodeArray[i];
    node->set(i);
  }

  // 隣接するノードの情報を link_array に入れる．
  vector<vector<SizeType>> link_array(mNodeNum);
  for ( auto edge: graph.edge_list() ) {
    SizeType id1 = edge.id1();
    SizeType id2 = edge.id2();
    link_array[id1].push_back(id2);
    link_array[id2].push_back(id1);
  }

  // link_array の情報を MclqNode に移す．
  for ( SizeType i = 0; i < mNodeNum; ++ i ) {
    MclqNode* node1 = &mNodeArray[i];
    const vector<SizeType>& link_list = link_array[i];
    SizeType link_num = link_list.size();
    MclqNode** adj_link = new MclqNode*[link_num];
    for ( SizeType j = 0; j < link_num; ++ j ) {
      MclqNode* node2 = &mNodeArray[link_list[j]];
      adj_link[j] = node2;
    }
    node1->set_adj_link(link_num, adj_link);
  }
}

// @brief デストラクタ
MclqSolver::~MclqSolver()
{
  delete [] mNodeArray;
}

END_NAMESPACE_YM_UDGRAPH
