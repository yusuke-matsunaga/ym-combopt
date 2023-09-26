#ifndef DSATNODE_H
#define DSATNODE_H

/// @file DsatNode.h
/// @brief DsatNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"


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
  init(
    SizeType id,
    SizeType adj_degree,
    SizeType vectlen
  );

  /// @brief ID番号を得る．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief saturation degree を返す．
  SizeType
  sat_degree() const
  {
    return mSatDegree;
  }

  /// @brief 隣接ノードの次数を返す．
  SizeType
  adj_degree() const
  {
    return mAdjDegree;
  }

  /// @brief color のノードが隣接しているときに true を返す．
  bool
  check_adj_color(
    SizeType color
  ) const
  {
    auto blk = color / 64;
    auto sft = color % 64;
    return ((mColorSet[blk] >> sft) & 1ULL) == 1UL;
  }

  /// @brief color を隣接色に加える．
  void
  add_adj_color(
    SizeType color
  )
  {
    auto blk = color / 64;
    auto sft = color % 64;
    mColorSet[blk] |= (1ULL << sft);
    ++ mSatDegree;
  }

  /// @brief ヒープ上の位置(+1)を返す．
  ///
  /// ヒープになければ 0 を返す．
  SizeType
  heap_location() const
  {
    return mHeapIdx;
  }

  /// @brief ヒープ上の位置を設定する．
  void
  set_heap_location(
    SizeType pos
  )
  {
    mHeapIdx = pos;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // ヒープ中の位置(+1)
  SizeType mHeapIdx;

  // 隣接するノードの色の集合を表すビットベクタ
  std::uint64_t* mColorSet;

  // SAT degree
  SizeType mSatDegree;

  // adjacent degree
  SizeType mAdjDegree;

};

END_NAMESPACE_YM_UDGRAPH

#endif // DSATNODE_H
