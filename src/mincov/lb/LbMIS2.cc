
/// @file LbMIS2.cc
/// @brief LbMIS2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS2.h"
#include "ym/McBlock.h"
#include "MisNode.h"
#include "MisNodeHeap.h"
#include "ym/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMIS2
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] block 対象の行列
// @return 下限値
int
LbMIS2::operator()(const McBlock& block)
{
  if ( block.row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限
  // 各行に対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクとリストを作る．
  SimpleAlloc alloc;
  int rs = block.row_size();
  int rn = block.row_num();

  MisNodeHeap node_heap(rs);

  MisNode** node_array = alloc.get_array<MisNode*>(rs);
  for ( int i = 0; i < rs; ++ i ) {
    node_array[i] = nullptr;
  }

  int idx = 0;
  for ( auto row_pos: block.row_head_list() ) {
    MisNode* node = node_heap.node(idx);
    ++ idx;
    node->set(row_pos);
    node_array[row_pos] = node;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  int* row_list = alloc.get_array<int>(rn);
  vector<bool> mark(rn, false);
  for ( auto row_pos: block.row_head_list() ) {
    // マークを用いて隣接関係を作る．
    int row_list_idx = 0;
    for ( auto col_pos1: block.row_list(row_pos) ) {
      for ( auto row_pos2: block.col_list(col_pos1) ) {
	if ( !mark[row_pos2] ) {
	  mark[row_pos2] = true;
	  row_list[row_list_idx] = row_pos2;
	  ++ row_list_idx;
	}
      }
    }
    MisNode* node1 = node_array[row_pos];
    MisNode** adj_link = alloc.get_array<MisNode*>(row_list_idx);
    for ( int i = 0; i < row_list_idx; ++ i ) {
      MisNode* node2 = node_array[row_list[i]];
      adj_link[i] = node2;
    }
    node1->set_adj_link(row_list_idx, adj_link);

    node_heap.put(node1);
  }

  // 未処理の MisNode のうち Node::mNum が最小のものを取り出し，解に加える．
  // 実際にはその行を被覆する列の最小コストを解に足す．
  int cost = 0;
  while ( !node_heap.empty() ) {
    MisNode* best_node = node_heap.get_min();

    // best_node に対応する行を被覆する列の最小コストを求める．
    int min_cost = UINT_MAX;
    for ( auto cpos: block.row_list(best_node->row_pos()) ) {
      if ( min_cost > block.col_cost(cpos) ) {
	min_cost = block.col_cost(cpos);
      }
    }
    cost += min_cost;

    for ( int i = 0; i < best_node->adj_size(); ++ i ) {
      // best_node に隣接しているノードも処理済みとする．
      MisNode* node2 = best_node->adj_node(i);
      if ( !node2->deleted() ) {
	node_heap.delete_node(node2);
	// さらにこのノードに隣接しているノードの mNum を減らす．
	for ( int j = 0; j < node2->adj_size(); ++ j ) {
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
