#ifndef SOLVER_H
#define SOLVER_H

/// @file Solver.h
/// @brief Solver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "ym/json.h"
//#include "mincov/McMatrix.h"
//#include "mincov/LbCalc.h"
//#include "mincov/Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Solver Solver.h "Solver.h"
/// @brief 最小被覆問題の厳密解を求めるクラス
//////////////////////////////////////////////////////////////////////
class Solver
{
protected:

  /// @brief コンストラクタ
  Solver(
    McMatrix& matrix,        ///< [in] 問題の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );


public:
  //////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを解析してインスタンスを生成する関数
  static
  unique_ptr<Solver>
  new_obj(
    McMatrix& matrix,        ///< [in] 対象の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );

  /// @brief デストラクタ
  virtual
  ~Solver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコスト
  virtual
  SizeType
  solve(
    vector<SizeType>& solution ///< [out] 選ばれた列集合
  ) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象の行列を返す．
  McMatrix&
  matrix()
  {
    return mMatrix;
  }

  /// @brief 列を選択する．
  SizeType
  select();

  /// @brief デバッグレベルを得る．
  int
  debug() const
  {
    return mDebug;
  }

  /// @brief デバッグ用の最大深さを得る．
  int
  debug_depth() const
  {
    return mDebugDepth;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // オプション解析用の便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブール値を取り出す．
  static
  bool
  get_bool(
    const JsonValue& opt_obj, ///< [in] オプションを表す JSON オブジェクト
    const string& keyword,    ///< [in] キーワード文字列
    bool default_val = false  ///< [in] デフォルト値
  );

  /// @brief int値を取り出す．
  static
  int
  get_int(
    const JsonValue& opt_obj, ///< [in] オプションを表す JSON オブジェクト
    const string& keyword,    ///< [in] キーワード文字列
    int default_val = 0       ///< [in] デフォルト値
  );

  /// @brief 文字列値を取り出す．
  static
  string
  get_string(
    const JsonValue& opt_obj,            ///< [in] オプションを表す JSON オブジェクト
    const string& keyword,               ///< [in] キーワード文字列
    const string& default_val = string{} ///< [in] デフォルト値
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の行列
  McMatrix& mMatrix;

  // 下界の計算クラス
  unique_ptr<LbCalc> mLbCalc;

  // 列を選択するクラス
  unique_ptr<Selector> mSelector;

  // デバッグレベル
  int mDebug;

  // デバッグで表示する最大深さ
  int mDebugDepth;

};

END_NAMESPACE_YM_MINCOV

#endif // SOLVER_H
