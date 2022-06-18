#ifndef MAXCLIQUE_H
#define MAXCLIQUE_H

/// @file MaxClique.h
/// @brief MaxClique のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MaxClique MaxClique.h "MaxClique.h"
/// @brief 最大クリーク問題を解くクラス
//////////////////////////////////////////////////////////////////////
class MaxClique
{
public:

  /// @brief コンストラクタ
  MaxClique(
    SizeType size ///< [in] ノード数
  ) : mCostArray(size),
      mNlistArray(size)
  {
  }

  /// @brief デストラクタ
  ~MaxClique() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コストを設定する．
  void
  set_cost(
    SizeType id,
    double cost
  )
  {
    mCostArray[id] = cost;
  }

  /// @brief 2つのノードを隣接させる．
  ///< [in] id1, id2 ノード番号
  void
  connect(
    SizeType id1,
    SizeType id2
  )
  {
    mNlistArray[id1].push_back(id2);
    mNlistArray[id2].push_back(id1);
  }

  /// @brief 最大クリークを求める．
  /// @return 重みの和を返す．
  double
  solve(
    vector<SizeType>& ans ///< [out] 解のノード番号を入れる配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コストの配列
  vector<double> mCostArray;

  // 隣接ノードリストの配列
  vector<vector<SizeType> > mNlistArray;

};

END_NAMESPACE_YM_MINCOV

#endif // MAXCLIQUE_H
