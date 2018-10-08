
/// @file MclqSolver.cc
/// @brief MclqSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "MclqSolver.h"
#include "MclqNode.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_UDGRAPH

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
MclqSolver::MclqSolver(const UdGraph& graph) :
  mNodeNum(graph.node_num())
{
  mNodeArray = mAlloc.get_array<MclqNode>(mNodeNum);
  for ( int i = 0; i < mNodeNum; ++ i ) {
    auto node = &mNodeArray[i];
    node->set(i);
  }

  // 隣接するノードの情報を link_array に入れる．
  vector<vector<int> > link_array(mNodeNum);
  for ( auto edge: graph.edge_list() ) {
    int id1 = edge.id1();
    int id2 = edge.id2();
    link_array[id1].push_back(id2);
    link_array[id2].push_back(id1);
  }

  // link_array の情報を MclqNode に移す．
  for ( int i = 0; i < mNodeNum; ++ i ) {
    MclqNode* node1 = &mNodeArray[i];
    const vector<int>& link_list = link_array[i];
    int link_num = link_list.size();
    MclqNode** adj_link = mAlloc.get_array<MclqNode*>(link_num);
    for ( int j = 0; j < link_num; ++ j ) {
      MclqNode* node2 = &mNodeArray[link_list[j]];
      adj_link[j] = node2;
    }
    node1->set_adj_link(link_num, adj_link);
  }
}

// @brief デストラクタ
MclqSolver::~MclqSolver()
{
}

END_NAMESPACE_YM_UDGRAPH
