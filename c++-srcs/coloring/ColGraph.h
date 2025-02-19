#ifndef COLGRAPH_H
#define COLGRAPH_H

/// @file ColGraph.h
/// @brief ColGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"


BEGIN_NAMESPACE_YM_COLORING

//////////////////////////////////////////////////////////////////////
/// @class ColGraph ColGraph.h "ColGraph.h"
/// @brief coloring 用のグラフを表すクラス
//////////////////////////////////////////////////////////////////////
class ColGraph
{
public:

  /// @brief コンストラクタ
  ColGraph(
    const UdGraph& graph ///< [in] 対象のグラフ
  );

  /// @brief コンストラクタ
  ColGraph(
    const UdGraph& graph,             ///< [in] 対象のグラフ
    const vector<SizeType>& color_map ///< [in] 部分的な彩色結果
  );

  /// @brief デストラクタ
  ~ColGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  SizeType
  node_num() const
  {
    return mNodeNum;
  }

  /// @brief 枝数を得る．
  SizeType
  edge_num() const
  {
    return mEdgeNum;
  }

  /// @brief ノード番号のリストを返す．
  ///
  /// 彩色されていたノードは含まないのでサイズは node_num() より小さい場合がある．
  const vector<SizeType>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief 隣接するノード番号のリストを得る．
  const vector<SizeType>&
  adj_list(
    SizeType node_id ///< [in] 対象のノード番号 ( 0 <= node_id < node_num() )
  ) const
  {
    if ( node_id >= node_num() ) {
      throw std::out_of_range{"node_id is out of range"};
    }

    return mAdjListArray[node_id];
  }

  /// @brief 隣接するノード数を返す．
  SizeType
  adj_degree(
    SizeType node_id ///< [in] 対象のノード番号 ( 0 <= node_id < node_num() )
  ) const
  {
    if ( node_id >= node_num() ) {
      throw std::out_of_range{"node_id is out of range"};
    }

    return mAdjListArray[node_id].size();
  }

  /// @brief 現在使用中の色数を返す．
  SizeType
  color_num() const
  {
    return mColNum;
  }

  /// @brief ノードの色を返す．
  SizeType
  color(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const
  {
    if ( node_id >= node_num() ) {
      throw std::out_of_range{"node_id is out of range"};
    }

    return mColorMap[node_id];
  }

  /// @brief 新しい色を割り当てる．
  /// @return 割り当てた新しい色を返す．
  SizeType
  new_color()
  {
    ++ mColNum;
    return mColNum;
  }

  /// @brief ノードに色を割り当てる．
  void
  set_color(
    SizeType node_id, ///< [in] ノード番号 ( 0 <= node_id < node_num() )
    SizeType color    ///< [in] 色 ( 1 <= color <= color_num() )
  )
  {
    if ( node_id >= node_num() ) {
      throw std::out_of_range{"node_id is out of range"};
    }
    if ( color == 0 ) {
      throw std::invalid_argument{"color should not be 0"};
    }
    if ( color > color_num() ) {
      throw std::out_of_range{"color is out of range"};
    }

    mColorMap[node_id] = color;
  }

  /// @brief ノード集合に色を割り当てる．
  void
  set_color(
    const vector<SizeType>& node_id_list, ///< [in] ノード番号のリスト
    SizeType color                        ///< [in] 色 ( 1 <= color <= color_num() )
  )
  {
    for ( auto node_id: node_id_list ) {
      set_color(node_id, color);
    }
  }

  /// @brief 彩色結果を得る．
  /// @return 彩色数(= color_num())を返す．
  SizeType
  get_color_map(
    vector<SizeType>& color_map ///< [out] 彩色結果を納めるベクタ
  ) const;

  /// @brief 全てのノードが彩色されていたら true を返す．
  bool
  is_colored() const;

  /// @brief 彩色結果が妥当か検証する．
  /// @retval true 妥当な彩色結果だった．
  /// @retval false 隣接ノード間で同じ色が割り当てられていた．
  ///
  /// ここでは彩色されていないノードの有無はチェックしない．
  bool
  verify() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  SizeType mNodeNum;

  // 枝数
  SizeType mEdgeNum;

  // ノードの隣接リストの配列
  vector<vector<SizeType>> mAdjListArray;

  // 未彩色のノードの配列
  vector<SizeType> mNodeList;

  // 現在使用中の色数
  SizeType mColNum;

  // 彩色結果の配列
  vector<SizeType> mColorMap;

};

END_NAMESPACE_YM_COLORING

#endif // COLGRAPH_H
