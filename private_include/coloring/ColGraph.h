#ifndef COLGRAPH_H
#define COLGRAPH_H

/// @file ColGraph.h
/// @brief ColGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
#include "ym/Array.h"


BEGIN_NAMESPACE_YM_UDGRAPH

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
  ~ColGraph();


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
  Array<SizeType>
  node_list() const
  {
    return Array<SizeType>{mNodeList, 0, mNodeNum1};
  }

  /// @brief 隣接するノード番号のリストを得る．
  Array<SizeType>
  adj_list(
    SizeType node_id ///< [in] 対象のノード番号 ( 0 <= node_id < node_num() )
  ) const
  {
    ASSERT_COND( node_id >= 0 && node_id < node_num() );

    const auto& adj_list = mAdjListArray[node_id];
    return Array<SizeType>{adj_list.mBody, 0, adj_list.mNum};
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
    ASSERT_COND( 0 <= node_id && node_id < node_num() );

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
    ASSERT_COND( 0 <= node_id && node_id < node_num() );
    ASSERT_COND( 1 <= color && color <= color_num() );

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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をセットする．
  ///
  /// コンストラクタのみから使われると仮定しているので
  /// 古い内容の破棄は行わない．
  void
  init(
    const UdGraph& graph,             ///< [in] 対象のグラフ
    const vector<SizeType>& color_map ///< [in] 部分的な彩色結果
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 隣接リストを表す構造体
  struct AdjList
  {
    ~AdjList()
    {
      delete [] mBody;
    }

    // 要素数
    SizeType mNum{0};

    // 実体の配列
    SizeType* mBody{nullptr};
  };

  // ノード数
  SizeType mNodeNum;

  // 枝数
  SizeType mEdgeNum;

  // ノードの隣接リストの配列
  // サイズは mNodeNum;
  AdjList* mAdjListArray;

  // 未彩色のノード数
  SizeType mNodeNum1;

  // 未彩色のノードの配列
  // サイズは mNodeNum1;
  SizeType* mNodeList;

  // 現在使用中の色数
  SizeType mColNum;

  // 彩色結果の配列
  SizeType* mColorMap;

};

END_NAMESPACE_YM_UDGRAPH

#endif // COLGRAPH_H
