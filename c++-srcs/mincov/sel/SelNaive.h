#ifndef SELNAIVE_H
#define SELNAIVE_H

/// @file SelNaive.h
/// @brief SelNaive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2023 Yusuke Matsunaga
/// All rights reserved.


#include "mincov/Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class SelNaive SelNaive.h "SelNaive.h"
/// @brief 単純な Selector
///
/// 最もカバーしている行の多い列を選ぶ
//////////////////////////////////////////////////////////////////////
class SelNaive :
  public Selector
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の列を選ぶ．
  /// @return 選ばれた列番号を返す．
  SizeType
  select(
    const McMatrix& matrix ///< [in] 対象の行列
  ) override;

};

END_NAMESPACE_YM_MINCOV


#endif // SELECTOR_H
