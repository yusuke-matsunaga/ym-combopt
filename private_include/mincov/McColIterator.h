#ifndef YM_MCCOLITERATOR_H
#define YM_MCCOLITERATOR_H

/// @file mincov/McColIterator.h
/// @brief McColIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "mincov/McCell.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McColIterator McColIterator.h "mincov/McColIterator.h"
/// @brief McMatrix の要素を列方向にたどる反復子
//////////////////////////////////////////////////////////////////////
class McColIterator
{
public:

  /// @brief コンストラクタ
  McColIterator(
    McCell* cell = nullptr ///< [in] 対象の McCell
  ) : mCurCell{cell}
  {
  }

  /// @brief デストラクタ
  ~McColIterator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  /// @return 要素の行番号を返す．
  SizeType
  operator*() const
  {
    return mCurCell->row_pos();
  }

  /// @brief インクリメント演算子
  McColIterator
  operator++()
  {
    if ( mCurCell != nullptr ) {
      mCurCell = mCurCell->col_next();
    }

    return *this;
  }

  /// @brief インクリメント演算子(後置)
  McColIterator
  operator++(int) // 引数はダミー
  {
    McColIterator ans{mCurCell};

    if ( mCurCell != nullptr ) {
      mCurCell = mCurCell->col_next();
    }

    return ans;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const McColIterator& right
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
  const McColIterator& left, ///< [in] オペランド1
  const McColIterator& right ///< [in] オペランド2
)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCCOLITERATOR_H
