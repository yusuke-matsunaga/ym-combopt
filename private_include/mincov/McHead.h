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
  friend class McBlock;
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
  void
  init(int pos);

  /// @brief 位置番号を返す．
  int
  pos() const;

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief 要素数を増やす．
  /// @return 増加後の要素数を返す．
  int
  inc_num();

  /// @brief 要素数を減らす．
  /// @return 減少後の要素数を返す．
  int
  dec_num();

  /// @brief 行の先頭の要素を返す．
  McCell*
  row_front() const;

  /// @brief 行の末尾の要素を返す．
  McCell*
  row_back() const;

  /// @brief 行方向のリストを返す．
  McRowList
  row_list() const;

  /// @brief 行方向で挿入する位置を探す．
  /// @param[in] cell 挿入する要素
  /// @retval true 追加が成功した．
  /// @retval false 同じ要素がすでに存在した．
  ///
  /// 結果は cell の mLeftLink, mRightLink に設定される．
  bool
  row_search(McCell* cell);

  /// @brief 列の先頭の要素を返す．
  McCell*
  col_front() const;

  /// @brief 列の末尾の要素を返す．
  McCell*
  col_back() const;

  /// @brief 列方向のリストを返す．
  McColList
  col_list() const;

  /// @brief 列方向で挿入する位置を探す．
  /// @param[in] cell 挿入する要素
  /// @retval true 追加が成功した．
  /// @retval false 同じ要素がすでに存在した．
  ///
  /// 結果は cell の mUpLink, mDownLink に設定される．
  bool
  col_search(McCell* cell);

  /// @brief 直前のヘッダを返す．
  const McHead*
  prev() const;

  /// @brief 直後のヘッダを返す．
  const McHead*
  next() const;


public:
  //////////////////////////////////////////////////////////////////////
  // public かつ mutable という異例のデータメンバ
  //////////////////////////////////////////////////////////////////////

  mutable
  int mWork;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続している要素をクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置番号
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
inline
void
McHead::init(int pos)
{
  mPos = pos;
}

// @brief 位置番号を返す．
inline
int
McHead::pos() const
{
  return mPos;
}

// @brief 要素数を返す．
inline
int
McHead::num() const
{
  return mNum;
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
  return McRowList(row_front(), &mDummy);
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
  return McColList(col_front(), &mDummy);
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
