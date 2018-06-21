#ifndef DSATNODE_H
#define DSATNODE_H

/// @file DsatNode.h
/// @brief DsatNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class DsatNode DsatNode.h "DsatNode.h"
/// @brief 彩色問題を表すグラフのノード
//////////////////////////////////////////////////////////////////////
class DsatNode
{
public:

  /// @brief コンストラクタ
  DsatNode();

  /// @brief デストラクタ
  ~DsatNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init(int id,
       int adj_degree,
       int vectlen);

  /// @brief ID番号を得る．
  int
  id() const;

  /// @brief saturation degree を返す．
  int
  sat_degree() const;

  /// @brief 隣接ノードの次数を返す．
  int
  adj_degree() const;

  /// @brief color のノードが隣接しているときに true を返す．
  bool
  check_adj_color(int color) const;

  /// @brief color を隣接色に加える．
  void
  add_adj_color(int color);

  /// @brief ヒープ上の位置(+1)を返す．
  ///
  /// ヒープになければ 0 を返す．
  int
  heap_location() const;

  /// @brief ヒープ上の位置を設定する．
  void
  set_heap_location(int pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  int mId;

  // ヒープ中の位置(+1)
  int mHeapIdx;

  // 隣接するノードの色の集合を表すビットベクタ
  ymuint64* mColorSet;

  // SAT degree
  int mSatDegree;

  // adjacent degree
  int mAdjDegree;
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
int
DsatNode::id() const
{
  return mId;
}

// @brief saturation degree を返す．
inline
int
DsatNode::sat_degree() const
{
  return mSatDegree;
}

// @brief 隣接ノードの次数を返す．
inline
int
DsatNode::adj_degree() const
{
  return mAdjDegree;
}

// @brief color のノードが隣接しているときに true を返す．
inline
bool
DsatNode::check_adj_color(int color) const
{
  ymuint blk = color / 64;
  ymuint sft = color % 64;
  return ((mColorSet[blk] >> sft) & 1ULL) == 1UL;
}

// @brief color を隣接色に加える．
inline
void
DsatNode::add_adj_color(int color)
{
  ymuint blk = color / 64;
  ymuint sft = color % 64;
  mColorSet[blk] |= (1ULL << sft);
  ++ mSatDegree;
}

// @brief ヒープ上の位置(+1)を返す．
//
// ヒープになければ 0 を返す．
inline
int
DsatNode::heap_location() const
{
  return mHeapIdx;
}

// @brief ヒープ上の位置を設定する．
inline
void
DsatNode::set_heap_location(int pos)
{
  mHeapIdx = pos;
}

END_NAMESPACE_YM_UDGRAPH

#endif // DSATNODE_H
