#ifndef MCLQSOLVER_H
#define MCLQSOLVER_H

/// @file MclqSolver.h
/// @brief MclqSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/udgraph_nsdef.h"


BEGIN_NAMESPACE_YM_UDGRAPH

class MclqNode;

//////////////////////////////////////////////////////////////////////
/// @class MclqSolver MclqSolver.h "MclqSolver.h"
/// @brief max clique を解くためのクラス
//////////////////////////////////////////////////////////////////////
class MclqSolver
{
public:

  /// @brief コンストラクタ
  MclqSolver(
    const UdGraph& graph ///< [in] 対象のグラフ
  );

  /// @brief デストラクタ
  ~MclqSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief greedy ヒューリスティックで解を求める．
  vector<SizeType>
  greedy();

  /// @brief 分枝限定法を用いた厳密解を求める．
  vector<SizeType>
  exact();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  SizeType mNodeNum;

  // ノードの配列
  MclqNode* mNodeArray;

};

END_NAMESPACE_YM_UDGRAPH

#endif // MCLQSOLVER_H
