
/// @file Solver.cc
/// @brief Solver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Solver.h"
#include "Exact.h"
#include "Greedy.h"
#include "mincov/McMatrix.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"
#include "ym/JsonValue.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

BEGIN_NONAMESPACE

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Solver
//////////////////////////////////////////////////////////////////////

// @brief オプションを解析してインスタンスを生成する関数
unique_ptr<Solver>
Solver::new_obj(
  McMatrix& matrix,
  const JsonValue& opt_obj
)
{
  string algorithm{"greedy"};
  if ( opt_obj.has_key("algorithm") ) {
    algorithm = get_string(opt_obj["algorithm"], "algorithm");
  }

  if ( algorithm == "exact" ) {
    return unique_ptr<Solver>{new Exact{matrix, opt_obj}};
  }
  if ( algorithm == "greedy" ) {
    return unique_ptr<Solver>{new Greedy{matrix, opt_obj}};
  }
  {
    ostringstream buf;
    buf << algorithm << ": unknown algorithm";
    throw std::invalid_argument{buf.str()};
  }
  return nullptr;
}

// @brief コンストラクタ
Solver::Solver(
  McMatrix& matrix,
  const JsonValue& opt_obj
) : mMatrix{matrix},
    mDebug{get_int(opt_obj, "debug")},
    mDebugDepth{get_int(opt_obj, "debug_depth")}
{
}

// @brief デストラクタ
Solver::~Solver()
{
}

std::unique_ptr<Selector>
Solver::new_Selector(
  const JsonValue& opt_obj
)
{
  JsonValue sel_opt;
  if ( opt_obj.has_key("selector") ) {
    sel_opt = opt_obj.at("selector");
    if ( !sel_opt.is_object() ) {
      throw std::invalid_argument{"selector should be a JSON-object"};
    }
  }
  return Selector::new_obj(sel_opt);
}

std::unique_ptr<LbCalc>
Solver::new_LbCalc(
  const JsonValue& opt_obj
)
{
  JsonValue lb_opt;
  if ( opt_obj.has_key("lower_bound") ) {
    lb_opt = opt_obj.at("lower_bound");
  }
  return LbCalc::new_obj(lb_opt);
}

bool
Solver::get_bool(
  const JsonValue& opt_obj,
  const string& keyword,
  bool default_val
)
{
  if ( opt_obj.has_key(keyword) ) {
    auto value = opt_obj.at(keyword);
    if ( !value.is_bool() ) {
      ostringstream buf;
      buf << keyword << " should be boolean";
      throw std::invalid_argument{buf.str()};
    }
    return value.get_bool();
  }
  return default_val;
}

int
Solver::get_int(
  const JsonValue& opt_obj,
  const string& keyword,
  int default_val
)
{
  if ( opt_obj.has_key(keyword) ) {
    auto value = opt_obj.at(keyword);
    if ( !value.is_int() ) {
      ostringstream buf;
      buf << keyword << " should be an integer";
      throw std::invalid_argument{buf.str()};
    }
    return value.get_int();
  }
  return default_val;
}

string
Solver::get_string(
  const JsonValue& opt_obj,
  const string& keyword,
  const string& default_val
)
{
  if ( opt_obj.has_key(keyword) ) {
    auto value = opt_obj.at(keyword);
    if ( !value.is_string() ) {
      ostringstream buf;
      buf << keyword << " should be a string";
      throw std::invalid_argument{buf.str()};
    }
    return value.get_string();
  }
  return default_val;
}

END_NAMESPACE_YM_MINCOV
