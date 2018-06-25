#ifndef LBCALC_H
#define LBCALC_H

/// @file LbCalc.h
/// @brief LbCalc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbCalc LbCalc.h "LbCalc.h"
/// @brief 下界の計算クラス
//////////////////////////////////////////////////////////////////////
class LbCalc
{
public:

  /// @brief デストラクタ
  virtual
  ~LbCalc() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする
  /// @param[in] block 対象のブロック
  virtual
  int
  operator()(const McBlock& block) = 0;

};

END_NAMESPACE_YM_MINCOV


#endif // LBMIS_H
