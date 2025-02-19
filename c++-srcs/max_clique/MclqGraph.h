#ifndef MCLQGRAPH_H
#define MCLQGRAPH_H

/// @file MclqGraph.h
/// @brief MclqGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_MAXCLIQUE

//////////////////////////////////////////////////////////////////////
/// @class MclqGraph MclqGraph.h "MclqGraph.h"
/// @brief max clique 用のグラフ構造を表すクラス
//////////////////////////////////////////////////////////////////////
class MclqGraph
{
public:

  /// @brief コンストラクタ
  MclqGraph(
    const UdGraph& graph ///< [in] 対象のグラフ
  ) : mNodeNum{graph.node_num()},
      mAdjLinkArray(mNodeNum)
  {
    // 隣接するノードの情報を link_array に入れる．
    vector<vector<SizeType>> link_array(mNodeNum);
    for ( auto& edge: graph.edge_list() ) {
      auto id1 = edge.id1();
      auto id2 = edge.id2();
      link_array[id1].push_back(id2);
      link_array[id2].push_back(id1);
    }

    // link_array の情報を mAdjLinkArray に移す．
    for ( SizeType id = 0; id < mNodeNum; ++ id ) {
      mAdjLinkArray[id] = link_array[id];
    }
  }

  /// @brief デストラクタ
  ~MclqGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeNum;
  }

  /// @brief 隣接するノード番号のリストを返す．
  const vector<SizeType>&
  adj_list(
    SizeType id ///< [in] ノード番号 ( 0 <= id < node_num() )
  ) const
  {
    _check_id(id);
    return mAdjLinkArray[id];
  }

  /// @brief 隣接するノード数を返す．
  SizeType
  adj_num(
    SizeType id ///< [in] ノード番号 ( 0 <= id < node_num() )
  ) const
  {
    return adj_list(id).size();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号が正しいかチェックする．
  void
  _check_id(
    SizeType id ///< [in] ノード番号
  ) const
  {
    if ( id >= node_num() ) {
      throw std::out_of_range{"id is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  SizeType mNodeNum;

  // 隣接リストの配列
  vector<vector<SizeType>> mAdjLinkArray;

};

END_NAMESPACE_YM_MAXCLIQUE

#endif // MCLQGRAPH_H
