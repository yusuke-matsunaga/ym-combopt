#ifndef ISCOV_H
#define ISCOV_H

/// @file IsCov.h
/// @brief IsCov のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "ColGraph.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class IsCov IsCov.h "IsCov.h"
/// @brief independent set covering を行うクラス
//////////////////////////////////////////////////////////////////////
class IsCov
{
public:

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  IsCov(const UdGraph& graph);

  /// @brief デストラクタ
  ~IsCov();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief independent set cover を行う．
  int
  covering(int limit,
	   vector<int>& color_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 集合に加えるノードを選ぶ．
  /// @param[in] cand_list 候補となるノードのリスト
  /// @param[in] cur_mark 現在の候補集合を表す配列
  ///
  /// - cur_mark[node_id] = true のノードが候補集合の要素
  /// - 現在の集合に隣接していないノードの内，隣接ノード数の少ないものを選ぶ．
  /// - 追加できるノードがない場合は -1 を返す．
  int
  select_node(const vector<int>& cand_list,
	      const vector<bool>& cur_mark);

  /// @brief 候補リストを更新する．
  /// @param[inout] cand_list 候補リスト
  /// @param[in] node_id 新たに加わったノード
  void
  update_cand_list(vector<int>& cand_list,
		   int node_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のグラフ
  ColGraph mGraph;

  // 乱数生成器
  RandGen mRandGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_UDGRAPH

#endif // ISCOV_H
