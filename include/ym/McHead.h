#ifndef YM_MCHEAD_H
#define YM_MCHEAD_H

/// @file ym/McHead.h
/// @brief McHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McHead McHead.h "McHead.h"
/// @brief Mincov 用の行と列のヘッダを表すクラス
///
/// * 行か列かを表す1ビットのフラグ
/// * 行番号/列番号
/// * 削除フラグ
/// * 行/列の要素数
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

  /// @brief 要素数を増やす．
  void
  inc_num();

  /// @brief 要素数を減らす．
  void
  dec_num();

  /// @brief 削除フラグを返す．
  bool
  is_deleted() const;

  /// @brief 削除フラグをセットする．
  /// @param[in] flag フラグの値
  void
  set_deleted(bool flag);

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置番号 + 行/列フラグ
  // 最下位ビットが 0:行, 1:列 を表す．
  int mPos;

  // 要素数 + 削除フラグ
  // 最下位ビットが削除フラグ
  unsigned int mNum;

  // 直前のヘッダを指すリンク
  McHead* mPrev;

  // 直後のヘッダを指すリンク
  McHead* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McHead::McHead() :
  mPos(0),
  mNum(0),
  mPrev(this),
  mNext(this)
{
}

// @brief デストラクタ
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
  return static_cast<int>(mNum >> 1);
}

// @brief 削除フラグを返す．
inline
bool
McHead::is_deleted() const
{
  return static_cast<bool>(mNum & 1);
}

// @brief 要素数を増やす．
inline
void
McHead::inc_num()
{
  mNum += 2;
}

// @brief 要素数を減らす．
inline
void
McHead::dec_num()
{
  mNum -= 2;
}

// @brief 削除フラグをセットする．
// @param[in] flag フラグの値
inline
void
McHead::set_deleted(bool flag)
{
  if ( flag ) {
    mNum |= 1U;
  }
  else {
    mNum &= ~1U;
  }
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

END_NAMESPACE_YM_MINCOV

#endif // YM_MCHEAD_H
