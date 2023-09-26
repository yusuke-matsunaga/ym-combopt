
/// @file LbMAX.cc
/// @brief LbMAX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LbMAX.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMAX
//////////////////////////////////////////////////////////////////////

// @brief 下界の計算をする．
int
LbMAX::calc(
  const McMatrix& matrix
)
{
  int max_val = 0;
  for ( auto& calc: mChildList ) {
    int val = calc->calc(matrix);
    if ( max_val < val ) {
      max_val = val;
    }
  }
  return max_val;
}

END_NAMESPACE_YM_MINCOV
