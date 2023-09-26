#ifndef YM_MCHEAD_H
#define YM_MCHEAD_H

/// @file mincov/McHead.h
/// @brief McHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"


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
  McHead(
  ) : mPos{0},
      mNum{0},
      mPrev{this},
      mNext{this}
  {
  }

  /// @brief デストラクタ
  ~McHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init(
    SizeType pos, ///< [in] 位置番号
    bool col_flag ///< [in] 列の時に true とするフラグ
  )
  {
    mPos = (pos << 1) | static_cast<SizeType>(col_flag);
  }

  /// @brief 行ヘッダの時に true を返す．
  bool
  is_row() const
  {
    return !is_col();
  }

  /// @brief 列ヘッダの時に true を返す．
  bool
  is_col() const
  {
    return static_cast<bool>(mPos & 1);
  }

  /// @brief 位置番号を返す．
  SizeType
  pos() const
  {
    return mPos >> 1;
  }

  /// @brief 要素数を返す．
  SizeType
  num() const
  {
    return mNum >> 2;
  }

  /// @brief 要素数を増やす．
  void
  inc_num()
  {
    mNum += 4;
    set_dirty(true);
  }

  /// @brief 要素数を減らす．
  void
  dec_num()
  {
    mNum -= 4;
    set_dirty(true);
  }

  /// @brief 削除フラグを返す．
  bool
  is_deleted() const
  {
    return static_cast<bool>((mNum >> 0) & 1U);
  }

  /// @brief 削除フラグをセットする．
  void
  set_deleted(
    bool flag ///< [in] フラグの値
  )
  {
    if ( flag ) {
      mNum |= 1U;
    }
    else {
      mNum &= ~1U;
    }
  }

  /// @brief dirty フラグを返す．
  bool
  is_dirty() const
  {
    return static_cast<bool>((mNum >> 1) & 1U);
  }

  /// @brief dirty フラグをセットする．
  void
  set_dirty(
    bool flag ///< [in] フラグの値
  )
  {
    if ( flag ) {
      mNum |= 2U;
    }
    else {
      mNum &= ~2U;
    }
  }

  /// @brief 直前のヘッダを返す．
  const McHead*
  prev() const
  {
    return mPrev;
  }

  /// @brief 直後のヘッダを返す．
  const McHead*
  next() const
  {
    return mNext;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置番号 + 行/列フラグ
  // 最下位ビットが 0:行, 1:列 を表す．
  SizeType mPos;

  // 要素数 + dirty フラグ + 削除フラグ
  SizeType mNum;

  // 直前のヘッダを指すリンク
  McHead* mPrev;

  // 直後のヘッダを指すリンク
  McHead* mNext;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MCHEAD_H
