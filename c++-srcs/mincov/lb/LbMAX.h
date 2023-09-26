#ifndef LBMAX_H
#define LBMAX_H

/// @file LbMax.h
/// @brief LbMax のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/LbCalc.h"


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
  LbMAX(
    vector<unique_ptr<LbCalc>>&& child_list
  ) : mChildList{std::move(child_list)}
  {
  }

  /// @brief デストラクタ
  ~LbMAX() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする．
  int
  calc(
    const McMatrix& matrix
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下界の計算クラスのリスト
  vector<unique_ptr<LbCalc>> mChildList;

};

END_NAMESPACE_YM_MINCOV

#endif // LBMAX_H
