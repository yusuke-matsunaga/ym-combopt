#ifndef YM_MCROWITERATOR_H
#define YM_MCROWITERATOR_H

/// @file ym/McRowIterator.h
/// @brief McRowIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "ym/McCell.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowIterator McRowIterator.h "ym/McRowIterator.h"
/// @brief McMatrix の要素を行方向にたどる反復子
//////////////////////////////////////////////////////////////////////
class McRowIterator
{
public:

  /// @brief コンストラクタ
  McRowIterator(
    McCell* cell = nullptr ///< [in] 対象の McCell
  ) : mCurCell{cell}
  {
  }

  /// @brief デストラクタ
  ~McRowIterator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  /// @return 要素の列番号を返す．
  SizeType
  operator*() const
  {
    return mCurCell->col_pos();
  }

  /// @brief インクリメント演算子
  McRowIterator
  operator++()
  {
    if ( mCurCell != nullptr ) {
      mCurCell = mCurCell->row_next();
    }

    return *this;
  }

  /// @brief インクリメント演算子(後置)
  McRowIterator
  operator++(int)
  {
    McRowIterator ans{mCurCell};

    if ( mCurCell != nullptr ) {
      mCurCell = mCurCell->row_next();
    }

    return ans;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const McRowIterator& right
  ) const
  {
    return mCurCell == right.mCurCell;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在のセル
  McCell* mCurCell;

};

/// @brief 非等価比較演算子
inline
bool
operator!=(
  const McRowIterator& left, ///< [in] オペランド1
  const McRowIterator& right ///< [in] オペランド2
)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCROWITERATOR_H
