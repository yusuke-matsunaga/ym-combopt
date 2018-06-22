#ifndef MCROWLIST_H
#define MCROWLIST_H

/// @file McRowList.h
/// @brief McRowList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "McRowHead.h"
#include "McRowIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowList McRowList.h "McRowList.h"
/// @brief McRowHead のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class McRowList
{
  friend class McMatrix;

public:

  using iterator = McRowIterator;

public:

  /// @brief コンストラクタ
  McRowList();

  /// @brief デストラクタ
  ~McRowList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 内容をセットする．
  void
  set(const vector<McRowHead*>& row_list);

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ダミーの先頭要素
  McRowHead mDummyHead;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McRowList::McRowList() :
  mDummyHead(-1)
{
}

// @brief デストラクタ
inline
McRowList::~McRowList()
{
}

// @brief 内容をクリアする．
inline
void
McRowList::clear()
{
  mDummyHead.mNext = mDummyHead.mPrev = &mDummyHead;
}

// @brief 先頭の反復子を返す．
inline
McRowIterator
McRowList::begin() const
{
  return McRowIterator(mDummyHead.mNext);
}

// @brief 末尾の反復子を返す．
inline
McRowIterator
McRowList::end() const
{
  return McRowIterator(&mDummyHead);
}

END_NAMESPACE_YM_MINCOV

#endif // MCROWLIST_H
