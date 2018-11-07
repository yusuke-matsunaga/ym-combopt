#ifndef ISX2_H
#define ISX2_H

/// @file Isx2.h
/// @brief Isx2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "coloring/ColGraph.h"
#include <random>


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class Isx2 Isx2.h "Isx2.h"
/// @brief independent set extraction を行うクラス
//////////////////////////////////////////////////////////////////////
class Isx2 :
  public ColGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  Isx2(const UdGraph& graph);

  /// @brief デストラクタ
  ~Isx2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief independent set extraction を用いた coloring を行う．
  /// @param[in] limit 残りのノード数がこの値を下回ったら処理をやめる．
  /// @param[out] color_map 彩色結果を収める配列
  /// @return 彩色数を返す．
  ///
  /// ここでは部分的な彩色を行う．
  int
  coloring(int limit,
	   vector<int>& color_map);


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

  /// @brief 独立集合を mIndepSetList に追加する．
  /// @param[in] indep_set 追加する独立集合
  /// @retval true 正常に追加した．
  /// @retval false すでに同じ内容の独立集合が存在した．
  bool
  add_indep_set(const vector<int>& indep_set);

  /// @brief pairwise disjoint な極大集合を求める．
  /// @param[out] max_iset 結果を集合番号を収めるベクタ
  void
  get_max_disjoint_set(vector<int>& max_iset);

  /// @brief mCandList, mCandMark を初期化する．
  void
  init_cand_list();

  /// @brief 集合に加えるノ選ぶ．
  ///
  /// - 独立集合に隣接していないノードの内，隣接ノード数の少ないものを選ぶ．
  /// - 追加できるノードがない場合は -1 を返す．
  int
  select_node();

  /// @brief 候補リストを更新する．
  /// @param[in] node_id 新たに加わったノード
  void
  update_cand_list(int node_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 候補ノードのリスト
  vector<int> mCandList;

  // 候補ノードの印
  // サイズは node_num()
  vector<bool> mCandMark;

  // 候補ノードの隣接数
  // サイズは node_num()
  vector<int> mAdjCount;

  // select_node() で用いる作業用リスト
  vector<int> mTmpList;

  // 現在の独立集合
  vector<int> mIndepSet;

  // 現在の独立集合のリスト
  vector<vector<int> > mIndepSetList;

  // 乱数生成器
  std::mt19937 mRandGen;

  // 完全なランダム選択をする確率
  double mRandRatio;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_UDGRAPH

#endif // ISX2_H
