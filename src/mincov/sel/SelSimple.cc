
/// @file SelSimple.cc
/// @brief SelSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelSimple.h"
#include "mincov/McBlock.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelSimple
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] block 対象の行列
// @return 選ばれた列番号を返す．
int
SelSimple::operator()(const McBlock& block)
{
  // 各行にカバーしている列数に応じた重みをつけ，
  // その重みの和が最大となる列を選ぶ．
  double max_weight = 0.0;
  int max_col = 0;
  for ( auto col_head: block.col_head_list() ) {
    double weight = 0.0;
    for ( auto cell: col_head->col_list() ) {
      double num = block.row_elem_num(cell->row_pos());
      weight += (1.0 / (num - 1.0));
    }
    weight /= block.col_cost(col_head->pos());

    if ( max_weight < weight ) {
      max_weight = weight;
      max_col = col_head->pos();
    }
  }
  return max_col;
}

END_NAMESPACE_YM_MINCOV
