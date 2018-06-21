#ifndef ISX_H
#define ISX_H

/// @file Isx.h
/// @brief Isx のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "ColGraph.h"
#include "ym/RandGen.h"


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
  /// @param[in] graph 対象のグラフ
  Isx(const UdGraph& graph);

  /// @brief デストラクタ
  ~Isx();


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

  /// @brief ノード集合に新しい色を割り当てる．
  /// @param[in] node_set ノード集合
  void
  color_nodes(const vector<int>& node_set);


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

  // 乱数生成器
  RandGen mRandGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_UDGRAPH

#endif // ISX_H
