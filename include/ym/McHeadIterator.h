#ifndef YM_MCHEADITERATOR_H
#define YM_MCHEADITERATOR_H

/// @file ym/McHeadIterator.h
/// @brief McHeadIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McHeadIterator McHeadIterator.h "ym/McHeadIterator.h"
/// @brief McHead の反復子
//////////////////////////////////////////////////////////////////////
class McHeadIterator
{
public:

  /// @brief コンストラクタ
  McHeadIterator(const McHead* head = nullptr);

  /// @brief デストラクタ
  ~McHeadIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif dereference 演算子
  int
  operator*() const;

  /// @brief インクリメント演算子
  McHeadIterator
  operator++();

  /// @brief インクリメント演算子
  McHeadIterator
  operator++(int);

  /// @brief 等価比較演算子
  bool
  operator==(const McHeadIterator& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の ヘッダ
  const McHead* mCurHead;

};

/// @brief 非等価比較演算子
bool
operator!=(const McHeadIterator& left,
	   const McHeadIterator& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McHeadIterator::McHeadIterator(const McHead* head) :
  mCurHead(head)
{
}

// @brief デストラクタ
inline
McHeadIterator::~McHeadIterator()
{
}

// @breif dereference 演算子
inline
int
McHeadIterator::operator*() const
{
  return mCurHead->pos();
}

// @brief インクリメント演算子
inline
McHeadIterator
McHeadIterator::operator++()
{
  if ( mCurHead != nullptr ) {
    mCurHead = mCurHead->next();
  }

  return *this;
}

// @brief インクリメント演算子(後置)
inline
McHeadIterator
McHeadIterator::operator++(int)
{
  McHeadIterator ans(mCurHead);

  if ( mCurHead != nullptr ) {
    mCurHead = mCurHead->next();
  }

  return ans;
}

// @brief 等価比較演算子
inline
bool
McHeadIterator::operator==(const McHeadIterator& right) const
{
  return mCurHead == right.mCurHead;
}

/// @brief 非等価比較演算子
inline
bool
operator!=(const McHeadIterator& left,
	   const McHeadIterator& right)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCHEADITERATOR_H
