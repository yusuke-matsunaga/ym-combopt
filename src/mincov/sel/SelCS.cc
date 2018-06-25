
/// @file SelCS.cc
/// @brief SelCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelCS.h"
#include "mincov/McBlock.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelCS
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] block 対象の行列
// @return 選ばれた列番号を返す．
int
SelCS::operator()(const McBlock& block)
{
  // 各行にカバーしている列数に応じた重みをつけ，
  // その重みの和が最大となる列を選ぶ．
  int nr = block.row_size();
  vector<double> row_weights(nr);
  for ( auto row_head: block.row_head_list() ) {
    int row_pos = row_head->pos();
    double min_cost = DBL_MAX;
    for ( auto cell: row_head->row_list() ) {
      int col_pos = cell->col_pos();
      auto col_head = block.col_head(col_pos);
      double col_cost = static_cast<double>(block.col_cost(col_pos)) / col_head->num();
      if ( min_cost > col_cost ) {
	min_cost = col_cost;
      }
    }
    row_weights[row_pos] = min_cost;
  }

  double min_delta = DBL_MAX;
  int min_col = 0;

  for ( auto col_head: block.col_head_list() ) {
    int col_pos = col_head->pos();
    double col_cost = block.col_cost(col_pos);

    vector<int> col_delta(block.col_size(), 0);
    vector<int> col_list;
    for ( auto cell: col_head->col_list() ) {
      int row_pos = cell->row_pos();
      auto row_head = block.row_head(row_pos);
      for ( auto cell1: row_head->row_list() ) {
	int col_pos = cell1->col_pos();
	if ( col_delta[col_pos] == 0 ) {
	  col_list.push_back(col_pos);
	}
	++ col_delta[col_pos];
      }
    }

    vector<bool> row_mark(block.row_size(), false);
    vector<int> row_list;
    for ( auto col_pos: col_list ) {
      auto col_head = block.col_head(col_pos);
      double cost1 = block.col_cost(col_pos);
      cost1 /= col_head->num();
      for ( auto cell: col_head->col_list() ) {
	int row_pos = cell->row_pos();
	if ( row_weights[row_pos] < cost1 ) {
	  continue;
	}
	if ( row_mark[row_pos] ) {
	  continue;
	}
	row_mark[row_pos] = true;
	row_list.push_back(row_pos);
      }
    }

    double delta_sum = 0.0;
    for ( auto row_pos: row_list ) {
      auto row_head = block.row_head(row_pos);
      double min_weight = DBL_MAX;
      for ( auto cell: row_head->row_list() ) {
	int col_pos1 = cell->col_pos();
	double n = block.col_head(col_pos1)->num() - col_delta[col_pos1];
	double cost1 = block.col_cost(col_pos1) / n;
	if ( min_weight > cost1 ) {
	  min_weight = cost1;
	}
      }
      double delta = min_weight - row_weights[row_pos];
      delta_sum += delta;
    }

    //cout << "Col#" << col->pos() << ": " << delta_sum << endl;
    if ( min_delta > delta_sum ) {
      min_delta = delta_sum;
      min_col = col_pos;
    }
  }
  return min_col;
}

END_NAMESPACE_YM_MINCOV
