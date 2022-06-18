#ifndef LBMIS1_H
#define LBMIS1_H

/// @file LbMIS1.h
/// @brief LbMIS1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/LbCalc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbMIS1 LbMIS1.h "LbMIS1.h"
/// @brief MIS(maximum independent set) を用いた下界の計算クラス
///
/// ノードをリンクトリストでつなぐ実装
//////////////////////////////////////////////////////////////////////
class LbMIS1 :
  public LbCalc
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする
  int
  operator()(
    const McMatrix& matrix
  ) override;

};

END_NAMESPACE_YM_MINCOV

#endif // LBMIS_H
