
/// @file LbMIS3.cc
/// @brief LbMIS3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS3.h"
#include "McMatrix.h"
#include "ym/UdGraph.h"
#include "ym/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMIS3
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
int
LbMIS3::operator()(const McMatrix& matrix)
{
  if ( matrix.row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限
  // 各行に対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクとリストを作る．
  SimpleAlloc alloc;
  int rs = matrix.row_size();
  int rn = matrix.row_num();

  UdGraph graph(rs);

  int* row_map = alloc.get_array<int>(rs);
  for ( int i = 0; i < rs; ++ i ) {
    row_map[i] = 0;
  }

  int idx = 0;
  for ( auto row1: matrix.row_list() ) {
    int row_pos = row1->pos();
    row_map[row_pos] = idx;
    ++ idx;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  for ( auto row1: matrix.row_list() ) {
    // マークを消す．
    // 結構めんどくさいけど効率はいい
    for ( auto cell1 = row1->row_front();
	  !row1->is_end(cell1); cell1 = cell1->row_next() ) {
      auto col1 = matrix.col(cell1->col_pos());
      for ( auto cell2 = col1->front();
	    !col1->is_end(cell2); cell2 = cell2->col_next() ) {
	int row_pos = cell2->row_pos();
	matrix.row(row_pos)->mWork = 0;
      }
    }
    // マークを用いて隣接関係を作る．
    int row_pos1 = row1->pos();
    int id1 = row_map[row_pos1];
    int row_list_idx = 0;
    for ( auto cell1 = row1->row_front();
	  !row1->is_end(cell1); cell1 = cell1->row_next() ) {
      auto col1 = matrix.col(cell1->col_pos());
      for ( auto cell2 = col1->front();
	    !col1->is_end(cell2); cell2 = cell2->col_next() ) {
	int row_pos2 = cell2->row_pos();
	if ( matrix.row(row_pos2)->mWork == 0 ) {
	  matrix.row(row_pos2)->mWork = 1;
	  int id2 = row_map[row_pos2];
	  graph.connect(id1, id2);
	}
      }
    }
  }

  // 各行を被覆する列の最小コストを求める．
  for ( auto row1: matrix.row_list() ) {
    int row_pos = row1->pos();
    int id = row_map[row_pos];
    int min_cost = UINT_MAX;
    for ( auto cell = row1->row_front();
	  !row1->is_end(cell); cell = cell->row_next() ) {
      int cpos = cell->col_pos();
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
  }

  vector<int> node_set;
  max_clique(graph, node_set);
#warning "TODO: 未完成"
  int cost = 0;

  return cost;
}

END_NAMESPACE_YM_MINCOV
