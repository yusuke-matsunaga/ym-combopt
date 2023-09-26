
/// @file MinCov.cc
/// @brief MinCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MinCov.h"
#include "ym/JsonValue.h"
#include "mincov/McMatrix.h"
#include "Solver.h"
#if 0
#include "Greedy.h"
#include "Exact.h"
#include "lb/LbMIS1.h"
#include "lb/LbMIS2.h"
#include "lb/LbMIS3.h"
#include "lb/LbCS.h"
#include "sel/SelSimple.h"
#include "sel/SelNaive.h"
#include "sel/SelCS.h"
#endif
#include "ym/Range.h"

BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MinCov
//////////////////////////////////////////////////////////////////////

// @brief 最小被覆問題を解く．
SizeType
MinCov::solve(
  vector<SizeType>& solution,
  const string& option_str
)
{
  McMatrix matrix{row_size(), mColCostArray, mElemList};
  auto option = JsonValue::parse(option_str);
  auto solver = Solver::new_obj(matrix, option);
  SizeType cost = solver->solve(solution);

  { // 結果が正しいか検証しておく．
    vector<SizeType> uncov_row_list;
    bool stat = verify_solution(solution, uncov_row_list);
    if ( !stat ) {
      for ( auto row: uncov_row_list ) {
	cout << "Error: Row#" << row << " is not covered" << endl;
      }
    }
  }

  return cost;
}

#if 0
// @brief 最小被覆問題を解く．
SizeType
MinCov::exact(
  vector<SizeType>& solution,
  const JsonValue& json_obj
)
{
  sanity_check();

  vector<string> lb_str_list;
  if ( json_obj.has_key("lower_bound") ) {
    auto value = expect_string(json_obj["lower_bound"], "lower_bound");
    lb_str_list.push_back(value);
  }

  LbCS lb_cs;
  LbMIS1 lb_mis1;
  LbMIS2 lb_mis2;
  LbMIS3 lb_mis3;
  vector<LbCalc*> lb_list;
  for ( auto lb_str: lb_str_list ) {
    if ( lb_str == string("cs") ) {
      lb_list.push_back(&lb_cs);
    }
    else if ( lb_str == string("mis1") ) {
      lb_list.push_back(&lb_mis1);
    }
    else if ( lb_str == string("mis2") ) {
      lb_list.push_back(&lb_mis2);
    }
    else if ( lb_str == string("mis3") ) {
      lb_list.push_back(&lb_mis3);
    }
    else {
      cerr << "Error in MinCov::exact(): illegal value for 'lower_bound': "
	   << lb_str << ", ignored." << endl;
    }
  }
  if ( lb_list.empty() ) {
    // 'lower_bound' オプションが指定されていない時の
    // でファオルトフォールバック
    lb_list.push_back(&lb_cs);
    lb_list.push_back(&lb_mis1);
  }

  McMatrix matrix(row_size(), mColCostArray, mElemList);
  Exact solver(matrix, json_obj);
  SizeType cost = solver.solve(solution);

  return cost;
}

BEGIN_NONAMESPACE

// @brief greedy アルゴリズムで解を求める．
void
greedy(
  McMatrix& matrix,
  const string& option,
  vector<SizeType>& solution
)
{
  string sel_str;
  for ( auto opt_pair: opt_list ) {
    auto opt_kwd = opt_pair.first;
    auto opt_val = opt_pair.second;
    if ( opt_kwd == string("selector") ) {
      // 重複していたら最後の指定が有効となる．
      sel_str = opt_val;
    }
    else if ( opt_kwd == string("debug") ) {
      if ( opt_val == string() || opt_val == string("on") ) {
	Greedy::set_debug_flag(true);
      }
      else if ( opt_val == string("off") ) {
	Greedy::set_debug_flag(false);
      }
      else {
	cerr << "Error in MinCov::greedy(): illegal value for 'debug': "
	     << opt_val << ", ignored." << endl;
	// 無視
      }
    }
  }

  SelSimple sel_simple;
  SelNaive sel_naive;
  SelCS sel_cs;
  Selector* selector = nullptr;
  if ( sel_str == string() || sel_str == string("simple") ) {
    selector = &sel_simple;
  }
  else if ( sel_str == string("naive") ) {
    selector = &sel_naive;
  }
  else if ( sel_str == string("cs") ) {
    selector = &sel_cs;
  }
  else {
    cerr << "Error in MinCov::greedy(): illegal value for 'selector': "
	 << sel_str << ", 'SelSimple' is used." << endl;
    selector = &sel_simple;
  }

  Greedy::solve(matrix, *selector, solution);
}

END_NONAMESPACE


// @brief ヒューリスティックで最小被覆問題を解く．
SizeType
MinCov::heuristic(
  vector<SizeType>& solution,
  const string& algorithm,
  const string& option)
{
  sanity_check();

  McMatrix matrix{row_size(), mColCostArray, mElemList};

  // 最初に縮約を行う．
  vector<SizeType> dummy;
  matrix.reduce_loop(solution, dummy);

  // この時点で解けていたらヒューリスティックは必要ない．
  if ( matrix.active_row_num() > 0 ) {
    vector<SizeType> solution1;
    if ( algorithm == string("greedy") ) {
      greedy(matrix, option, solution1);
    }
    else if ( algorithm == string("random") ) {
    }
    else {
      greedy(matrix, option, solution1);
    }
    solution.insert(solution.end(), solution1.begin(), solution1.end());
  }

  { // 結果が正しいか検証しておく．
    vector<SizeType> uncov_row_list;
    bool stat = verify_solution(solution, uncov_row_list);
    if ( !stat ) {
      for ( auto row: uncov_row_list ) {
	cout << "Error: Row#" << row << " is not covered" << endl;
      }
    }
  }

  return matrix.cost(solution);
}
#endif

// @brief mElemList をチェックする．
void
MinCov::sanity_check()
{
  vector<bool> row_mark(row_size(), false);
  for ( auto p: mElemList ) {
    SizeType row_pos = p.first;
    row_mark[row_pos] = true;
  }
  for ( auto row_pos: Range(row_size()) ) {
    if ( !row_mark[row_pos] ) {
      cout << "Warning: Row#" << row_pos << " has no elem" << endl;
    }
  }
}

// @brief 解を検証する．
bool
MinCov::verify_solution(
  const vector<SizeType>& solution,
  vector<SizeType>& uncov_row_list
)
{
  McMatrix matrix{row_size(), mColCostArray, mElemList};
  vector<bool> row_mark(row_size(), false);
  for ( auto col: solution ) {
    for ( auto row: matrix.col_list(col) ) {
      row_mark[row] = true;
    }
  }
  uncov_row_list.clear();
  for ( auto row: Range(row_size()) ) {
    if ( matrix.row_elem_num(row) > 0 && !row_mark[row] ) {
      uncov_row_list.push_back(row);
    }
  }
  return uncov_row_list.empty();
}

END_NAMESPACE_YM_MINCOV
