#ifndef MCCOLLIST_H
#define MCCOLLIST_H

/// @file McColList.h
/// @brief McColList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"
#include "McColIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McColList McColList.h "McColList.h"
/// @brief McCell を列方向にたどるためのクラス
///
/// このクラスは読み出し専用でリストの内容を変えることはできない．
//////////////////////////////////////////////////////////////////////
class McColList
{
public:

  using iterator = McColIterator;

public:

  /// @brief コンストラクタ
  /// @param[in] begin_cell 先頭の要素
  /// @param[in] end_cell 末尾の要素
  McColList(const McCell* begin_cell,
	    const McCell* end_cell);

  /// @brief デストラクタ
  ~McColList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;

  friend
  bool
  operator==(const McColList& list1,
	     const McColList& list2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  const McCell* mBegin;

  // 末尾の要素
  const McCell* mEnd;

};

/// @relates McColList
/// @brief 等価比較演算子
/// @param[in] list1, list2 オペランド
bool
operator==(const McColList& list1,
	   const McColList& list2);

/// @relates McColList
/// @brief 非等価比較演算子
/// @param[in] list1, list2 オペランド
bool
operator!=(const McColList& list1,
	   const McColList& list2);

/// @relates McColList
/// @brief 包含関係を調べる．
/// @param[in] list1, list2 オペランド
///
/// list1 が list2 の要素をすべて含んでいたら true を返す．
bool
check_containment(const McColList& list1,
		  const McColList& list2);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] begin_cell 先頭の要素
// @param[in] end_cell 末尾の要素
inline
McColList::McColList(const McCell* begin_cell,
		     const McCell* end_cell) :
  mBegin(begin_cell),
  mEnd(end_cell)
{
}

// @brief デストラクタ
inline
McColList::~McColList()
{
}

// @brief 先頭の反復子を返す．
inline
McColIterator
McColList::begin() const
{
  return McColIterator(mBegin);
}

// @brief 末尾の反復子を返す．
inline
McColIterator
McColList::end() const
{
  return McColIterator(mEnd);
}

// @brief 非等価比較演算子
// @param[in] list1, list2 オペランド
inline
bool
operator!=(const McColList& list1,
	   const McColList& list2)
{
  return !operator==(list1, list2);
}

END_NAMESPACE_YM_MINCOV

#endif // MCCOLLIST_H
