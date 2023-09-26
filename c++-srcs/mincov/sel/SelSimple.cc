
/// @file SelSimple.cc
/// @brief SelSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SelSimple.h"
#include "mincov/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelSimple
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
SizeType
SelSimple::select(
  const McMatrix& matrix
)
{
  // 各行にカバーしている列数に応じた重みをつけ，
  // その重みの和が最大となる列を選ぶ．
  double max_weight = 0.0;
  SizeType max_col = 0;
  for ( auto col_pos: matrix.col_head_list() ) {
    double weight = 0.0;
    for ( auto row_pos: matrix.col_list(col_pos) ) {
      double num = matrix.row_elem_num(row_pos);
      weight += (1.0 / (num - 1.0));
    }
    weight /= matrix.col_cost(col_pos);

    if ( max_weight < weight ) {
      max_weight = weight;
      max_col = col_pos;
    }
  }
  return max_col;
}

END_NAMESPACE_YM_MINCOV
