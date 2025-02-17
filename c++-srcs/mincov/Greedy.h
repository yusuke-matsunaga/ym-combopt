#ifndef GREEDY_H
#define GREEDY_H

/// @file Greedy.h
/// @brief Greedy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Solver.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Greedy Greedy.h "Greedy.h"
/// @brief Minimum Covering 問題を greedy に解くクラス
//////////////////////////////////////////////////////////////////////
class Greedy :
  public Solver
{
public:

  /// @brief コンストラクタ
  Greedy(
    McMatrix& matrix,        ///< [in] 対象の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  SizeType
  solve(
    vector<SizeType>& solution ///< [out] 選ばれた列集合
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 列を選択するクラス
  std::unique_ptr<Selector> mSelector;

};

END_NAMESPACE_YM_MINCOV

#endif // GREEDY_H
