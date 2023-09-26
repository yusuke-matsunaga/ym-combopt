#ifndef YM_UDGRAPH_H
#define YM_UDGRAPH_H

/// @file ym/UdGraph.h
/// @brief UdGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016, 2018, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class UdEdge UdGraph.h "ym/UdGraph.h"
/// @brief UdGraph の枝を表すクラス
///
/// 向きがないので id1 <= id2 と仮定する．
//////////////////////////////////////////////////////////////////////
class UdEdge
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 内容は未定
  UdEdge() = default;

  /// @brief 内容を指定したコンストラクタ
  ///
  /// id1 > id2 の場合は取り替える．
  UdEdge(
    SizeType id1, ///< [in] ノード1の番号
    SizeType id2  ///< [in] ノード2の番号
  )
  {
    if ( id1 <= id2 ) {
      mId1 = id1;
      mId2 = id2;
    }
    else {
      mId1 = id2;
      mId2 = id1;
    }
  }

  /// @brief デストラクタ
  ~UdEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 小さい方のノード番号を得る．
  SizeType
  id1() const
  {
    return mId1;
  }

  /// @brief 大きい方のノード番号を得る．
  SizeType
  id2() const
  {
    return mId2;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号1
  SizeType mId1;

  // ノード番号2
  SizeType mId2;

};


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
  ///
  /// この状態では枝はない
  explicit
  UdGraph(
    SizeType node_num = 0 ///< [in] ノード数
  );

  /// @brief コピーコンストラクタ
  UdGraph(
    const UdGraph& src ///< [in] コピー元のオブジェクト
  ) = default;

  // @brief コピー代入演算子
  UdGraph&
  operator=(
    const UdGraph& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~UdGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を(再)設定する．
  ///
  /// 以前の内容は消去される．
  void
  resize(
    SizeType node_num ///< [in] ノード数
  );

  /// @brief 2つのノードを接続する．
  ///
  /// - 無向グラフなので id1 <= id2 になるように正規化する．
  /// - id1 == id2 も許す(反射)
  /// - 多重辺は許さない．
  void
  connect(
    SizeType id1, ///< [in] ノード1の番号 ( 0 <= id1 < node_num() )
    SizeType id2  ///< [in] ノード2の番号 ( 0 <= id2 < node_num() )
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  SizeType
  node_num() const;

  /// @brief 枝の総数を返す．
  SizeType
  edge_num() const;

  /// @brief 反射の時に true を返す．
  ///
  /// 反射とはすべてのノードに自己ループがあること
  bool
  is_reflective() const;

  /// @brief 枝の情報を返す．
  /// @return 枝を返す．
  const UdEdge&
  edge(
    SizeType idx ///< [in] 枝番号 ( 0 <= idx < edge_num() )
  ) const;

  /// @brief 全ての枝のリストを返す．
  const vector<UdEdge>&
  edge_list() const;

  /// @brief グラフの内容を出力する．
  void
  dump(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル入出力
  //////////////////////////////////////////////////////////////////////

  /// @brief DIMACS 形式のファイルを読み込む．
  ///
  /// エラーの場合には例外が送出される．
  static
  UdGraph
  read_dimacs(
    const string& filename ///< [in] 入力元のファイル名
  );

  /// @brief DIMACS 形式のファイルを読み込む．
  ///
  /// エラーの場合には例外が送出される．
  static
  UdGraph
  read_dimacs(
    istream& s    ///< [in] 入力元のストリーム
  );

  /// @brief 内容を DIMACS 形式で出力する．
  void
  write_dimacs(
    const string& filename ///< [in] 出力先のファイル名
  ) const;

  /// @brief 内容を DIMACS 形式で出力する．
  void
  write_dimacs(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // グラフアルゴリズム
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色問題を解く
  /// @return 彩色数を返す．
  SizeType
  coloring(
    vector<SizeType>& color_map, ///< [out] ノードに対する彩色結果(=SizeType)を収める配列
    const string& algorithm      ///< [in] アルゴリズム名
    = string{}                   ///< 空の時にはデフォルトのアルゴリズムを用いる．
  ) const;

  /// @brief (最大)独立集合を求める．
  /// @return 独立集合の要素(ノード番号)を収める配列
  vector<SizeType>
  independent_set(
    const string& algorithm      ///< [in] アルゴリズム名
    = string{}                   ///< 空文字列の時はデフォルトアルゴリズムが用いられる．
  ) const;

  /// @brief (最大)クリークを求める．
  /// @return クリークの要素(ノード番号)を収める配列
  vector<SizeType>
  max_clique(
    const string& algorithm     ///< [in] アルゴリズム名
    = string{}		        ///< 空文字列の時はデフォルトアルゴリズムが用いられる．
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  SizeType mNodeNum;

  // 枝の実体の配列
  vector<UdEdge> mEdgeList;

};


//////////////////////////////////////////////////////////////////////
/// @class UdgError UdGraph.h "UdGraph.h"
/// @brief UdGraph 関係のエラーを表す例外クラス
//////////////////////////////////////////////////////////////////////
class UdgError :
  public exception
{
public:

  /// @brief コンストラクタ
  UdgError(
    const string& msg ///< [in] エラーメッセージ
  ) : mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~UdgError() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表す文字列を返す．
  const string&
  str() const
  {
    return mMsg;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージ
  string mMsg;

};

END_NAMESPACE_YM_UDGRAPH

#endif // YM_UDGRAPH_H
