#ifndef MAXCLIQUE_H
#define MAXCLIQUE_H

/// @file MaxClique.h
/// @brief MaxClique のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ym_mincov.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MaxClique MaxClique.h "MaxClique.h"
/// @brief 最大クリーク問題を解くクラス
//////////////////////////////////////////////////////////////////////
class MaxClique
{
public:

  /// @brief コンストラクタ
  /// @param[in] size ノード数
  MaxClique(int size);

  /// @brief デストラクタ
  ~MaxClique();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コストを設定する．
  void
  set_cost(int id,
	   double cost);

  /// @brief 2つのノードを隣接させる．
  /// @param[in] id1, id2 ノード番号
  void
  connect(int id1,
	  int id2);

  /// @brief 最大クリークを求める．
  /// @param[out] ans 解のノード番号を入れる配列
  double
  solve(vector<int>& ans);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コストの配列
  vector<double> mCostArray;

  // 隣接ノードリストの配列
  vector<vector<int> > mNlistArray;

};

END_NAMESPACE_YM_MINCOV

#endif // MAXCLIQUE_H
