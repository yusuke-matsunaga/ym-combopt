
/// @file SelNaive.cc
/// @brief SelNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelNaive.h"
#include "mincov/McBlock.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelNaive
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] block 対象の行列
// @return 選ばれた列番号を返す．
int
SelNaive::operator()(const McBlock& block)
{
  // 最もカバーしている行の多い列を選ぶ
  int max_num = 0;
  int max_col = 0;
  for ( auto col_head: block.col_head_list() ) {
    if ( max_num < col_head->num() ) {
      max_num = col_head->num();
      max_col = col_head->pos();
    }
  }
  return max_col;
}

END_NAMESPACE_YM_MINCOV
