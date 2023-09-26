#ifndef GREEDY_H
#define GREEDY_H

/// @file Greedy.h
/// @brief Greedy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
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

};

END_NAMESPACE_YM_MINCOV

#endif // GREEDY_H
