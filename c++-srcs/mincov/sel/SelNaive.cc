
/// @file SelNaive.cc
/// @brief SelNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SelNaive.h"
#include "ym/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelNaive
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] block 対象の行列
// @return 選ばれた列番号を返す．
int
SelNaive::operator()(const McMatrix& matrix)
{
  // 最もカバーしている行の多い列を選ぶ
  int max_num = 0;
  int max_col = 0;
  for ( auto col_pos: matrix.col_head_list() ) {
    if ( max_num < matrix.col_elem_num(col_pos) ) {
      max_num = matrix.col_elem_num(col_pos);
      max_col = col_pos;
    }
  }
  return max_col;
}

END_NAMESPACE_YM_MINCOV
