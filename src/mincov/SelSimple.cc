
/// @file SelSimple.cc
/// @brief SelSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelSimple.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelSimple
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] matrix 対象の行列
// @return 選ばれた列番号を返す．
int
SelSimple::operator()(const McMatrix& matrix)
{
  // 各行にカバーしている列数に応じた重みをつけ，
  // その重みの和が最大となる列を選ぶ．
  double max_weight = 0.0;
  int max_col = 0;
  for ( auto col: matrix.col_list() ) {
    double weight = 0.0;
    for ( auto cell = col->col_front();
	  !col->is_end(cell); cell = cell->col_next() ) {
      auto row = matrix.row(cell->row_pos());
      weight += (1.0 / (row->num() - 1.0));
    }
    weight /= matrix.col_cost(col->pos());

    if ( max_weight < weight ) {
      max_weight = weight;
      max_col = col->pos();
    }
  }
  return max_col;
}

END_NAMESPACE_YM_MINCOV
