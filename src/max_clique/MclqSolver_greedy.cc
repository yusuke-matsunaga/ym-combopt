
/// @file MclqSolver_greedy.cc
/// @brief MclqSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "MclqSolver.h"
#include "MclqNode.h"
#include "NodeHeap.h"


BEGIN_NAMESPACE_YM_UDGRAPH

BEGIN_NONAMESPACE

// @brief max_clique 用の NodeHeap
class MclqComp
{
public:

  /// @brief 比較関数
  /// @param[in] node1, node2 対象のノード
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  int
  operator()(MclqNode* node1,
	     MclqNode* node2);

};

// @brief 比較関数
// @param[in] node1, node2 対象のノード
// @retval 負の値 node1 が node2 より前にある．
// @retval 0 node1 と node2 は同じ
// @retval 正の値 node1 が node2 より後ろにある．
int
MclqComp::operator()(MclqNode* node1,
		     MclqNode* node2)
{
  return node2->adj_num() - node1->adj_num();
}

END_NONAMESPACE

// @brief greedy ヒューリスティックで解を求める．
int
MclqSolver::greedy(vector<int>& node_set)
{
  using MclqHeap = NodeHeap<MclqNode, MclqComp>;
  MclqHeap node_heap(mNodeNum);

  // ノードをヒープに積む．
  vector<MclqNode*> node_list;
  for ( int i = 0; i < mNodeNum; ++ i ) {
    auto node = &mNodeArray[i];
    node_heap.put_node(node);
    node_list.push_back(node);
  }

  // 作業用のフラグ配列
  bool* tmp_mark = mAlloc.get_array<bool>(mNodeNum);
  for ( int i = 0; i < mNodeNum; ++ i ) {
    tmp_mark[i] = false;
  }

  node_set.clear();

  // 未処理の MclqNode のうち MclqNode::adj_num() が最大のものを取り出し，解に加える．
  while ( !node_heap.empty() ) {
    MclqNode* best_node = node_heap.get_min();
    node_set.push_back(best_node->id());

    for ( int i = 0; i < best_node->adj_size(); ++ i ) {
      // best_node に隣接しているノードにマークをつける．
      MclqNode* node2 = best_node->adj_node(i);
      tmp_mark[node2->id()] = true;
    }

    // マークのついていないノードを削除する．
    // node_list を直接操作するのは面倒なので
    // 一旦 tmp_list にコピーする．
    vector<MclqNode*> tmp_list;
    tmp_list.reserve(node_list.size());
    for ( auto node: node_list ) {
      if ( node != best_node ) {
	ASSERT_COND( !node->deleted() );
	tmp_list.push_back(node);
      }
    }
    node_list.clear();
    for ( auto node: tmp_list ) {
      if ( tmp_mark[node->id()] ) {
	node_list.push_back(node);
      }
      else {
	// このノードを削除する．
	node_heap.delete_node(node);
	// さらにこのノードに隣接しているノードの隣接数を減らす．
	for ( int i = 0; i < node->adj_size(); ++ i ) {
	  MclqNode* node1 = node->adj_node(i);
	  if ( !node1->deleted() ) {
	    node1->dec_adj_num();
	    node_heap.update(node1);
	  }
	}
      }
    }
    // マークを消す．
    for ( int i = 0; i < best_node->adj_size(); ++ i ) {
      // best_node に隣接しているノードのマークを消す．
      MclqNode* node2 = best_node->adj_node(i);
      tmp_mark[node2->id()] = false;
    }
  }

  return node_set.size();
}

END_NAMESPACE_YM_UDGRAPH
