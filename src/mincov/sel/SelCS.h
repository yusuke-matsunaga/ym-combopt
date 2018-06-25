﻿#ifndef SELCS_H
#define SELCS_H

/// @file SelCS.h
/// @brief SelCS のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov/Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class SelCS SelCS.h "SelCS.h"
/// @brief 列を選ぶファンクタ
///
/// 各行にカバーしている列数に応じた重みをつけ，
/// その重みの和が最大となる列を選ぶ．
//////////////////////////////////////////////////////////////////////
class SelCS :
  public Selector
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の列を選ぶ．
  /// @param[in] block 対象の行列
  /// @return 選ばれた列番号を返す．
  int
  operator()(const McBlock& block) override;

};

END_NAMESPACE_YM_MINCOV


#endif // SELCS_H
