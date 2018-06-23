#ifndef MCROWITERATOR_H
#define MCROWITERATOR_H

/// @file McRowIterator.h
/// @brief McRowIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"
#include "McCell.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowIterator McRowIterator.h "McRowIterator.h"
/// @brief McCell を行方向にたどる反復子
//////////////////////////////////////////////////////////////////////
class McRowIterator
{
public:

  /// @brief コンストラクタ
  /// @param[in] cell 対象の McCell
  McRowIterator(McCell* cell = nullptr);

  /// @brief デストラクタ
  ~McRowIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif dereference 演算子
  McCell*
  operator*() const;

  /// @brief インクリメント演算子
  McRowIterator
  operator++();

  /// @brief インクリメント演算子(後置)
  McRowIterator
  operator++(int);

  /// @brief 等価比較演算子
  bool
  operator==(const McRowIterator& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在のセル
  McCell* mCurCell;

};

/// @brief 非等価比較演算子
bool
operator!=(const McRowIterator& left,
	   const McRowIterator& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cell 対象の McCell
inline
McRowIterator::McRowIterator(McCell* cell) :
  mCurCell(cell)
{
}

// @brief デストラクタ
inline
McRowIterator::~McRowIterator()
{
}

// @breif dereference 演算子
inline
McCell*
McRowIterator::operator*() const
{
  return mCurCell;
}

// @brief インクリメント演算子
inline
McRowIterator
McRowIterator::operator++()
{
  if ( mCurCell != nullptr ) {
    mCurCell = mCurCell->row_next();
  }

  return *this;
}

// @brief インクリメント演算子
inline
McRowIterator
McRowIterator::operator++(int)
{
  McRowIterator ans(mCurCell);

  if ( mCurCell != nullptr ) {
    mCurCell = mCurCell->row_next();
  }

  return ans;
}

// @brief 等価比較演算子
inline
bool
McRowIterator::operator==(const McRowIterator& right) const
{
  return mCurCell == right.mCurCell;
}

// @brief 非等価比較演算子
inline
bool
operator!=(const McRowIterator& left,
	   const McRowIterator& right)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_MINCOV

#endif // MCROWITERATOR_H
