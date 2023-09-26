
/// @file Greedy.cc
/// @brief Greedy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Greedy.h"
#include "mincov/Selector.h"
#include "mincov/McMatrix.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Greedy
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Greedy::Greedy(
  McMatrix& matrix,
  const JsonValue& opt_obj
) : Solver{matrix, opt_obj}
{
}

// @grief greedy アルゴリズムで解を求める．
SizeType
Greedy::solve(
  vector<SizeType>& solution
)
{
  if ( debug() ) {
    cout << "Greedy::solve() start" << endl;
    matrix().print(cout);
  }

  while ( matrix().active_row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    auto col = select();
    if ( debug() ) {
      cout << " selecting Col#" << col << endl;
    }

    // その列を選択する．
    matrix().select_col(col);
    solution.push_back(col);

    if ( debug() ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }

    // 行列を縮約する．
    vector<SizeType> dummy;
    matrix().reduce_loop(solution, dummy);

    if ( debug() ) {
      cout << "After reduction" << endl;
      matrix().print(cout);
    }
  }

  return matrix().cost(solution);
}

END_NAMESPACE_YM_MINCOV
