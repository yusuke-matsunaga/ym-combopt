#ifndef DSATUR_H
#define DSATUR_H

/// @file Dsatur.h
/// @brief Dsatur のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
#include "ColGraph.h"


BEGIN_NAMESPACE_YM_COLORING

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
  ~Dsatur() = default;


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

  /// @brief SAT degree を返す．
  SizeType
  sat_degree(
    SizeType node_id
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに関係する諸元を表す構造体
  struct NodeInfo
  {

    // 隣接するノードの色の集合を表すビットベクタ
    vector<bool> mColorSet;

    // SAT degree
    SizeType mSatDegree;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色の結果を SAT degree に反映させる．
  /// @return 変化したノードのリストを返す．
  vector<SizeType>
  update_sat_degree(
    SizeType node_id
  );

  /// @brief 隣接ノードに color の色を持つノードがあるか調べる．
  bool
  check_adj_color(
    SizeType node_id,
    SizeType color
  )
  {
    auto& node_info = mNodeArray[node_id];
    return node_info.mColorSet[color];
  }

  /// @brief 隣接ノードの色を追加する．
  void
  add_adj_color(
    SizeType node_id,
    SizeType color
  )
  {
    auto& node_info = mNodeArray[node_id];
    node_info.mColorSet[color] = true;
    ++ node_info.mSatDegree;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの実体の配列
  vector<NodeInfo> mNodeArray;

};

END_NAMESPACE_YM_COLORING

#endif // DSATUR_H
