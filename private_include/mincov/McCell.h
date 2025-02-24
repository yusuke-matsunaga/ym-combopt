﻿#ifndef YM_MCCELL_H
#define YM_MCCELL_H

/// @file mincov/McCell.h
/// @brief McCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McCell McCell.h "mincov/McCell.h"
/// @brief mincov 用の行列要素を表すクラス
//////////////////////////////////////////////////////////////////////
class McCell
{
  friend class McMatrix;

public:

  /// @brief コンストラクタ
  McCell(
    SizeType row_pos, ///< [in] 行番号
    SizeType col_pos  ///< [in] 列番号
  ) : mRowPos{row_pos},
      mColPos{col_pos}
  {
    mLeftLink = this;
    mRightLink = this;
    mUpLink = this;
    mDownLink = this;
  }

  /// @brief デストラクタ
  ~McCell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号を返す．
  SizeType
  row_pos() const
  {
    return mRowPos;
  }

  /// @brief 列番号を返す．
  SizeType
  col_pos() const
  {
    return mColPos;
  }

  /// @brief 同じ行の直前の要素を返す．
  McCell*
  row_prev() const
  {
    return mLeftLink;
  }

  /// @brief 同じ行の直後の要素を返す．
  McCell*
  row_next() const
  {
    return mRightLink;
  }

  /// @brief 同じ列の直前の要素を返す．
  McCell*
  col_prev() const
  {
    return mUpLink;
  }

  /// @brief 同じ列の直後の要素を返す．
  McCell*
  col_next() const
  {
    return mDownLink;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  SizeType mRowPos;

  // 列番号
  SizeType mColPos;

  // 左隣の要素
  McCell* mLeftLink;

  // 右隣の要素
  McCell* mRightLink;

  // 上の要素
  McCell* mUpLink;

  // 下の要素
  McCell* mDownLink;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MCCELL_H
