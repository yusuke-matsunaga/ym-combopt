
/// @file SelCS.cc
/// @brief SelCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelCS.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelCS
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] matrix 対象の行列
// @return 選ばれた列番号を返す．
int
SelCS::operator()(const McMatrix& matrix)
{
  // 各行にカバーしている列数に応じた重みをつけ，
  // その重みの和が最大となる列を選ぶ．
  int nr = matrix.row_size();
  vector<double> row_weights(nr);
  for ( auto row: matrix.row_list() ) {
    int row_pos = row->pos();
    double min_cost = DBL_MAX;
    for ( auto cell: row->row_list() ) {
      auto col = matrix.col(cell->col_pos());
      double col_cost = static_cast<double>(matrix.col_cost(col->pos())) / col->num();
      if ( min_cost > col_cost ) {
	min_cost = col_cost;
      }
    }
    row_weights[row_pos] = min_cost;
  }

  double min_delta = DBL_MAX;
  int min_col = 0;

  for ( auto col: matrix.col_list() ) {
    int col_pos = col->pos();
    double col_cost = matrix.col_cost(col_pos);

    vector<int> col_delta(matrix.col_size(), 0);
    vector<int> col_list;
    for ( auto cell: col->col_list() ) {
      int row_pos = cell->row_pos();
      auto row = matrix.row(row_pos);
      for ( auto cell1: row->row_list() ) {
	int col_pos = cell1->col_pos();
	if ( col_delta[col_pos] == 0 ) {
	  col_list.push_back(col_pos);
	}
	++ col_delta[col_pos];
      }
    }

    vector<bool> row_mark(matrix.row_size(), false);
    vector<int> row_list;
    for ( auto col_pos: col_list ) {
      auto col1 = matrix.col(col_pos);
      double cost1 = matrix.col_cost(col_pos);
      cost1 /= col1->num();
      for ( auto cell: col1->col_list() ) {
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
      auto row = matrix.row(row_pos);
      double min_weight = DBL_MAX;
      for ( auto cell: row->row_list() ) {
	int col_pos1 = cell->col_pos();
	double n = matrix.col(col_pos1)->num() - col_delta[col_pos1];
	double cost1 = matrix.col_cost(col_pos1) / n;
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
