﻿
/// @file LbMIS1.cc
/// @brief LbMIS1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS1.h"
#include "ym/McBlock.h"
#include "ym/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

struct Node {

  void
  set(int pos);

  int mRowPos;
  bool mDeleted;
  Node** mAdjLink;
  int mAdjNum;
  int mNum;
  Node* mNext;

  int mHeapIdx;
};

void
Node::set(int pos)
{
  mRowPos = pos;
  mDeleted = false;
  mNum = 0;
  mNext = nullptr;
}

struct Lt
{
  bool
  operator()(Node* left,
	     Node* right)
  {
    return left->mNum < right->mNum;
  }
};


//////////////////////////////////////////////////////////////////////
// クラス LbMIS1
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
int
LbMIS1::operator()(const McBlock& block)
{
  if ( block.row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限

  // 各行に対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクトリストを作る．
  SimpleAlloc alloc;
  int rs = block.row_size();
  int rn = block.row_num();
  void* p = alloc.get_memory(sizeof(Node*) * rs);
  Node** node_array = new (p) Node*[rs];
  for ( int i = 0; i < rs; ++ i ) {
    node_array[i] = nullptr;
  }
  void* q = alloc.get_memory(sizeof(Node) * rn);
  Node* node_chunk = new (q) Node[rn];
  Node* top = nullptr;
  Node* last = nullptr;
  int idx = 0;
  for ( auto row_pos: block.row_head_list() ) {
    Node* node = &node_chunk[idx];
    ++ idx;
    node->set(row_pos);
    node_array[row_pos] = node;
    if ( last == nullptr ) {
      ASSERT_COND( top == nullptr );
      top = node;
    }
    else {
      last->mNext = node;
    }
    last = node;
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
    Node* node1 = node_array[row_pos];
    void* p = alloc.get_memory(sizeof(Node*) * row_list_idx);
    node1->mAdjLink = new (p) Node*[row_list_idx];
    for ( int i = 0; i < row_list_idx; ++ i ) {
      Node* node2 = node_array[row_list[i]];
      node1->mAdjLink[i] = node2;
    }
    node1->mAdjNum = row_list_idx;
    node1->mNum = row_list_idx;
  }

  // 未処理の Node のうち Node::mNum が最小のものを取り出し，解に加える．
  // 実際にはその行を被覆する列の最小コストを解に足す．
  int cost = 0;
  for ( ; ; ) {
    Node** pprev = &top;
    Node* best_node = nullptr;
    int best_num = UINT_MAX;
    for ( ; ; ) {
      Node* node = *pprev;
      if ( node == nullptr ) {
	break;
      }
      if ( node->mDeleted ) {
	*pprev = node->mNext;
      }
      else {
	int num = node->mNum;
	if ( best_num > num ) {
	  best_num = num;
	  best_node = node;
	}
	pprev = &node->mNext;
      }
    }
    if ( best_node == nullptr ) {
      break;
    }

    // best_node に対応する行を被覆する列の最小コストを求める．
    int min_cost = UINT_MAX;
    for ( auto cpos: block.row_list(best_node->mRowPos) ) {
      if ( min_cost > block.col_cost(cpos) ) {
	min_cost = block.col_cost(cpos);
      }
    }
    cost += min_cost;

    // 処理済みの印をつける．
    best_node->mDeleted = true;
    for ( int i = 0; i < best_node->mAdjNum; ++ i ) {
      // best_node に隣接しているノードも処理済みとする．
      Node* node2 = best_node->mAdjLink[i];
      if ( !node2->mDeleted ) {
	node2->mDeleted = true;
	// さらにこのノードに隣接しているノードの mNum を減らす．
	for ( int j = 0; j < node2->mAdjNum; ++ j ) {
	  Node* node3 = node2->mAdjLink[j];
	  -- node3->mNum;
	}
      }
    }
  }

  return cost;
}

END_NAMESPACE_YM_MINCOV
