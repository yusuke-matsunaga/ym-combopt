#ifndef YM_UDGRAPH_H
#define YM_UDGRAPH_H

/// @file ym/UdGraph.h
/// @brief UdGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"
#include "ym/UdEdge.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class UdGraph UdGraph.h "ym/UdGraph.h"
/// @brief 一般的な無向グラフを表すクラス
///
/// - Undirected Graph の略
/// - 枝(ノード番号の対)のリストを持つ．
/// - このクラスはインターフェイス用のもので凝ったデータ構造は持っていない．
//////////////////////////////////////////////////////////////////////
class UdGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  ///
  /// この状態では枝はない
  explicit
  UdGraph(int node_num = 0);

  /// @brief コピーコンストラクタ
  UdGraph(const UdGraph& src) = default;

  // @brief コピー代入演算子
  UdGraph&
  operator=(const UdGraph& src) = default;

  /// @brief デストラクタ
  ~UdGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を(再)設定する．
  /// @param[in] node_num ノード数
  ///
  /// 以前の内容は消去される．
  void
  resize(int node_num);

  /// @brief 2つのノードを接続する．
  /// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
  ///
  /// - 無向グラフなので id1 <= id2 になるように正規化する．
  /// - id1 == id2 も許す(反射)
  /// - 多重辺は許さない．
  void
  connect(int id1,
	  int id2);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  int
  node_num() const;

  /// @brief 枝の総数を返す．
  int
  edge_num() const;

  /// @brief 反射の時に true を返す．
  ///
  /// 反射とはすべてのノードに自己ループがあること
  bool
  is_reflective() const;

  /// @brief 枝の情報を返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  /// @return 枝を返す．
  const UdEdge&
  edge(int idx) const;

  /// @brief 全ての枝のリストを返す．
  const vector<UdEdge>&
  edge_list() const;

  /// @brief グラフの内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // グラフアルゴリズム
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色問題を解く
  /// @param[in] algorithm アルゴリズム名
  /// @param[out] color_map ノードに対する彩色結果(=int)を収める配列
  /// @return 彩色数を返す．
  int
  coloring(const string& algorithm,
	   vector<int>& color_map) const;

  /// @brief 彩色問題を解く
  /// @param[out] color_map ノードに対する彩色結果(=int)を収める配列
  /// @return 彩色数を返す．
  ///
  /// デフォルトのヒューリスティックを用いる．
  int
  coloring(vector<int>& color_map) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  int mNodeNum;

  // 枝の実体の配列
  vector<UdEdge> mEdgeList;

};

END_NAMESPACE_YM_UDGRAPH

BEGIN_NAMESPACE_YM

/// @relates UdGraph
/// @brief DIMACS 形式のファイルを読み込む．
/// @param[in] filename 入力元のファイル名
/// @param[in] graph 読み込んだ内容を設定するグラフ
/// @retval true 読み込みが成功した．
/// @retval false 読み込みが失敗した．
extern
bool
read_dimacs(const string& filename,
	    UdGraph& graph);

/// @relates UdGraph
/// @brief DIMACS 形式のファイルを読み込む．
/// @param[in] s 入力元のストリーム
/// @param[in] graph 読み込んだ内容を設定するグラフ
/// @retval true 読み込みが成功した．
/// @retval false 読み込みが失敗した．
extern
bool
read_dimacs(istream& s,
	    UdGraph& graph);

/// @relates UdGraph
/// @brief 内容を DIMACS 形式で出力する．
/// @param[in] filename 出力先のファイル名
/// @param[in] graph 対象のグラフ
extern
void
write_dimacs(const string& filename,
	     const UdGraph& graph);

/// @relates UdGraph
/// @brief 内容を DIMACS 形式で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] graph 対象のグラフ
extern
void
write_dimacs(ostream& s,
	     const UdGraph& graph);

/// @relates UdGraph
/// @brief (最大)独立集合を求める．
/// @param[in] graph 対象のグラフ
/// @param[in] algorithm アルゴリズム名
/// @param[out] node_set 独立集合の要素(ノード番号)を収める配列
/// @return 要素数を返す．
extern
int
independent_set(const UdGraph& graph,
		const string& algorithm,
		vector<int>& node_set);

/// @relates UdGraph
/// @brief (最大)独立集合を求める．
/// @param[in] graph 対象のグラフ
/// @param[out] node_set 独立集合の要素(ノード番号)を収める配列
/// @return 要素数を返す．
///
/// デフォルトのヒューリスティックを用いる．
extern
int
independent_set(const UdGraph& graph,
		vector<int>& node_set);

/// @relates UdGraph
/// @brief (最大)クリークを求める．
/// @param[in] graph 対象のグラフ
/// @param[in] algorithm アルゴリズム名
/// @param[out] node_set クリークの要素(ノード番号)を収める配列
/// @return 要素数を返す．
extern
int
max_clique(const UdGraph& graph,
	   const string& algorithm,
	   vector<int>& node_set);

/// @relates UdGraph
/// @brief (最大)クリークを求める．
/// @param[in] graph 対象のグラフ
/// @param[out] node_set クリークの要素(ノード番号)を収める配列
/// @return 要素数を返す．
extern
int
max_clique(const UdGraph& graph,
	   vector<int>& node_set);

END_NAMESPACE_YM

#endif // YM_UDGRAPH_H
