
/// @file LbMIS1.cc
/// @brief LbMIS1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LbMIS1.h"
#include "mincov/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

struct Node {
  SizeType mRowPos{0};
  bool mDeleted{false};
  vector<Node*> mAdjLink;
  SizeType mNum{0};
  Node* mNext{nullptr};

  //SizeType mHeapIdx{0};
};

struct Lt
{
  bool
  operator()(
    Node* left,
    Node* right
  )
  {
    return left->mNum < right->mNum;
  }
};


//////////////////////////////////////////////////////////////////////
// クラス LbMIS1
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
int
LbMIS1::calc(
  const McMatrix& matrix
)
{
  if ( matrix.active_row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限

  // 各行に対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクトリストを作る．
  SizeType rs = matrix.row_size();
  SizeType rn = matrix.active_row_num();
  vector<Node*> node_array(rs, nullptr);
  vector<Node> node_chunk(rn);
  Node* top = nullptr;
  Node* last = nullptr;
  SizeType idx = 0;
  for ( auto row_pos: matrix.row_head_list() ) {
    Node* node = &node_chunk[idx];
    ++ idx;
    node->mRowPos = row_pos;
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
  vector<bool> mark(rn, false);
  for ( auto row_pos: matrix.row_head_list() ) {
    // マークを用いて隣接関係を作る．
    Node* node1 = node_array[row_pos];
    for ( auto col_pos1: matrix.row_list(row_pos) ) {
      for ( auto row_pos2: matrix.col_list(col_pos1) ) {
	if ( !mark[row_pos2] ) {
	  mark[row_pos2] = true;
	  node1->mAdjLink.push_back(node_array[row_pos2]);
	}
      }
    }
    node1->mNum = node1->mAdjLink.size();
    // マークを消す．
    for ( auto node: node1->mAdjLink ) {
      mark[node->mRowPos] = false;
    }
  }

  // 未処理の Node のうち Node::mNum が最小のものを取り出し，解に加える．
  // 実際にはその行を被覆する列の最小コストを解に足す．
  int cost = 0;
  for ( ; ; ) {
    Node** pprev = &top;
    Node* best_node = nullptr;
    SizeType best_num = UINT_MAX;
    for ( ; ; ) {
      Node* node = *pprev;
      if ( node == nullptr ) {
	break;
      }
      if ( node->mDeleted ) {
	*pprev = node->mNext;
      }
      else {
	SizeType num = node->mNum;
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
    int min_cost = INT_MAX;
    for ( auto cpos: matrix.row_list(best_node->mRowPos) ) {
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
    cost += min_cost;

    // 処理済みの印をつける．
    best_node->mDeleted = true;
    // best_node に隣接しているノードも処理済みとする．
    for ( auto node2: best_node->mAdjLink ) {
      if ( !node2->mDeleted ) {
	node2->mDeleted = true;
	// さらにこのノードに隣接しているノードの mNum を減らす．
	for ( auto node3: node2->mAdjLink ) {
	  -- node3->mNum;
	}
      }
    }
  }

  return cost;
}

END_NAMESPACE_YM_MINCOV
