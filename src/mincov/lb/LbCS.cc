
/// @file LbCS.cc
/// @brief LbCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCS.h"
#include "ym/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LBCS
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] block 対象のブロック
// @return 下限値
int
LbCS::operator()(const McMatrix& matrix)
{
  double cost = 0.0;
  for ( auto row_pos: matrix.row_head_list() ) {
    double min_cost = DBL_MAX;
    for ( auto col_pos: matrix.row_list(row_pos) ) {
      double cost = matrix.col_cost(col_pos);
      double col_num = matrix.col_elem_num(col_pos);
      double col_cost = cost / col_num;
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
