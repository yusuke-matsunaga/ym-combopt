#ifndef MISNODE_H
#define MISNODE_H

/// @file MisNode.h
/// @brief MisNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_mincov.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MisNode MisNode.h "MisNode.h"
/// @brief LbMIS で用いられるクラス
//////////////////////////////////////////////////////////////////////
class MisNode
{
  friend class MisNodeHeap;

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @param[in] row_pos 行番号
  void
  set(int row_pos);

  /// @brief 隣接ノードの情報を設定する．
  void
  set_adj_link(int adj_num,
	       MisNode** adj_link);

  /// @brief 行番号を返す．
  int
  row_pos() const;

  /// @brief 削除済みフラグを返す．
  bool
  deleted() const;

  /// @brief 隣接するノード数を返す．
  int
  adj_size() const;

  /// @brief 隣接するノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
  MisNode*
  adj_node(int pos) const;

  /// @brief 有効な隣接ノード数を返す．
  int
  adj_num() const;

  /// @brief adj_num を１減らす
  void
  dec_adj_num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  int mRowPos;

  // 隣接するノードのポインタ配列
  MisNode** mAdjLink;

  // mAdjLink のサイズ
  int mAdjSize;

  // mAdjLink 中の有効な要素数
  int mNum;

  // ヒープ上のインデックス
  int mHeapIdx;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 内容を初期化する．
// @param[in] row_pos 行番号
inline
void
MisNode::set(int row_pos)
{
  mRowPos = row_pos;
  mAdjLink = nullptr;
  mAdjSize = 0;
  mNum = 0;
  mHeapIdx = 0;
}

// @brief 隣接ノードの情報を設定する．
inline
void
MisNode::set_adj_link(int adj_num,
		      MisNode** adj_link)
{
  mAdjLink = adj_link;
  mAdjSize = adj_num;
  mNum = adj_num;
}

// @brief 行番号を返す．
inline
int
MisNode::row_pos() const
{
  return mRowPos;
}

// @brief 削除済みフラグを返す．
inline
bool
MisNode::deleted() const
{
  return mHeapIdx == 0;
}

// @brief 隣接するノード数を返す．
inline
int
MisNode::adj_size() const
{
  return mAdjSize;
}

// @brief 隣接するノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
inline
MisNode*
MisNode::adj_node(int pos) const
{
  return mAdjLink[pos];
}

// @brief 有効な隣接ノード数を返す．
inline
int
MisNode::adj_num() const
{
  return mNum;
}

// @brief adj_num を１減らす
inline
void
MisNode::dec_adj_num()
{
  -- mNum;
}

END_NAMESPACE_YM_MINCOV

#endif // MISNODE_H
