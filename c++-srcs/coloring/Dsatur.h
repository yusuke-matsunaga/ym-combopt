#ifndef DSATUR_H
#define DSATUR_H

/// @file Dsatur.h
/// @brief Dsatur のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
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
  Dsatur(
    const UdGraph& graph ///< [in] 対象のグラフ
  );

  /// @brief コンストラクタ
  Dsatur(
    const UdGraph& graph,             ///< [in] 対象のグラフ
    const vector<SizeType>& color_map ///< [in] 部分的な彩色結果
  );

  /// @brief デストラクタ
  ~Dsatur();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色する．
  /// @return 彩色数を返す．
  SizeType
  coloring(
    vector<SizeType>& color_map ///< [out] ノードに対する彩色結果(=int)を収める配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief ノードに色を割り当てる．
  void
  color_node(
    SizeType node_id,
    SizeType color
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの実体の配列
  vector<DsatNode> mNodeArray;

};

END_NAMESPACE_YM_UDGRAPH

#endif // DSATUR_H
