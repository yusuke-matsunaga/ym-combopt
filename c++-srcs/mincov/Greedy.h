#ifndef GREEDY_H
#define GREEDY_H

/// @file Greedy.h
/// @brief Greedy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Greedy Greedy.h "Greedy.h"
/// @brief Minimum Covering 問題を greedy に解くクラス
///
/// 実はクラスである必要はほとんどない．
//////////////////////////////////////////////////////////////////////
class Greedy
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  static
  void
  solve(
    McMatrix& matrix,          ///< [in] 対象の行列
    Selector& selector,	       ///< [in] 列を選ぶ関数オブジェクト
    vector<SizeType>& solution ///< [out] 選ばれた列集合
  );

  /// @brief デバッグフラグをセットする．
  static
  void
  set_debug_flag(
    bool flag ///< [in] セットする値
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // デバッグフラグ
  static
  bool mDebug;

};

END_NAMESPACE_YM_MINCOV

#endif // GREEDY_H
