#ifndef ISX_H
#define ISX_H

/// @file Isx.h
/// @brief Isx のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
#include "coloring/ColGraph.h"
#include <random>


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class Isx Isx.h "Isx.h"
/// @brief independent set extraction を行うクラス
//////////////////////////////////////////////////////////////////////
class Isx :
  public ColGraph
{
public:

  /// @brief コンストラクタ
  Isx(
    const UdGraph& graph ///< [in] 対象のグラフ
  );

  /// @brief デストラクタ
  ~Isx();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief independent set extraction を用いた coloring を行う．
  /// @return 彩色数を返す．
  ///
  /// ここでは部分的な彩色を行う．
  SizeType
  coloring(
    SizeType limit,             ///< [in] 残りのノード数がこの値を下回ったら処理をやめる．
    vector<SizeType>& color_map	///< [out] 彩色結果を収める配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief maximal independent set を選ぶ．
  ///
  /// - 結果は mIndepSet に格納される．
  /// - mRandGen を用いてランダムに選ぶ．
  void
  get_indep_set();

  /// @brief mCandList, mCandMark を初期化する．
  void
  init_cand_list();

  /// @brief 候補リストを更新する．
  void
  update_cand_list(
    SizeType node_id ///< [in] 新たに加わったノード
  );

  /// @brief ランダムに選ぶ．
  SizeType
  random_select(
    const vector<SizeType>& cand_list ///< [in] 候補のノード番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 候補ノードのリスト
  vector<SizeType> mCandList;

  // 候補ノードの印
  // サイズは node_num()
  vector<bool> mCandMark;

  // 候補ノードの隣接数
  // サイズは node_num()
  vector<SizeType> mAdjCount;

  // 現在の独立集合
  vector<SizeType> mIndepSet;

  // 乱数生成器
  std::mt19937 mRandGen;

};

END_NAMESPACE_YM_UDGRAPH

#endif // ISX_H
