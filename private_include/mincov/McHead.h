#ifndef MCHEAD_H
#define MCHEAD_H

/// @file McHead.h
/// @brief McHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"

#include "McRowList.h"
#include "McColList.h"
#include "McCell.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McHead McHead.h "McHead.h"
/// @brief Mincov 用の行と列のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class McHead
{
  friend class McHeadList;

public:

  /// @brief コンストラクタ
  McHead();

  /// @brief デストラクタ
  ~McHead();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] pos 位置番号
  /// @param[in] col_flag 列の時に true とするフラグ
  void
  init(int pos,
       bool col_flag);

  /// @brief 行ヘッダの時に true を返す．
  bool
  is_row() const;

  /// @brief 列ヘッダの時に true を返す．
  bool
  is_col() const;

  /// @brief 位置番号を返す．
  int
  pos() const;

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief 削除フラグを返す．
  bool
  is_deleted() const;

  /// @brief 行の先頭の要素を返す．
  McCell*
  row_front() const;

  /// @brief 行の末尾の要素を返す．
  McCell*
  row_back() const;

  /// @brief 行方向のリストを返す．
  McRowList
  row_list() const;

  /// @brief 行方向のリストに挿入する．
  /// @param[in] cell 挿入する要素
  /// @retval true 追加が成功した．
  /// @retval false 同じ要素がすでに存在した．
  bool
  row_insert(McCell* cell);

  /// @brief 行方向のリストから削除する．
  /// @param[in] cell 削除する要素
  void
  row_delete(McCell* cell);

  /// @brief row_delete() で削除したセルを元の位置に戻す．
  /// @param[in] cell 削除する要素
  ///
  /// cell->row_prev(), cell->row_next() に正しい値が入っている
  void
  row_restore(McCell* cell);

  /// @brief 列の先頭の要素を返す．
  McCell*
  col_front() const;

  /// @brief 列の末尾の要素を返す．
  McCell*
  col_back() const;

  /// @brief 列方向のリストを返す．
  McColList
  col_list() const;

  /// @brief 列方向のリストに挿入する.
  /// @param[in] cell 挿入する要素
  /// @retval true 追加が成功した．
  /// @retval false 同じ要素がすでに存在した．
  bool
  col_insert(McCell* cell);

  /// @brief 列方向のリストから削除する．
  /// @param[in] cell 削除する要素
  void
  col_delete(McCell* cell);

  /// @brief col_delete() で削除したセルを元の位置に戻す．
  /// @param[in] cell 削除する要素
  ///
  /// cell->col_prev(), cell->col_next() に正しい値が入っている
  void
  col_restore(McCell* cell);

  /// @brief 直前のヘッダを返す．
  const McHead*
  prev() const;

  /// @brief 直後のヘッダを返す．
  const McHead*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続している要素をクリアする．
  void
  clear();

  /// @brief 要素数を増やす．
  /// @return 増加後の要素数を返す．
  int
  inc_num();

  /// @brief 要素数を減らす．
  /// @return 減少後の要素数を返す．
  int
  dec_num();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置番号 + 行/列フラグ
  // 最下位ビットが 0:行, 1:列 を表す．
  int mPos;

  // 要素数
  int mNum;

  // 二重連結リストのダミーヘッダ
  McCell mDummy;

  // 直前のヘッダを指すリンク
  McHead* mPrev;

  // 直後のヘッダを指すリンク
  McHead* mNext;

  // 削除フラグ
  bool mDeleted;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McHead::McHead() :
  mPos(0),
  mDummy(-1, -1),
  mPrev(nullptr),
  mNext(nullptr),
  mDeleted(false)
{
  clear();
}

/// @brief デストラクタ
inline
McHead::~McHead()
{
}

// @brief 初期化する．
// @param[in] pos 位置番号
// @param[in] col_flag 列の時に true とするフラグ
inline
void
McHead::init(int pos,
	     bool col_flag)
{
  mPos = (pos << 1) | static_cast<int>(col_flag);
}

// @brief 行ヘッダの時に true を返す．
inline
bool
McHead::is_row() const
{
  return !is_col();
}

// @brief 列ヘッダの時に true を返す．
inline
bool
McHead::is_col() const
{
  return static_cast<bool>(mPos & 1);
}

// @brief 位置番号を返す．
inline
int
McHead::pos() const
{
  return mPos >> 1;
}

// @brief 要素数を返す．
inline
int
McHead::num() const
{
  return mNum;
}

// @brief 削除フラグを返す．
inline
bool
McHead::is_deleted() const
{
  return mDeleted;
}

// @brief 要素数を増やす．
inline
int
McHead::inc_num()
{
  return ++ mNum;
}

// @brief 要素数を減らす．
inline
int
McHead::dec_num()
{
  return -- mNum;
}

// @brief 行の先頭の要素を返す．
inline
McCell*
McHead::row_front() const
{
  return mDummy.mRightLink;
}

// @brief 行の末尾の要素を返す．
inline
McCell*
McHead::row_back() const
{
  return mDummy.mLeftLink;
}

// @brief 行方向のリストを返す．
inline
McRowList
McHead::row_list() const
{
  return McRowList(row_front(), const_cast<McCell*>(&mDummy));
}

// @brief 行方向のリストから削除する．
// @param[in] cell 削除する要素
inline
void
McHead::row_delete(McCell* cell)
{
  ASSERT_COND( is_row() );

  McCell* prev = cell->mLeftLink;
  McCell* next = cell->mRightLink;

  ASSERT_COND( prev->mRightLink == cell );
  ASSERT_COND( next->mLeftLink == cell );

  prev->mRightLink = next;
  next->mLeftLink = prev;

  dec_num();
}

// @brief row_delete() で削除したセルを元の位置に戻す．
// @param[in] cell 削除する要素
//
// cell->row_prev(), cell->row_next() に正しい値が入っている
inline
void
McHead::row_restore(McCell* cell)
{
  ASSERT_COND( is_row() );

  McCell* prev = cell->mLeftLink;
  McCell* next = cell->mRightLink;

  ASSERT_COND( prev->mRightLink == next );
  ASSERT_COND( next->mLeftLink == prev );

  prev->mRightLink = cell;
  next->mLeftLink = cell;

  inc_num();
}

// @brief 列の先頭の要素を返す．
inline
McCell*
McHead::col_front() const
{
  return mDummy.mDownLink;
}

// @brief 列の末尾の要素を返す．
inline
McCell*
McHead::col_back() const
{
  return mDummy.mUpLink;
}

// @brief 列方向のリストを返す．
inline
McColList
McHead::col_list() const
{
  return McColList(col_front(), const_cast<McCell*>(&mDummy));
}

// @brief 列方向のリストから削除する．
// @param[in] cell 削除する要素
inline
void
McHead::col_delete(McCell* cell)
{
  ASSERT_COND( is_col() );

  McCell* prev = cell->mUpLink;
  McCell* next = cell->mDownLink;

  ASSERT_COND( prev->mDownLink == cell );
  ASSERT_COND( next->mUpLink == cell );

  prev->mDownLink = next;
  next->mUpLink = prev;

  dec_num();
}

// @brief col_delete() で削除したセルを元の位置に戻す．
// @param[in] cell 削除する要素
//
// cell->col_prev(), cell->col_next() に正しい値が入っている
inline
void
McHead::col_restore(McCell* cell)
{
  ASSERT_COND( is_col() );

  McCell* prev = cell->mUpLink;
  McCell* next = cell->mDownLink;

  ASSERT_COND( prev->mDownLink == next );
  ASSERT_COND( next->mUpLink == prev );

  prev->mDownLink = cell;
  next->mUpLink = cell;

  inc_num();
}

// @brief 直前のヘッダを返す．
inline
const McHead*
McHead::prev() const
{
  return mPrev;
}

// @brief 直後のヘッダを返す．
inline
const McHead*
McHead::next() const
{
  return mNext;
}

// @brief 接続している要素をクリアする．
inline
void
McHead::clear()
{
  mDummy.mLeftLink = &mDummy;
  mDummy.mRightLink = &mDummy;
  mDummy.mUpLink = &mDummy;
  mDummy.mDownLink = &mDummy;
  mNum = 0;
}

END_NAMESPACE_YM_MINCOV

#endif // MCHEAD_H
