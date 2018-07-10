
/// @file Greedy.cc
/// @brief Greedy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Greedy.h"
#include "ym/McBlock.h"
#include "mincov/Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Greedy
//////////////////////////////////////////////////////////////////////

// @grief greedy アルゴリズムで解を求める．
// @param[in] block 対象の行列
// @param[in] selector 列を選ぶ関数オブジェクト
// @param[out] solution 選ばれた列集合
void
Greedy::solve(McBlock& block,
	      Selector& selector,
	      vector<int>& solution)
{
  if ( mDebug ) {
    cout << "Greedy::solve() start" << endl;
    block.print(cout);
  }

  while ( block.row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    int col = selector(block);
    if ( mDebug ) {
      cout << " selecting Col#" << col << endl;
    }

    // その列を選択する．
    block.select_col(col);
    solution.push_back(col);

    if ( mDebug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }

    // 行列を縮約する．
    block.reduce(solution);

    if ( mDebug ) {
      cout << "After reduction" << endl;
      block.print(cout);
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
