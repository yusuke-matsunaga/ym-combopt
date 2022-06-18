#ifndef YM_MCCOLCOMP_H
#define YM_MCCOLCOMP_H

/// @file ym/McColComp.h
/// @brief McColComp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McColComp McColComp.h "McColComp.h"
/// @brief McMatrix::reduce で用いる列比較関数
//////////////////////////////////////////////////////////////////////
class McColComp
{
public:

  /// @brief デストラクタ
  virtual
  ~McColComp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief col1 の代わりに col2 を使っても全体のコストが上がらない時に true を返す．
  virtual
  bool
  operator()(
    SizeType col1, ///< [in] 列番号1
    SizeType col2  ///< [in] 列番号2
  ) const;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MCCOLCOMP_H
