#ifndef DSATUR_H
#define DSATUR_H

/// @file Dsatur.h
/// @brief Dsatur のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "coloring/ColGraph.h"


BEGIN_NAMESPACE_YM_UDGRAPH

class DsatNode;

//////////////////////////////////////////////////////////////////////
/// @class Dsatur Dsatur.h "Dsatur.h"
/// @brief 彩色問題を dsatur アルゴリズムで解くためのクラス
//////////////////////////////////////////////////////////////////////
class Dsatur :
  public ColGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  Dsatur(const UdGraph& graph);

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  /// @param[in] color_map 部分的な彩色結果
  Dsatur(const UdGraph& graph,
	 const vector<int>& color_map);

  /// @brief デストラクタ
  ~Dsatur();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色する．
  /// @param[out] color_map ノードに対する彩色結果(=int)を収める配列
  /// @return 彩色数を返す．
  int
  coloring(vector<int>& color_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief ノードに色を割り当てる．
  void
  color_node(int node_id,
	     int color);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの実体の配列
  DsatNode* mNodeArray;

};

END_NAMESPACE_YM_UDGRAPH

#endif // DSATUR_H
