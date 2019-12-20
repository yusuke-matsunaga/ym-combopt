#ifndef MCLQNODE_H
#define MCLQNODE_H

/// @file MclqNode.h
/// @brief MclqNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/udgraph_nsdef.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class MclqNode MclqNode.h "MclqNode.h"
/// @brief MclqHeap で用いられるクラス
//////////////////////////////////////////////////////////////////////
class MclqNode
{
public:

  /// @brief コンストラクタ
  MclqNode() = default;

  /// @brief デストラクタ
  ~MclqNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @param[in] id ノード番号
  void
  set(int id);

  /// @brief 隣接ノードの情報を設定する．
  void
  set_adj_link(int adj_num,
	       MclqNode** adj_link);

  /// @brief ノード番号を返す．
  int
  id() const;

  /// @brief 削除済みフラグを返す．
  bool
  deleted() const;

  /// @brief 隣接するノード数を返す．
  int
  adj_size() const;

  /// @brief 隣接するノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
  MclqNode*
  adj_node(int pos) const;

  /// @brief 有効な隣接ノード数を返す．
  int
  adj_num() const;

  /// @brief adj_num を1減らす
  void
  dec_adj_num();

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

  // ノード番号
  int mId;

  // 隣接するノードのポインタ配列
  MclqNode** mAdjLink{nullptr};

  // mAdjLink のサイズ
  int mAdjSize{0};

  // mAdjLink 中の有効な要素数
  int mNum{0};

  // ヒープ上のインデックス
  int mHeapIdx{0};

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
inline
MclqNode::~MclqNode()
{
  delete [] mAdjLink;
}

// @brief 内容を初期化する．
// @param[in] id ノード番号
inline
void
MclqNode::set(int id)
{
  mId = id;
}

// @brief 隣接ノードの情報を設定する．
inline
void
MclqNode::set_adj_link(int adj_num,
		       MclqNode** adj_link)
{
  mAdjLink = adj_link;
  mAdjSize = adj_num;
  mNum = adj_num;
}

// @brief ノード番号を返す．
inline
int
MclqNode::id() const
{
  return mId;
}

// @brief 削除済みフラグを返す．
inline
bool
MclqNode::deleted() const
{
  return mHeapIdx == 0;
}

// @brief 隣接するノード数を返す．
inline
int
MclqNode::adj_size() const
{
  return mAdjSize;
}

// @brief 隣接するノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
inline
MclqNode*
MclqNode::adj_node(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < adj_size() );

  return mAdjLink[pos];
}

// @brief 有効な隣接ノード数を返す．
inline
int
MclqNode::adj_num() const
{
  return mNum;
}

// @brief adj_num を１減らす
inline
void
MclqNode::dec_adj_num()
{
  -- mNum;
}

// @brief ヒープ上の位置(+1)を返す．
//
// ヒープになければ 0 を返す．
inline
int
MclqNode::heap_location() const
{
  return mHeapIdx;
}

// @brief ヒープ上の位置を設定する．
inline
void
MclqNode::set_heap_location(int pos)
{
  mHeapIdx = pos;
}

END_NAMESPACE_YM_UDGRAPH

#endif // MCLQNODE_H
