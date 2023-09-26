#ifndef LBCALC_H
#define LBCALC_H

/// @file LbCalc.h
/// @brief LbCalc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "ym/json.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbCalc LbCalc.h "LbCalc.h"
/// @brief 下界の計算クラス
//////////////////////////////////////////////////////////////////////
class LbCalc
{
public:

  /// @brief インスタンスを生成するクラスメソッド
  static
  unique_ptr<LbCalc>
  new_obj(
    const JsonValue& opt_obj ///< [in] オプションを表すJSONオブジェクト
  );

  /// @brief デストラクタ
  virtual
  ~LbCalc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする
  virtual
  int
  calc(
    const McMatrix& matrix ///< [in] 対象の行列
  ) = 0;

};

END_NAMESPACE_YM_MINCOV


#endif // LBMIS_H
