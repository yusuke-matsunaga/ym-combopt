#ifndef YM_MCROWLIST_H
#define YM_MCROWLIST_H

/// @file mincov/McRowList.h
/// @brief McRowList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "mincov/McRowIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowList McRowList.h "mincov/McRowList.h"
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
  McRowList(
    McCell* begin_cell, ///< [in] 先頭の要素
    McCell* end_cell	///< [in] 末尾の要素
  ) : mBegin{begin_cell},
      mEnd{end_cell}
  {
  }

  /// @brief デストラクタ
  ~McRowList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の要素の列番号を返す．
  SizeType
  front() const
  {
    return mBegin->col_pos();
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return McRowIterator{mBegin};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return McRowIterator{mEnd};
  }

  friend
  bool
  operator==(
    const McRowList& list1,
    const McRowList& list2
  );


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
bool
operator==(
  const McRowList& list1,  ///< [in] オペランド1
  const McRowList& list2   ///< [in] オペランド2
);

/// @relates McRowList
/// @brief 非等価比較演算子
inline
bool
operator!=(
  const McRowList& list1, ///< [in] オペランド1
  const McRowList& list2  ///< [in] オペランド2
)
{
  return !operator==(list1, list2);
}

/// @relates McRowList
/// @brief 包含関係を調べる．
///
/// list1 が list2 の要素をすべて含んでいたら true を返す．
bool
check_containment(
  const McRowList& list1,  ///< [in] オペランド1
  const McRowList& list2   ///< [in] オペランド2
);

END_NAMESPACE_YM_MINCOV

#endif // YM_MCROWLIST_H
