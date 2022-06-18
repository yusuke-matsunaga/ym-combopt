#ifndef ISCOV_H
#define ISCOV_H

/// @file IsCov.h
/// @brief IsCov のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/udgraph_nsdef.h"
#include "coloring/ColGraph.h"
#include <random>


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class IsCov IsCov.h "IsCov.h"
/// @brief independent set covering を行うクラス
//////////////////////////////////////////////////////////////////////
class IsCov
{
public:

  /// @brief コンストラクタ
  IsCov(
    const UdGraph& graph ///< [in] 対象のグラフ
  );

  /// @brief デストラクタ
  ~IsCov();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief independent set cover を行う．
  SizeType
  covering(
    SizeType limit,
    vector<SizeType>& color_map
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 集合に加えるノードを選ぶ．
  ///
  /// - cur_mark[node_id] = true のノードが候補集合の要素
  /// - 現在の集合に隣接していないノードの内，隣接ノード数の少ないものを選ぶ．
  /// - 追加できるノードがない場合は -1 を返す．
  SizeType
  select_node(
    const vector<SizeType>& cand_list, ///< [in] 候補となるノードのリスト
    const vector<bool>& cur_mark       ///< [in] 現在の候補集合を表す配列
  );

  /// @brief 候補リストを更新する．
  void
  update_cand_list(
    vector<SizeType>& cand_list, ///< [inout] 候補リスト
    SizeType node_id		 ///< [in] 新たに加わったノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のグラフ
  ColGraph mGraph;

  // 乱数生成器
  std::mt19937 mRandGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_UDGRAPH

#endif // ISCOV_H
