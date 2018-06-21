#ifndef TABUCOL_H
#define TABUCOL_H

/// @file TabuCol.h
/// @brief TabuCol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "ColGraph.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class TabuCol TabuCol.h "TabuCol.h"
/// @brief tabu list を用いた彩色アルゴリズム
//////////////////////////////////////////////////////////////////////
class TabuCol :
  public ColGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  /// @param[in] k 彩色数
  TabuCol(const UdGraph& graph,
	  int k);

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  /// @param[in] color_map 部分的な彩色結果
  /// @param[in] k 彩色数
  TabuCol(const UdGraph& graph,
	  const vector<int>& color_map,
	  int k);

  /// @brief デストラクタ
  ~TabuCol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色が可能か調べる．
  /// @param[in] iter_limit 最大の繰り返し回数
  /// @param[in] L タブー期間の基本パラメータ
  /// @param[in] alpha タブ期間の節点数依存パラメータ
  /// @param[out] color_map 彩色結果を入れる配列
  /// @retval true 彩色できた．
  /// @retval false 彩色できなかった．
  ///
  /// color_map[i] には i 番目の節点の色が入る．
  bool
  coloring(int iter_limit,
	   int L,
	   double alpha,
	   vector<int>& color_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部データ構造の初期化を行う．
  void
  init();

  /// @brief 初期解を作る．
  void
  gen_random_solution();

  /// @brief γ(node_id, col) が最小となる move を得る．
  pair<int, int>
  get_move();

  /// @brief γ(node_id, col) を返す．
  /// @param[in] node_id 節点番号
  /// @param[in] col 色番号 ( 1 <= col <= mK )
  int
  gamma(int node_id,
	int col) const;

  /// @brief タブーリストに追加する．
  /// @param[in] node_id 節点番号
  /// @param[in] col 色番号
  /// @param[in] tenure 期間
  void
  add_tabu(int node_id,
	   int col,
	   int tenure);

  /// @brief conflict vertices の個数を数える．
  int
  conflict_num() const;

  /// @brief タブーリストに入っていないかチェックする．
  /// @param[in] node_id 節点番号
  /// @param[in] col 色番号
  /// @retval true 禁止されていなかった．
  /// @retval false 禁止されていた．
  bool
  check_tabu(int node_id,
	     int col) const;

  /// @brief 節点番号と色番号からインデックスを作る．
  int
  encode(int node_id,
	 int col) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 彩色数
  int mK;

  // γテーブル
  // サイズは node_num() * mK
  int* mGammaTable;

  // tabu list を表す配列
  // サイズは node_num() * mK
  int* mTabuMatrix;

  // 現在の繰り返し回数
  int mIter;

  // 乱数発生器
  RandGen mRandGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief γ(node_id, col) を返す．
// @param[in] node_id 節点番号
// @param[in] col 色番号 ( 1 <= col <= mK )
inline
int
TabuCol::gamma(int node_id,
	       int col) const
{
  return mGammaTable[encode(node_id, col)];
}

// @brief タブーリストに追加する．
// @param[in] node_id 節点番号
// @param[in] col 色番号
// @param[in] tenure 期間
inline
void
TabuCol::add_tabu(int node_id,
		  int col,
		  int tenure)
{
  mTabuMatrix[encode(node_id, col)] = mIter + tenure;
}

// @brief タブーリストに入っていないかチェックする．
// @param[in] node_id 節点番号
// @param[in] col 色番号
// @retval true 禁止されていなかった．
// @retval false 禁止されていた．
inline
bool
TabuCol::check_tabu(int node_id,
		    int col) const
{
  return mTabuMatrix[encode(node_id, col)] <= mIter;
}

// @brief 節点と色番号からインデックスを作る．
inline
int
TabuCol::encode(int node_id,
		int col) const
{
  ASSERT_COND( node_id >= 0 && node_id < node_num() );
  ASSERT_COND( col >= 1 && col <= mK );

  return node_id * mK + (col - 1);
}

END_NAMESPACE_YM_UDGRAPH

#endif // TABUCOL_H
