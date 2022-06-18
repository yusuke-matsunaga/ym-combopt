#ifndef YM_MCHEADLIST_H
#define YM_MCHEADLIST_H

/// @file ym/McHeadList.h
/// @brief McHeadList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "ym/McHead.h"
#include "ym/McHeadIterator.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McHeadList McHeadList.h "McHeadList.h"
/// @brief McHead のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class McHeadList
{
  friend class McMatrix;

public:

  using iterator = McHeadIterator;

public:

  /// @brief コンストラクタ
  McHeadList(
  ) :mNum{0}
  {
  }

  /// @brief デストラクタ
  ~McHeadList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear()
  {
    mDummy.mNext = mDummy.mPrev = &mDummy;
    mNum = 0;
  }

  /// @brief 内容をセットする．
  void
  set(
    const vector<McHead*>& head_list
  );

  /// @brief 要素を追加する．
  void
  insert(
    McHead* head ///< [in] 追加する要素
  );

  /// @brief 要素を削除する．
  ///
  /// head がこのリストに含まれていると仮定する．
  void
  exclude(
    McHead* head ///< [in] 削除する要素
  );

  /// @brief delete() で削除した要素を復元する．
  void
  restore(
    McHead* head ///< [in] 復元する要素
  );

  /// @brief 分割したリストをマージして元にもどす．
  ///
  /// src1, src2 の内容は破棄される．
  void
  merge(
    McHeadList& src1, ///< [in] リスト1
    McHeadList& src2  ///< [in] リスト2
  );

  /// @brief 要素数を返す．
  SizeType
  num() const
  {
    return mNum;
  }

  /// @brief 最初の要素を返す．
  const McHead*
  front() const
  {
    return mDummy.mNext;
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return McHeadIterator(mDummy.mNext);
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return McHeadIterator(&mDummy);
  }

  friend
  bool
  operator==(
    const McHeadList& list1,
    const McHeadList& list2
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ダミーの先頭要素
  McHead mDummy;

  // 要素数
  SizeType mNum;

};

/// @relates McHeadList
/// @brief 等価比較演算子
bool
operator==(
  const McHeadList& list1, ///< [in] オペランド1
  const McHeadList& list2  ///< [in] オペランド2
);

/// @relates McHeadList
/// @brief 非等価比較演算子
inline
bool
operator!=(
  const McHeadList& list1, ///< [in] オペランド1
  const McHeadList& list2  ///< [in] オペランド2
)
{
  return !operator==(list1, list2);
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCHEADLIST_H
