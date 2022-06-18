#ifndef LBMAX_H
#define LBMAX_H

/// @file LbMax.h
/// @brief LbMax のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LbCalc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbMAX LbMAX.h "LbMAX.h"
/// @brief 複数の LbCalc の MAX を計算するクラス
//////////////////////////////////////////////////////////////////////
class LbMAX :
  public LbCalc
{
public:

  /// @brief コンストラクタ
  LbMAX() = default;

  /// @brief デストラクタ
  ~LbMAX();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする．
  int
  operator()(
    const McMatrix& matrix
  ) override;

  /// @brief 下界の計算クラスを追加する．
  void
  add_calc(
    LbCalc* calc
  )
  {
    mCalcList.push_back(unique_ptr<LbCal>{calc});
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下界の計算クラスのリスト
  vector<unique_ptr<LbCalc>> mCalcList;

};

END_NAMESPACE_YM_MINCOV

#endif // LBMAX_H
