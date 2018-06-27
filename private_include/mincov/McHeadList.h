#ifndef MCHEADLIST_H
#define MCHEADLIST_H

/// @file McHeadList.h
/// @brief McHeadList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "McHead.h"
#include "McHeadIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McHeadList McHeadList.h "McHeadList.h"
/// @brief McHead のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class McHeadList
{
  friend class McMatrix;

public:

  using iterator = McHeadIterator;

public:

  /// @brief コンストラクタ
  McHeadList();

  /// @brief デストラクタ
  ~McHeadList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 内容をセットする．
  void
  set(const vector<McHead*>& head_list);

  /// @brief 要素を追加する．
  /// @param[in] head 追加する要素
  void
  insert(McHead* head);

  /// @brief 要素を削除する．
  /// @param[in] head 削除する要素
  ///
  /// head がこのリストに含まれていると仮定する．
  void
  exclude(McHead* head);

  /// @brief delete() で削除した要素を復元する．
  /// @param[in] head 復元する要素
  void
  restore(McHead* head);

  /// @brief 分割したリストをマージして元にもどす．
  /// @param[in] src1, src2 分割したリスト
  ///
  /// src1, src2 の内容は破棄される．
  void
  merge(McHeadList& src1,
	McHeadList& src2);

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief 最初の要素を返す．
  const McHead*
  front() const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;

  friend
  bool
  operator==(const McHeadList& list1,
	     const McHeadList& list2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ダミーの先頭要素
  McHead mDummy;

  // 要素数
  int mNum;

};

/// @relates McHeadList
/// @brief 等価比較演算子
/// @param[in] list1, list2 オペランド
bool
operator==(const McHeadList& list1,
	   const McHeadList& list2);

/// @relates McHeadList
/// @brief 非等価比較演算子
/// @param[in] list1, list2 オペランド
bool
operator!=(const McHeadList& list1,
	   const McHeadList& list2);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McHeadList::McHeadList() :
  mNum(0)
{
}

// @brief デストラクタ
inline
McHeadList::~McHeadList()
{
}

// @brief 内容をクリアする．
inline
void
McHeadList::clear()
{
  mDummy.mNext = mDummy.mPrev = &mDummy;
  mNum = 0;
}

// @brief 要素数を返す．
inline
int
McHeadList::num() const
{
  return mNum;
}

// @brief 最初の要素を返す．
inline
const McHead*
McHeadList::front() const
{
  return mDummy.mNext;
}

// @brief 先頭の反復子を返す．
inline
McHeadIterator
McHeadList::begin() const
{
  return McHeadIterator(mDummy.mNext);
}

// @brief 末尾の反復子を返す．
inline
McHeadIterator
McHeadList::end() const
{
  return McHeadIterator(&mDummy);
}

// @relates McHeadList
// @brief 非等価比較演算子
// @param[in] list1, list2 オペランド
inline
bool
operator!=(const McHeadList& list1,
	   const McHeadList& list2)
{
  return !operator==(list1, list2);
}

END_NAMESPACE_YM_MINCOV

#endif // MCHEADLIST_H
