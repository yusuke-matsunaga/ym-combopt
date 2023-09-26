#ifndef EXACT_H
#define EXACT_H

/// @file Exact.h
/// @brief Exact のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Solver.h"
#include "mincov/McMatrix.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Exact Exact.h "Exact.h"
/// @brief 最小被覆問題の厳密解を求めるクラス
//////////////////////////////////////////////////////////////////////
class Exact :
  public Solver
{
public:

  /// @brief コンストラクタ
  Exact(
    McMatrix& matrix,        ///< [in] 問題の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );

  /// @brief デストラクタ
  ~Exact();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコスト
  SizeType
  solve(
    vector<SizeType>& solution ///< [out] 選ばれた列集合
  ) override;


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

  // 下界の計算クラス
  unique_ptr<LbCalc> mLbCalc;

  // 現在のベスト
  int mBest;

  // 現在のベスト解
  vector<SizeType> mBestSolution;

  // 現在の解
  vector<SizeType> mCurSolution;

  // block_partition を行うとき true にするフラグ
  bool mDoPartition;

};

END_NAMESPACE_YM_MINCOV

#endif // EXACT_H
