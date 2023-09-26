#ifndef SELECTOR_H
#define SELECTOR_H

/// @file Selector.h
/// @brief Selector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "mincov_nsdef.h"
#include "ym/json.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Selector Selector.h "Selector.h"
/// @brief 列を選ぶファンクタ
//////////////////////////////////////////////////////////////////////
class Selector
{
public:

  /// @brief インスタンスを生成するクラスメソッド
  static
  unique_ptr<Selector>
  new_obj(
    const JsonValue& option ///< [in] オプションを表すJSONオブジェクト
  );

  /// @brief デストラクタ
  virtual
  ~Selector() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の列を選ぶ．
  /// @return 選ばれた列番号を返す．
  virtual
  SizeType
  select(
    const McMatrix& matrix ///< [in] 対象の行列
  ) = 0;

};

END_NAMESPACE_YM_MINCOV


#endif // SELECTOR_H
