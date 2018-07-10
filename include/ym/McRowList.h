#ifndef YM_MCROWLIST_H
#define YM_MCROWLIST_H

/// @file ym/McRowList.h
/// @brief McRowList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"
#include "ym/McRowIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowList McRowList.h "ym/McRowList.h"
/// @brief McMatrix の要素を行方向にたどるためのクラス
///
/// このクラスは読み出し専用でリストの内容を変えることはできない．
//////////////////////////////////////////////////////////////////////
class McRowList
{
public:

  using iterator = McRowIterator;

public:

  /// @brief コンストラクタ
  /// @param[in] begin_cell 先頭の要素
  /// @param[in] end_cell 末尾の要素
  McRowList(McCell* begin_cell,
	    McCell* end_cell);

  /// @brief デストラクタ
  ~McRowList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の要素の列番号を返す．
  int
  front() const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;

  friend
  bool
  operator==(const McRowList& list1,
	     const McRowList& list2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  McCell* mBegin;

  // 末尾の要素
  McCell* mEnd;

};

/// @relates McRowList
/// @brief 等価比較演算子
/// @param[in] list1, list2 オペランド
bool
operator==(const McRowList& list1,
	   const McRowList& list2);

/// @relates McRowList
/// @brief 非等価比較演算子
/// @param[in] list1, list2 オペランド
bool
operator!=(const McRowList& list1,
	   const McRowList& list2);

/// @relates McRowList
/// @brief 包含関係を調べる．
/// @param[in] list1, list2 オペランド
///
/// list1 が list2 の要素をすべて含んでいたら true を返す．
bool
check_containment(const McRowList& list1,
		  const McRowList& list2);



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] begin_cell 先頭の要素
// @param[in] end_cell 末尾の要素
inline
McRowList::McRowList(McCell* begin_cell,
		     McCell* end_cell) :
  mBegin(begin_cell),
  mEnd(end_cell)
{
}

// @brief デストラクタ
inline
McRowList::~McRowList()
{
}

// @brief 先頭の要素を返す．
inline
int
McRowList::front() const
{
  return mBegin->col_pos();
}

// @brief 先頭の反復子を返す．
inline
McRowIterator
McRowList::begin() const
{
  return McRowIterator(mBegin);
}

// @brief 末尾の反復子を返す．
inline
McRowIterator
McRowList::end() const
{
  return McRowIterator(mEnd);
}

// @brief 非等価比較演算子
// @param[in] list1, list2 オペランド
inline
bool
operator!=(const McRowList& list1,
	   const McRowList& list2)
{
  return !operator==(list1, list2);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCROWLIST_H
