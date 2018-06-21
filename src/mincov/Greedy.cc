
/// @file Greedy.cc
/// @brief Greedy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Greedy.h"
#include "McMatrix.h"
#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Greedy
//////////////////////////////////////////////////////////////////////

// @grief greedy アルゴリズムで解を求める．
// @param[in] matrix 対象の行列
// @param[in] selector 列を選ぶ関数オブジェクト
// @param[out] solution 選ばれた列集合
int
Greedy::solve(const McMatrix& matrix,
	      Selector& selector,
	      vector<int>& solution)
{
  if ( mDebug ) {
    cout << "Greedy::solve() start" << endl;
  }

  // 作業用に行列のコピーを作る．
  McMatrix cur_matrix(matrix);

  for ( ; ; ) {
    cur_matrix.reduce(solution);

    if ( cur_matrix.row_num() == 0 ) {
      break;
    }

    // 次の分岐のための列をとってくる．
    int col = selector(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    solution.push_back(col);

    if ( mDebug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }
  }

  return matrix.cost(solution);
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
