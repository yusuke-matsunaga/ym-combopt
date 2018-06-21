
/// @file max_clique.cc
/// @brief max_clique の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "MclqSolver.h"


BEGIN_NAMESPACE_YM

// @brief 最大クリークを求める．
// @param[in] graph 対象のグラフ
// @param[in] algorithm アルゴリズム名
// @param[out] node_set クリークの要素(ノード番号)を収める配列
// @return 要素数を返す．
int
max_clique(const UdGraph& graph,
	   const string& algorithm,
	   vector<int>& node_set)
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

// @relates UdGraph
// @brief (最大)クリークを求める．
// @param[in] graph 対象のグラフ
// @param[out] node_set クリークの要素(ノード番号)を収める配列
// @return 要素数を返す．
int
max_clique(const UdGraph& graph,
	   vector<int>& node_set)
{
  return max_clique(graph, string(), node_set);
}

END_NAMESPACE_YM
