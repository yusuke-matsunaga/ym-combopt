
/// @file max_clique.cc
/// @brief max_clique の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#error "obsolete"

#include "ym/udgraph_nsdef.h"
#include "MclqSolver.h"


BEGIN_NAMESPACE_YM

// @brief 最大クリークを求める．
int
max_clique(
  const UdGraph& graph,
  vector<int>& node_set,
  const string& algorithm
)
{
  nsUdGraph::MclqSolver solver(graph);

  if ( algorithm == "exact" ) {
    return solver.exact(node_set);
  }
  else if ( algorithm == "greedy" ) {
    return solver.greedy(node_set);
  }
  else {
    // デフォルトフォールバック
    return solver.greedy(node_set);
  }
}

END_NAMESPACE_YM
