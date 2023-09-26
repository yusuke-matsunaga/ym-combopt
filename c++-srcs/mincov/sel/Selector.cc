
/// @file Selector.cc
/// @brief Selector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/Selector.h"
#include "SelSimple.h"
#include "SelNaive.h"
#include "SelCS.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_MINCOV

// @brief インスタンスを生成するクラスメソッド
unique_ptr<Selector>
Selector::new_obj(
  const JsonValue& option
)
{
  string type_str{"simple"}; // デフォルトフォールバック
  if ( option.has_key("type") ) {
    auto type_val = option.at("type");
    if ( !type_val.is_string() ) {
      throw std::invalid_argument{"selector.type should be a string"};
    }
    type_str = type_val.get_string();
  }
  if ( type_str == "simple" ) {
    return unique_ptr<Selector>{new SelSimple};
  }
  if ( type_str == "naive" ) {
    return unique_ptr<Selector>{new SelNaive};
  }
  if ( type_str == "cs" ) {
    return unique_ptr<Selector>{new SelCS};
  }
  ostringstream buf;
  buf << type_str << ": unknown value for selector.type";
  throw std::invalid_argument{buf.str()};
  return nullptr;
}

END_NAMESPACE_YM_MINCOV
