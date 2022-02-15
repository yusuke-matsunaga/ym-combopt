
/// @file LbMIS3.cc
/// @brief LbMIS3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "LbMIS3.h"
#include "ym/McMatrix.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMIS3
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
int
LbMIS3::operator()(
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
  int rs = matrix.row_size();
  int rn = matrix.active_row_num();

  UdGraph graph(rs);

  int* row_map = new int[rs];
  for ( int i = 0; i < rs; ++ i ) {
    row_map[i] = 0;
  }

  int idx = 0;
  for ( auto row_pos: matrix.row_head_list() ) {
    row_map[row_pos] = idx;
    ++ idx;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  vector<bool> mark(rn, false);
  for ( auto row_pos: matrix.row_head_list() ) {
    // マークを用いて隣接関係を作る．
    int id1 = row_map[row_pos];
    int row_list_idx = 0;
    for ( auto col_pos1: matrix.row_list(row_pos) ) {
      for ( auto row_pos2: matrix.col_list(col_pos1) ) {
	if ( !mark[row_pos2] ) {
	  mark[row_pos2] = true;
	  int id2 = row_map[row_pos2];
	  graph.connect(id1, id2);
	}
      }
    }
  }

  // 各行を被覆する列の最小コストを求める．
  for ( auto row_pos: matrix.row_head_list() ) {
    int id = row_map[row_pos];
    int min_cost = UINT_MAX;
    for ( auto cpos: matrix.row_list(row_pos) ) {
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
  }

  vector<int> node_set;
  max_clique(graph, node_set);
#warning "TODO: 未完成"
  int cost = 0;

  delete [] row_map;

  return cost;
}

END_NAMESPACE_YM_MINCOV
