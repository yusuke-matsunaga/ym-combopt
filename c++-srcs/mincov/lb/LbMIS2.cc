
/// @file LbMIS2.cc
/// @brief LbMIS2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LbMIS2.h"
#include "ym/McMatrix.h"
#include "MisNode.h"
#include "MisNodeHeap.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMIS2
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
int
LbMIS2::operator()(
  const McMatrix& matrix
)
{
  if ( matrix.active_row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限
  // 各行に対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクとリストを作る．
  SizeType rs = matrix.row_size();
  SizeType rn = matrix.active_row_num();

  MisNodeHeap node_heap(rs);

  vector<MisNode*> node_array(rn, nullptr);

  SizeType idx = 0;
  for ( auto row_pos: matrix.row_head_list() ) {
    MisNode* node = node_heap.node(idx);
    ++ idx;
    node->set_row_pos(row_pos);
    node_array[row_pos] = node;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  vector<bool> mark(rn, false);
  for ( auto row_pos: matrix.row_head_list() ) {
    // マークを用いて隣接関係を作る．
    vector<MisNode*> adj_link;
    for ( auto col_pos1: matrix.row_list(row_pos) ) {
      for ( auto row_pos2: matrix.col_list(col_pos1) ) {
	if ( !mark[row_pos2] ) {
	  mark[row_pos2] = true;
	  adj_link.push_back(node_array[row_pos2]);
	}
      }
    }
    // マークを消す．
    for ( auto node: adj_link ) {
      mark[node->row_pos()] = false;
    }
    MisNode* node1 = node_array[row_pos];
    node1->set_adj_link(std::move(adj_link));
    node_heap.put(node1);
  }

  // 未処理の MisNode のうち Node::mNum が最小のものを取り出し，解に加える．
  // 実際にはその行を被覆する列の最小コストを解に足す．
  int cost = 0;
  while ( !node_heap.empty() ) {
    MisNode* best_node = node_heap.get_min();

    // best_node に対応する行を被覆する列の最小コストを求める．
    int min_cost = INT_MAX;
    for ( auto cpos: matrix.row_list(best_node->row_pos()) ) {
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
    cost += min_cost;

    for ( SizeType i = 0; i < best_node->adj_size(); ++ i ) {
      // best_node に隣接しているノードも処理済みとする．
      MisNode* node2 = best_node->adj_node(i);
      if ( !node2->deleted() ) {
	node_heap.delete_node(node2);
	// さらにこのノードに隣接しているノードの mNum を減らす．
	for ( SizeType j = 0; j < node2->adj_size(); ++ j ) {
	  MisNode* node3 = node2->adj_node(j);
	  if ( !node3->deleted() ) {
	    node3->dec_adj_num();
	    node_heap.update(node3);
	  }
	}
      }
    }
  }

  return cost;
}

END_NAMESPACE_YM_MINCOV
