#ifndef MCLQSOLVER_H
#define MCLQSOLVER_H

/// @file MclqSolver.h
/// @brief MclqSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "ym/SimpleAlloc.h"


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
  /// @param[in] graph 対象のグラフ
  MclqSolver(const UdGraph& graph);

  /// @brief デストラクタ
  ~MclqSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief greedy ヒューリスティックで解を求める．
  int
  greedy(vector<int>& node_set);

  /// @brief 分枝限定法を用いた厳密解を求める．
  int
  exact(vector<int>& node_set);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // ノード数
  int mNodeNum;

  // ノードの配列
  MclqNode* mNodeArray;

};

END_NAMESPACE_YM_UDGRAPH

#endif // MCLQSOLVER_H
