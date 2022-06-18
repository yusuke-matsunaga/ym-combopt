#ifndef EXACT_H
#define EXACT_H

/// @file Exact.h
/// @brief Exact のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "ym/McMatrix.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Exact Exact.h "Exact.h"
/// @brief 最小被覆問題の厳密解を求めるクラス
//////////////////////////////////////////////////////////////////////
class Exact
{
public:

  /// @brief コンストラクタ
  Exact(
    McMatrix& block,                ///< [in] 問題の行列
    const vector<LbCalc*>& lb_list, ///< [in] 下界の計算クラスのリスト
    Selector& selector		    ///< [in] 列を選択するクラス
  );

  /// @brief デストラクタ
  ~Exact();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコスト
  int
  solve(
    vector<SizeType>& solution ///< [out] 選ばれた列集合
  );

  /// @brief 対象の行列を返す．
  const McMatrix&
  matrix() const;

  /// @brief partition フラグを設定する．
  static
  void
  set_partition_flag(
    bool flag
  );

  /// @brief デバッグフラグを設定する．
  static
  void
  set_debug_flag(
    bool flag
  );

  /// @brief mMaxDepth を設定する．
  static
  void
  set_max_depth(
    int depth
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 解を求める再帰関数
  bool
  _solve(
    int lb,
    int depth
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下界の計算クラスのリスト
  const vector<LbCalc*>& mLbCalcList;

  // 列を選択するクラス
  Selector& mSelector;

  // 対象の行列
  McMatrix& mMatrix;

  // 現在のベスト
  int mBest;

  // 現在のベスト解
  vector<SizeType> mBestSolution;

  // 現在の解
  vector<SizeType> mCurSolution;

  // block_partition を行うとき true にするフラグ
  static
  bool mDoPartition;

  // デバッグフラグ
  static
  bool mDebug;

  // デバッグで表示する最大深さ
  static
  int mMaxDepth;

};

END_NAMESPACE_YM_MINCOV

#endif // EXACT_H
