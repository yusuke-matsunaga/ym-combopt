#ifndef TABUCOL_H
#define TABUCOL_H

/// @file TabuCol.h
/// @brief TabuCol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/udgraph_nsdef.h"
#include "coloring/ColGraph.h"
#include <random>


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
  TabuCol(
    const UdGraph& graph, ///< graph 対象のグラフ
    SizeType k		  ///< k 彩色数
  );

  /// @brief コンストラクタ
  TabuCol(
    const UdGraph& graph,              ///< [in] 対象のグラフ
    const vector<SizeType>& color_map, ///< [in] 部分的な彩色結果
    SizeType k			       ///< [in] 彩色数
  );

  /// @brief デストラクタ
  ~TabuCol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色が可能か調べる．
  /// @retval true 彩色できた．
  /// @retval false 彩色できなかった．
  ///
  /// color_map[i] には i 番目の節点の色が入る．
  bool
  coloring(
    SizeType iter_limit,        ///< [in] 最大の繰り返し回数
    SizeType L,			///< [in] タブー期間の基本パラメータ
    double alpha,		///< [in] タブー期間の節点数依存パラメータ
    vector<SizeType>& color_map	///< [out] color_map 彩色結果を入れる配列
  );


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
  pair<SizeType, SizeType>
  get_move();

  /// @brief γ(node_id, col) を返す．
  int
  gamma(
    SizeType node_id, ///< [in] 節点番号
    SizeType col      ///< [in] 色番号 ( 1 <= col <= mK )
  ) const
  {
    return mGammaTable[encode(node_id, col)];
  }

  /// @brief タブーリストに追加する．
  void
  add_tabu(
    SizeType node_id, ///< [in] 節点番号
    SizeType col,     ///< [in] 色番号
    SizeType tenure   ///< [in] 期間
  )
  {
    mTabuMatrix[encode(node_id, col)] = mIter + tenure;
  }

  /// @brief conflict vertices の個数を数える．
  SizeType
  conflict_num() const;

  /// @brief タブーリストに入っていないかチェックする．
  /// @retval true 禁止されていなかった．
  /// @retval false 禁止されていた．
  bool
  check_tabu(
    SizeType node_id, ///< [in] 節点番号
    SizeType col      ///< [in] 色番号
  ) const
  {
    return mTabuMatrix[encode(node_id, col)] <= mIter;
  }

  /// @brief 節点番号と色番号からインデックスを作る．
  SizeType
  encode(
    SizeType node_id, ///< [in] 節点番号
    SizeType col      ///< [in] 色番号
  ) const
  {
    ASSERT_COND( node_id >= 0 && node_id < node_num() );
    ASSERT_COND( col >= 1 && col <= mK );

    return node_id * mK + (col - 1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 彩色数
  SizeType mK;

  // γテーブル
  // サイズは node_num() * mK
  int* mGammaTable;

  // tabu list を表す配列
  // サイズは node_num() * mK
  SizeType* mTabuMatrix;

  // 現在の繰り返し回数
  SizeType mIter;

  // 乱数発生器
  std::mt19937 mRandGen;

};

END_NAMESPACE_YM_UDGRAPH

#endif // TABUCOL_H
