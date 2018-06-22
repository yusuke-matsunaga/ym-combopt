
/// @file MinCov.cc
/// @brief MinCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MinCov.h"
#include "Greedy.h"
#include "Exact.h"
#include "LbMIS1.h"
#include "LbMIS2.h"
#include "LbMIS3.h"
#include "LbCS.h"
#include "SelSimple.h"
#include "SelNaive.h"
#include "SelCS.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

BEGIN_NONAMESPACE

// オプション文字列をパーズする
// * オプションは
//   <オプションキーワード>[:<オプション値>][, <オプションキーワード>[:<オプション値>]]
//   の形式と仮定する．
// * 空白の除去は行わない．単純に ',' と ':' のみで区切る．
// * 結果を opt_list に格納する．
void
parse_option(const string& option_str,
	     vector<pair<string, string> >& opt_list)
{
  // ',' で区切る
  string tmp_str(option_str);
  for ( ; ; ) {
    string::size_type p = tmp_str.find_first_of(',');
    string tmp = tmp_str.substr(0, p);
    // tmp を ':' で区切る．
    string::size_type q = tmp.find_first_of(':');
    if ( q == string::npos ) {
      // ':' がなかった
      opt_list.push_back(make_pair(tmp, string()));
    }
    else {
      string l_str = tmp.substr(0, q);
      string r_str = tmp.substr(q + 1, string::npos);
      opt_list.push_back(make_pair(l_str, r_str));
    }
    if ( p == string::npos ) {
      // 末尾だったので終わる．
      break;
    }
    // tmp_str を切り詰める．
    tmp_str = tmp_str.substr(p + 1, string::npos);
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス MinCov
//////////////////////////////////////////////////////////////////////

// @brief コスト配列を初期化する．
void
MinCov::init_cost_array()
{
  mColCostArray = new int[mColSize];
  for ( int i: Range(mColSize) ) {
    mColCostArray[i] = 1;
  }
}

// @brief コスト配列を初期化する．
void
MinCov::init_cost_array(const vector<int>& col_cost_array)
{
  ASSERT_COND( col_cost_array.size() == mColSize );

  mColCostArray = new int[mColSize];
  for ( int i: Range(mColSize) ) {
    mColCostArray[i] = col_cost_array[i];
  }
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @param[in] option オプション文字列
// @return 解のコスト
int
MinCov::exact(vector<int>& solution,
	      const string& option)
{
  // ここの仕事はオプション文字列を解析して
  // Exact を呼ぶこと．
  vector<pair<string, string> > opt_list;
  parse_option(option, opt_list);

  vector<string> lb_str_list;
  string sel_str;
  for ( auto opt_pair: opt_list ) {
    auto opt_kwd = opt_pair.first;
    auto opt_val = opt_pair.second;
    if ( opt_kwd == string("lower_bound") ) {
      lb_str_list.push_back(opt_val);
    }
    else if ( opt_kwd == string("selector") ) {
      // 重複していたら最後の指定が有効となる．
      sel_str = opt_val;
    }
    else if ( opt_kwd == string("partition") ) {
      if ( opt_val == string() || opt_val == string("on") ) {
	Exact::set_partition_flag(true);
      }
      else if ( opt_val == string("off") ) {
	Exact::set_partition_flag(false);
      }
      else {
	cerr << "Error in MinCov::exact(): illegal value for 'partition': "
	     << opt_val << ", ignored." << endl;
	// 無視
      }
    }
    else if ( opt_kwd == string("debug") ) {
      if ( opt_val == string() || opt_val == string("on") ) {
	Exact::set_debug_flag(true);
      }
      else if ( opt_val == string("off") ) {
	Exact::set_debug_flag(false);
      }
      else {
	cerr << "Error in MinCov::exact(): illegal value for 'debug': "
	     << opt_val << ", ignored." << endl;
	// 無視
      }
    }
    else if ( opt_kwd == string("debug_depth") ) {
      int debug_depth = atoi(opt_val.c_str());
      Exact::set_max_depth(debug_depth);
    }
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

  SelSimple sel_simple;
  SelNaive sel_naive;
  SelCS sel_cs;
  Selector* selector = nullptr;
  if ( sel_str == string("simple") ) {
    selector = &sel_simple;
  }
  else if ( sel_str == string("naive") ) {
    selector = &sel_naive;
  }
  else if ( sel_str == string("cs") ) {
    selector = &sel_cs;
  }
  else {
    cerr << "Error in MinCov::exact(): illegal value for 'selector': "
	 << sel_str << ", 'SelSimple' is used." << endl;
    selector = &sel_simple;
  }

  McMatrix matrix(row_size(), col_size(), mColCostArray, mElemList);

  Exact solver(matrix, lb_list, *selector);

  return solver.solve(solution);
}

// @brief ヒューリスティックで最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @param[in] algorithm ヒューリスティックの名前
// @param[in] option オプション文字列
// @return 解のコスト
int
MinCov::heuristic(vector<int>& solution,
		  const string& algorithm,
		  const string& option)
{
  if ( algorithm == string("greedy") ) {
    return greedy(option, solution);
  }
  else if ( algorithm == string("random") ) {
  }
  else {
    return greedy(string(), solution);
  }
  return 0;
}

// @brief greedy アルゴリズムで解を求める．
// @param[in] option オプション文字列
// @param[out] solution 解
// @return 解のコスト
int
MinCov::greedy(const string& option,
	       vector<int>& solution)
{
  // ここの仕事はオプション文字列を解析して
  // Greedy を呼ぶこと．
  vector<pair<string, string> > opt_list;
  parse_option(option, opt_list);

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
    cerr << "Error in MinCov::exact(): illegal value for 'selector': "
	 << sel_str << ", 'SelSimple' is used." << endl;
    selector = &sel_simple;
  }

  McMatrix matrix(row_size(), col_size(), mColCostArray, mElemList);

  return Greedy::solve(matrix, *selector, solution);
}

END_NAMESPACE_YM_MINCOV
