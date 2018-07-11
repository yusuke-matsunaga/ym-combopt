
/// @file Greedy.cc
/// @brief Greedy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Greedy.h"
#include "mincov/Selector.h"
#include "ym/McMatrix.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Greedy
//////////////////////////////////////////////////////////////////////

#if 0
BEGIN_NONAMESPACE

bool
col_dominance(McMatrix& matrix)
{
  bool change = false;

  // 要素を持たない列を削除する．
  for ( auto col_pos1: matrix.col_head_list() ) {
    if ( matrix.col_elem_num(col_pos1) == 0 ) {
      matrix.delete_col(col_pos1);
    }
  }

  // 削除する列番号のリスト
  vector<int> del_list;
  del_list.reserve(matrix.active_col_num());
  // 削除マーク
  vector<bool> mark(matrix.col_size(), false);
  for ( auto col_pos1: matrix.col_head_list() ) {
    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    int min_num = matrix.col_size() + 1;
    int min_row = -1;
    for ( auto row_pos: matrix.col_list(col_pos1) ) {
      int row_num = matrix.row_elem_num(row_pos);
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row_pos;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for ( auto col_pos2: matrix.row_list(min_row) ) {
      if ( col_pos2 == col_pos1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( mark[col_pos2] ) {
	// 削除済みならスキップ
	continue;
      }
      if ( matrix.col_elem_num(col_pos2) < matrix.col_elem_num(col_pos1) ) {
	// ただし col1 よりも要素数の少ない列は調べる必要はない．
	continue;
      }
      if ( matrix.col_cost(col_pos2) > matrix.col_cost(col_pos1) ) {
	// col2 のコストが col1 のコストより高ければ調べる必要はない．
	continue;
      }

      // col_head1 に含まれる要素を col_head2 がすべて含んでいる場合
      // col_head2 は col_head1 を支配している．
      if ( check_containment(matrix.col_list(col_pos2),
			     matrix.col_list(col_pos1)) ) {
	mark[col_pos1] = true;
	del_list.push_back(col_pos1);
	change = true;
	break;
      }
    }
  }

  // 実際に削除する．
  for ( auto col: del_list ) {
    matrix.delete_col(col);
  }

  return change;
}

bool
row_dominance(McMatrix& matrix)
{
  bool change = false;

  // 削除する行番号のリスト
  vector<int> del_list;
  del_list.reserve(matrix.active_row_num());
  // 削除マーク
  vector<bool> mark(matrix.row_size(), false);
  for ( auto row_pos1: matrix.row_head_list() ) {
    if ( mark[row_pos1] ) {
      // すでに削除の印がついていたらスキップ
      continue;
    }

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    int min_num = matrix.row_size() + 1;
    int min_col = -1;
    for ( auto col_pos: matrix.row_list(row_pos1) ) {
      int col_num = matrix.col_elem_num(col_pos);
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col_pos;
      }
    }
    // min_col に要素を持つ行のうち row1 に支配されている行を求める．
    for ( auto row_pos2: matrix.col_list(min_col) ) {
      if ( row_pos2 == row_pos1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( matrix.row_elem_num(row_pos2) < matrix.row_elem_num(row_pos1) ) {
	// 要素数が少ない行も比較しない．
	continue;
      }
      if ( mark[row_pos2] ) {
	// 削除された行も比較しない.
	continue;
      }

      // row1 に含まれる要素をすべて row2 が含んでいる場合
      // row1 が row2 を支配している．
      if ( check_containment(matrix.row_list(row_pos2),
			     matrix.row_list(row_pos1)) ) {
	mark[row_pos2] = true;
	del_list.push_back(row_pos2);
	change = true;
      }
    }
  }

  // 実際に削除する．
  for ( auto row: del_list ) {
    matrix.delete_row(row);
  }

  return change;
}

bool
essential_col(McMatrix& matrix,
	      vector<int>& solution)
{
  vector<bool> mark(matrix.col_size(), false);
  int old_size = solution.size();
  for ( auto row_pos1: matrix.row_head_list() ) {
    if ( matrix.row_elem_num(row_pos1) == 1 ) {
      int col_pos = matrix.row_list(row_pos1).front();
      if ( !mark[col_pos] ) {
	mark[col_pos] = true;
	solution.push_back(col_pos);
      }
    }
  }
  int size = solution.size();
  for ( auto i: Range(old_size, size) ) {
    int col_pos = solution[i];
    for ( auto row: matrix.col_list(col_pos) ) {
      matrix.delete_row(row);
    }
    matrix.delete_col(col_pos);
  }

  return size > old_size;
}

void
reduce(McMatrix& matrix,
       vector<int>& solution)
{

  int no_change = 0;
  for ( ; ; ) {
    // 列支配を探し，列の削除を行う．
    if ( col_dominance(matrix) ) {
      no_change = 0;
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }

    // 必須列を探し，列の選択を行う．
    if ( essential_col(matrix, solution) ) {
      no_change = 0;
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }

    // 行支配を探し，行の削除を行う．
    if ( row_dominance(matrix) ) {
      no_change = 0;
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }
  }
}

END_NONAMESPACE
#endif

// @grief greedy アルゴリズムで解を求める．
// @param[in] matrix 対象の行列
// @param[in] selector 列を選ぶ関数オブジェクト
// @param[out] solution 選ばれた列集合
void
Greedy::solve(McMatrix& matrix,
	      Selector& selector,
	      vector<int>& solution)
{
  if ( mDebug ) {
    cout << "Greedy::solve() start" << endl;
    matrix.print(cout);
  }

  while ( matrix.active_row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    int col = selector(matrix);
    if ( mDebug ) {
      cout << " selecting Col#" << col << endl;
    }

    // その列を選択する．
    matrix.select_col(col);
    solution.push_back(col);

    if ( mDebug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }

    // 行列を縮約する．
    matrix.reduce(solution);

    if ( mDebug ) {
      cout << "After reduction" << endl;
      matrix.print(cout);
    }
  }
}

// @brief デバッグフラグをセットする．
// @param[in] flag セットする値
void
Greedy::set_debug_flag(bool flag)
{
  mDebug = flag;
}

bool Greedy::mDebug = false;

END_NAMESPACE_YM_MINCOV
