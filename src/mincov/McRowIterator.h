#ifndef MCROWITERATOR_H
#define MCROWITERATOR_H

/// @file McRowIterator.h
/// @brief McRowIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowIterator McRowIterator.h "McRowIterator.h"
/// @brief McRowHead の反復子
//////////////////////////////////////////////////////////////////////
class McRowIterator
{
public:

  /// @brief コンストラクタ
  McRowIterator(const McRowHead* row = nullptr);

  /// @brief デストラクタ
  ~McRowIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif dereference 演算子
  const McRowHead*
  operator*() const;

  /// @brief インクリメント演算子
  McRowIterator
  operator++();

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

  // 現在の McRowHead
  const McRowHead* mCurRow;

};

/// @brief 非等価比較演算子
inline
bool
operator!=(const McRowIterator& left,
	   const McRowIterator& right)
{
  return !left.operator==(right);
}


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McRowIterator::McRowIterator(const McRowHead* row) :
  mCurRow(row)
{
}

// @brief デストラクタ
inline
McRowIterator::~McRowIterator()
{
}

// @breif dereference 演算子
inline
const McRowHead*
McRowIterator::operator*() const
{
  return mCurRow;
}

// @brief インクリメント演算子
inline
McRowIterator
McRowIterator::operator++()
{
  if ( mCurRow != nullptr ) {
    mCurRow = mCurRow->next();
  }

  return *this;
}

// @brief 等価比較演算子
inline
bool
McRowIterator::operator==(const McRowIterator& right) const
{
  return mCurRow == right.mCurRow;
}

END_NAMESPACE_YM_MINCOV

#endif // MCROWITERATOR_H
