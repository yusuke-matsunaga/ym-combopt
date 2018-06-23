#ifndef MCCOLITERATOR_H
#define MCCOLITERATOR_H

/// @file McColIterator.h
/// @brief McColIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"
#include "McCell.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McColIterator McColIterator.h "McColIterator.h"
/// @brief McCell を列方向にたどる反復子
//////////////////////////////////////////////////////////////////////
class McColIterator
{
public:

  /// @brief コンストラクタ
  /// @param[in] cell 対象の McCell
  McColIterator(const McCell* cell = nullptr);

  /// @brief デストラクタ
  ~McColIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif dereference 演算子
  const McCell*
  operator*() const;

  /// @brief インクリメント演算子
  McColIterator
  operator++();

  /// @brief インクリメント演算子(後置)
  McColIterator
  operator++(int);

  /// @brief 等価比較演算子
  bool
  operator==(const McColIterator& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在のセル
  const McCell* mCurCell;

};

/// @brief 非等価比較演算子
bool
operator!=(const McColIterator& left,
	   const McColIterator& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cell 対象の McCell
inline
McColIterator::McColIterator(const McCell* cell) :
  mCurCell(cell)
{
}

// @brief デストラクタ
inline
McColIterator::~McColIterator()
{
}

// @breif dereference 演算子
inline
const McCell*
McColIterator::operator*() const
{
  return mCurCell;
}

// @brief インクリメント演算子
inline
McColIterator
McColIterator::operator++()
{
  if ( mCurCell != nullptr ) {
    mCurCell = mCurCell->col_next();
  }

  return *this;
}

// @brief インクリメント演算子
inline
McColIterator
McColIterator::operator++(int)
{
  McColIterator ans(mCurCell);

  if ( mCurCell != nullptr ) {
    mCurCell = mCurCell->col_next();
  }

  return ans;
}

// @brief 等価比較演算子
inline
bool
McColIterator::operator==(const McColIterator& right) const
{
  return mCurCell == right.mCurCell;
}

// @brief 非等価比較演算子
inline
bool
operator!=(const McColIterator& left,
	   const McColIterator& right)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_MINCOV

#endif // MCCOLITERATOR_H
