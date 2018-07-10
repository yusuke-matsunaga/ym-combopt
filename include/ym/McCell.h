#ifndef YM_MCCELL_H
#define YM_MCCELL_H

/// @file ym/McCell.h
/// @brief McCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McCell McCell.h "ym/McCell.h"
/// @brief mincov 用の行列要素を表すクラス
//////////////////////////////////////////////////////////////////////
class McCell
{
  friend class McHead;

public:

  /// @brief コンストラクタ
  /// @param[in] row_pos 行番号
  /// @param[in] col_pos 列番号
  McCell(int row_pos,
	 int col_pos);

  /// @brief デストラクタ
  ~McCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号を返す．
  int
  row_pos() const;

  /// @brief 列番号を返す．
  int
  col_pos() const;

  /// @brief 同じ行の直前の要素を返す．
  McCell*
  row_prev() const;

  /// @brief 同じ行の直後の要素を返す．
  McCell*
  row_next() const;

  /// @brief 同じ列の直前の要素を返す．
  McCell*
  col_prev() const;

  /// @brief 同じ列の直後の要素を返す．
  McCell*
  col_next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  int mRowPos;

  // 列番号
  int mColPos;

  // 左隣の要素
  McCell* mLeftLink;

  // 右隣の要素
  McCell* mRightLink;

  // 上の要素
  McCell* mUpLink;

  // 下の要素
  McCell* mDownLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] row_pos 行番号
// @param[in] col_pos 列番号
inline
McCell::McCell(int row_pos,
	       int col_pos) :
  mRowPos(row_pos),
  mColPos(col_pos)
{
}

// @brief デストラクタ
inline
McCell::~McCell()
{
}

// @brief 行番号を返す．
inline
int
McCell::row_pos() const
{
  return mRowPos;
}

// @brief 列番号を返す．
inline
int
McCell::col_pos() const
{
  return mColPos;
}

// @brief 同じ行の直前の要素を返す．
inline
McCell*
McCell::row_prev() const
{
  return mLeftLink;
}

// @brief 同じ行の直後の要素を返す．
inline
McCell*
McCell::row_next() const
{
  return mRightLink;
}

// @brief 同じ列の直前の要素を返す．
inline
McCell*
McCell::col_prev() const
{
  return mUpLink;
}

// @brief 同じ列の直後の要素を返す．
inline
McCell*
McCell::col_next() const
{
  return mDownLink;
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCCELL_H
