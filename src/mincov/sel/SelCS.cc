
/// @file SelCS.cc
/// @brief SelCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelCS.h"
#include "ym/McBlock.h"


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
  for ( auto row_pos: block.row_head_list() ) {
    double min_cost = DBL_MAX;
    for ( auto col_pos: block.row_list(row_pos) ) {
      double col_cost = static_cast<double>(block.col_cost(col_pos)) / block.col_elem_num(col_pos);
      if ( min_cost > col_cost ) {
	min_cost = col_cost;
      }
    }
    row_weights[row_pos] = min_cost;
  }

  double min_delta = DBL_MAX;
  int min_col = 0;

  for ( auto col_pos: block.col_head_list() ) {
    double col_cost = block.col_cost(col_pos);

    vector<int> col_delta(block.col_size(), 0);
    vector<int> col_list;
    for ( auto row_pos: block.col_list(col_pos) ) {
      for ( auto col_pos1: block.row_list(row_pos) ) {
	if ( col_delta[col_pos1] == 0 ) {
	  col_list.push_back(col_pos1);
	}
	++ col_delta[col_pos1];
      }
    }

    vector<bool> row_mark(block.row_size(), false);
    vector<int> row_list;
    for ( auto col_pos1: col_list ) {
      double cost1 = block.col_cost(col_pos1);
      int num = block.col_elem_num(col_pos1);
      cost1 /= num;
      for ( auto row_pos: block.col_list(col_pos) ) {
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
      double min_weight = DBL_MAX;
      for ( auto col_pos1: block.row_list(row_pos) ) {
	double n = block.col_elem_num(col_pos1) - col_delta[col_pos1];
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
