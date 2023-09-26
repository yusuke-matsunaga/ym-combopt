#ifndef YM_MCHEADITERATOR_H
#define YM_MCHEADITERATOR_H

/// @file mincov/McHeadIterator.h
/// @brief McHeadIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McHeadIterator McHeadIterator.h "mincov/McHeadIterator.h"
/// @brief McHead の反復子
//////////////////////////////////////////////////////////////////////
class McHeadIterator
{
public:

  /// @brief コンストラクタ
  McHeadIterator(
    const McHead* head = nullptr
  ) : mCurHead{head}
  {
  }

  /// @brief デストラクタ
  ~McHeadIterator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  SizeType
  operator*() const
  {
    return mCurHead->pos();
  }

  /// @brief インクリメント演算子
  McHeadIterator
  operator++()
  {
    if ( mCurHead != nullptr ) {
      mCurHead = mCurHead->next();
    }

    return *this;
  }

  /// @brief インクリメント演算子
  McHeadIterator
  operator++(int)
  {
    McHeadIterator ans{mCurHead};

    if ( mCurHead != nullptr ) {
      mCurHead = mCurHead->next();
    }

    return ans;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const McHeadIterator& right
  ) const
  {
    return mCurHead == right.mCurHead;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の ヘッダ
  const McHead* mCurHead;

};

/// @brief 非等価比較演算子
inline
bool
operator!=(
  const McHeadIterator& left,
  const McHeadIterator& right
)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCHEADITERATOR_H
