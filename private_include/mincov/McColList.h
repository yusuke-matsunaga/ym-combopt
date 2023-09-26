#ifndef YM_MCCOLLIST_H
#define YM_MCCOLLIST_H

/// @file mincov/McColList.h
/// @brief McColList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "mincov/McColIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McColList McColList.h "mincov/McColList.h"
/// @brief McMatrix の要素を列方向にたどるためのクラス
///
/// このクラスは読み出し専用でリストの内容を変えることはできない．
//////////////////////////////////////////////////////////////////////
class McColList
{
public:

  using iterator = McColIterator;

public:

  /// @brief コンストラクタ
  McColList(
    McCell* begin_cell, ///< [in] 先頭の要素
    McCell* end_cell	///< [in] 末尾の要素
  ) : mBegin{begin_cell},
      mEnd{end_cell}
  {
  }

  /// @brief デストラクタ
  ~McColList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return McColIterator{mBegin};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return McColIterator{mEnd};
  }

  friend
  bool
  operator==(
    const McColList& list1,
    const McColList& list2
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

/// @relates McColList
/// @brief 等価比較演算子
///< [in] list1, list2 オペランド
bool
operator==(
  const McColList& list1,
  const McColList& list2
);

/// @relates McColList
/// @brief 非等価比較演算子
///< [in] list1, list2 オペランド
inline
bool
operator!=(
  const McColList& list1,
  const McColList& list2
)
{
  return !operator==(list1, list2);
}

/// @relates McColList
/// @brief 包含関係を調べる．
///
/// list1 が list2 の要素をすべて含んでいたら true を返す．
bool
check_containment(
  const McColList& list1, ///< [in] オペランド1
  const McColList& list2  ///< [in] オペランド2
);

END_NAMESPACE_YM_MINCOV

#endif // YM_MCCOLLIST_H
