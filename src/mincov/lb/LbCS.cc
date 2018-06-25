
/// @file LbCS.cc
/// @brief LbCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCS.h"
#include "mincov/McBlock.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LBCS
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] block 対象のブロック
// @return 下限値
int
LbCS::operator()(const McBlock& block)
{
  double cost = 0.0;
  for ( auto row_head: block.row_head_list() ) {
    double min_cost = DBL_MAX;
    for ( auto cell: row_head->row_list() ) {
      auto col_head = block.col_head(cell->col_pos());
      double col_cost = static_cast<double>(block.col_cost(col_head->pos())) / col_head->num();
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
