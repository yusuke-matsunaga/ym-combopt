#ifndef COLGRAPH_H
#define COLGRAPH_H

/// @file ColGraph.h
/// @brief ColGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/udgraph_nsdef.h"
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
  /// @param[in] graph 対象のグラフ
  ColGraph(const UdGraph& graph);

  /// @brief コンストラクタ
  /// @param[in] graph 対象のグラフ
  /// @param[in] color_map 部分的な彩色結果
  ColGraph(const UdGraph& graph,
	    const vector<int>& color_map);

  /// @brief デストラクタ
  ~ColGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  int
  node_num() const;

  /// @brief 枝数を得る．
  int
  edge_num() const;

  /// @brief ノード番号のリストを返す．
  ///
  /// 彩色されていたノードは含まないのでサイズは node_num() より小さい場合がある．
  Array<int>
  node_list() const;

  /// @brief 隣接するノード番号のリストを得る．
  /// @param[in] node_id 対象のノード番号 ( 0 <= node_id < node_num() )
  Array<int>
  adj_list(int node_id) const;

  /// @brief 現在使用中の色数を返す．
  int
  color_num() const;

  /// @brief ノードの色を返す．
  /// @param[in] node_id ノード番号 ( 0 <= node_id < node_num() )
  int
  color(int node_id) const;

  /// @brief 新しい色を割り当てる．
  /// @return 割り当てた新しい色を返す．
  int
  new_color();

  /// @brief ノードに色を割り当てる．
  /// @param[in] node_id ノード番号 ( 0 <= node_id < node_num() )
  /// @param[in] color 色 ( 1 <= color <= color_num() )
  void
  set_color(int node_id,
	    int color);

  /// @brief ノードに色を割り当てる．
  /// @param[in] node_id_list ノード番号のリスト
  /// @param[in] color 色 ( 1 <= color <= color_num() )
  void
  set_color(const vector<int>& node_id_list,
	    int color);

  /// @brief 彩色結果を得る．
  /// @param[out] color_map 彩色結果を納めるベクタ
  /// @return 彩色数(= color_num())を返す．
  int
  get_color_map(vector<int>& color_map) const;

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
  /// @param[in] graph 対象のグラフ
  /// @param[in] color_map 部分的な彩色結果
  ///
  /// コンストラクタのみから使われると仮定しているので
  /// 古い内容の破棄は行わない．
  void
  init(const UdGraph& graph,
       const vector<int>& color_map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 隣接リストを表す構造体
  struct AdjList
  {
    AdjList();

    ~AdjList();

    // 要素数
    int mNum;

    // 実体の配列
    int* mBody;
  };

  // ノード数
  int mNodeNum;

  // 枝数
  int mEdgeNum;

  // ノードの隣接リストの配列
  // サイズは mNodeNum;
  AdjList* mAdjListArray;

  // 未彩色のノード数
  int mNodeNum1;

  // 未彩色のノードの配列
  // サイズは mNodeNum1;
  int* mNodeList;

  // 現在使用中の色数
  int mColNum;

  // 彩色結果の配列
  int* mColorMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
int
ColGraph::node_num() const
{
  return mNodeNum;
}

// @brief 枝数を得る．
inline
int
ColGraph::edge_num() const
{
  return mEdgeNum;
}

// @brief ノード番号のリストを返す．
//
// 彩色されていたノードは含まないのでサイズは node_num() より小さい場合がある．
inline
Array<int>
ColGraph::node_list() const
{
  return Array<int>(mNodeList, 0, mNodeNum1);
}

// @brief 隣接するノード番号のリストを得る．
// @param[in] node_id 対象のノード番号 ( 0 <= node_id < node_num() )
inline
Array<int>
ColGraph::adj_list(int node_id) const
{
  ASSERT_COND( node_id >= 0 && node_id < node_num() );

  const AdjList& adj_list = mAdjListArray[node_id];
  return Array<int>(adj_list.mBody, 0, adj_list.mNum);
}

// @brief 現在使用中の色数を返す．
inline
int
ColGraph::color_num() const
{
  return mColNum;
}

// @brief ノードの色を返す．
// @param[in] node_id ノード番号 ( 0 <= node_id < node_num() )
inline
int
ColGraph::color(int node_id) const
{
  ASSERT_COND( node_id >= 0 && node_id < node_num() );

  return mColorMap[node_id];
}

// @brief 新しい色を割り当てる．
// @return 割り当てた新しい色を返す．
inline
int
ColGraph::new_color()
{
  ++ mColNum;
  return mColNum;
}

// @brief ノードに色を割り当てる．
// @param[in] node_id ノード番号 ( 0 <= node_id < node_num() )
// @param[in] color 色 ( 1 <= color <= color_num() )
inline
void
ColGraph::set_color(int node_id,
		    int color)
{
  ASSERT_COND( node_id >= 0 && node_id < node_num() );
  ASSERT_COND( color >= 1 && color <= color_num() );

  mColorMap[node_id] = color;
}

// @brief ノードに色を割り当てる．
// @param[in] node_id_list ノード番号のリスト
// @param[in] color 色 ( 1 <= color <= color_num() )
inline
void
ColGraph::set_color(const vector<int>& node_id_list,
		    int color)
{
  for ( auto node_id: node_id_list ) {
    set_color(node_id, color);
  }
}

inline
ColGraph::AdjList::AdjList() :
  mNum(0),
  mBody(nullptr)
{
}

inline
ColGraph::AdjList::~AdjList()
{
  delete [] mBody;
}

END_NAMESPACE_YM_UDGRAPH

#endif // COLGRAPH_H
