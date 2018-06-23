
/// @file LbCS.cc
/// @brief LbCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCS.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LBCS
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
int
LbCS::operator()(const McMatrix& matrix)
{
  double cost = 0.0;
  for ( auto row: matrix.row_list() ) {
    double min_cost = DBL_MAX;
    for ( auto cell = row->row_front();
	  !row->is_end(cell); cell = cell->row_next() ) {
      auto col = matrix.col(cell->col_pos());
      double col_cost = static_cast<double>(matrix.col_cost(col->pos())) / col->num();
      if ( min_cost > col_cost ) {
	min_cost = col_cost;
      }
    }
    cost += min_cost;
  }
  cost = ceil(cost);

  return static_cast<int>(cost);
}

END_NAMESPACE_YM_MINCOV
