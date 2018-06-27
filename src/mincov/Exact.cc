
/// @file Exact.cc
/// @brief Exact の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "Exact.h"
#include "mincov/McMatrix.h"
#include "mincov/McBlock.h"
#include "mincov/McHead.h"
#include "mincov/McCell.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

static
int solve_id = 0;

// 2つの行列が等しいかをチェックする関数
// 等しくなければ例外を送出する．
void
verify_block(McBlock& a,
	     McBlock& b)
{
  ASSERT_COND( a.row_size() == b.row_size() );
  ASSERT_COND( a.col_size() == b.col_size() );
  ASSERT_COND( a.row_head_list() == b.row_head_list() );
  ASSERT_COND( a.col_head_list() == b.col_head_list() );

  for ( int row_pos: Range(a.row_size()) ) {
    auto row_a = a.row_head(row_pos);
    auto row_b = b.row_head(row_pos);

    ASSERT_COND( row_a->row_list() == row_b->row_list() );
  }

  for ( int col_pos: Range(a.col_size()) ) {
    auto col_a = a.col_head(col_pos);
    auto col_b = b.col_head(col_pos);

    ASSERT_COND( col_a->col_list() == col_b->col_list() );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Exact
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] matrix 問題の行列
// @param[in] lb_calc_list 下界の計算クラスのリスト
// @param[in] selector 列を選択するクラス
Exact::Exact(McMatrix& matrix,
	     const vector<LbCalc*>& lb_calc_list,
	     Selector& selector) :
  mBlock(matrix),
  mLbCalcList(lb_calc_list),
  mSelector(selector)
{
}

// @brief コンストラクタ
// @param[in] matrix 問題の行列
// @param[in] row_list 注目する行番号のリスト
// @param[in] col_list 注目する列番号のリスト
// @param[in] lb_calc_list 下界の計算クラスのリスト
// @param[in] selector 列を選択するクラス
Exact::Exact(McMatrix& matrix,
	     const vector<int>& row_list,
	     const vector<int>& col_list,
	     const vector<LbCalc*>& lb_calc_list,
	     Selector& selector) :
  mBlock(matrix, row_list, col_list),
  mLbCalcList(lb_calc_list),
  mSelector(selector)
{
}

// @brief デストラクタ
Exact::~Exact()
{
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
int
Exact::solve(vector<int>& solution)
{
  // こちらは McMatrix 自身が持つ復元機能
  mBlock.save();

  solve_id = 0;

  mBest = INT_MAX;
  mCurSolution.clear();
  bool stat = _solve(0, 0);
  ASSERT_COND( stat );

  solution = mBestSolution;

  // 復元が正しいかチェックする．
  mBlock.restore();

  // solution がカバーになっているかチェックする．
  ASSERT_COND( mBlock.verify(solution) );

  if ( mDebug ) {
    cout << "Total branch: " << solve_id << endl;
  }

  return mBest;
}

// @brief 解を求める再帰関数
bool
Exact::_solve(int lb,
	      int depth)
{
  int cur_id = solve_id;
  ++ solve_id;

  mBlock.reduce(mCurSolution);

  int tmp_cost = mBlock.cost(mCurSolution);

  for ( auto lb_calc_p: mLbCalcList ) {
    int tmp_lb = (*lb_calc_p)(mBlock) + tmp_cost;
    if ( lb < tmp_lb ) {
      lb = tmp_lb;
    }
  }

  bool cur_debug = mDebug;
  if ( depth > mMaxDepth ) {
    cur_debug = false;
  }

  if ( cur_debug ) {
    int nr = mBlock.row_num();
    int nc = mBlock.col_num();
    cout << "[" << depth << "] " << nr << "x" << nc
	 << " sel=" << tmp_cost << " bnd=" << mBest
	 << " lb=" << lb
	 << endl;
  }

  if ( lb >= mBest ) {
    if ( cur_debug ) {
      cout << " bounded" << endl;
    }
    return false;
  }

  if ( mBlock.row_num() == 0 ) {
    mBest = tmp_cost;
    mBestSolution = mCurSolution;
    if ( cur_debug ) {
      cout << " best" << endl;
    }
    return true;
  }

#if 0
  vector<int> row_list1;
  vector<int> row_list2;
  vector<int> col_list1;
  vector<int> col_list2;
  if ( mDoPartition && mBlock.block_partition(row_list1, row_list2, col_list1, col_list2) ) {
    // ブロック分割を行う．
    Exact solver1(mBlock, row_list1, col_list1, mLbCalcList, mSelector);
    Exact solver2(mBlock, row_list2, col_list2, mLbCalcList, mSelector);
    if ( cur_debug ) {
      cout << endl
	   << "BLOCK PARTITION" << endl;
      cout << "Matrix#1" << endl;
      solver1.matrix().print(cout);
      cout << "Matrix#2" << endl;
      solver2.matrix().print(cout);
    }
    solver1.mBlock.save();
    solver2.mBlock.save();
    int cost_so_far = mBlock.cost(mCurSolution);
    int lb_rest = 0;
    for ( auto lb_calc_p: mLbCalcList ) {
      int tmp_lb = (*lb_calc_p)(solver2.matrix());
      if ( lb_rest < tmp_lb ) {
	lb_rest = tmp_lb;
      }
    }
    solver1.mBest = mBest - cost_so_far - lb_rest;
    solver1.mCurSolution.clear();
    bool stat1 = solver1._solve(0, depth + 1);
    if ( stat1 ) {
      mCurSolution.insert(mCurSolution.end(),
			  solver1.mBestSolution.begin(), solver1.mBestSolution.end());
      cost_so_far += solver1.mBest;
      solver2.mBest = mBest - cost_so_far;
      solver2.mCurSolution.clear();
      stat1 = solver2._solve(0, depth + 1);
      if ( stat1 ) {
	mCurSolution.insert(mCurSolution.end(),
			    solver2.mBestSolution.begin(), solver2.mBestSolution.end());
	cost_so_far += solver2.mBest;
      }
    }
    solver1.mBlock.restore();
    solver2.mBlock.restore();
    mBlock.merge(solver1.mBlock, solver2.mBlock);

    if ( stat1 ) {
      ASSERT_COND( mBlock.verify(mCurSolution) );
      if ( mBest > cost_so_far ) {
	mBest = cost_so_far;
	mBestSolution = mCurSolution;
	return true;
      }
    }
    return false;
  }
#endif

  // 次の分岐のための列をとってくる．
  int col = mSelector(mBlock);

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(mBlock);
  vector<int> orig_solution(mCurSolution);
#endif

  int cur_n = mCurSolution.size();
  mBlock.save();

  // その列を選択したときの最良解を求める．
  mBlock.select_col(col);
  mCurSolution.push_back(col);

  if ( cur_debug ) {
    cout << "[" << depth << "]A select column#" << col << endl;
  }

  bool stat1 = _solve(lb, depth + 1);

  mBlock.restore();
  int c = mCurSolution.size() - cur_n;
  for ( int i = 0; i < c; ++ i ) {
    mCurSolution.pop_back();
  }

#if defined(VERIFYY_MINCOV)
  verify_matrix(orig_matrix, mBlock);
  ASSERT_COND( orig_solution == mCurSlution );
#endif

  // 今得た最良解が下界と等しかったら探索を続ける必要はない．
  if ( lb >= mBest ) {
    if ( cur_debug ) {
      cout << "[" << depth << "]C bounded" << endl;
    }
    return true;
  }

  // その列を選択しなかったときの最良解を求める．
  mBlock.deselect_col(col);

  if ( cur_debug ) {
    cout << "[" << depth << "]B deselect column#" << col << endl;
  }

  bool stat2 = _solve(lb, depth + 1);

  return stat1 || stat2;
}

// @brief 対象のブロックを返す．
const McBlock&
Exact::block() const
{
  return mBlock;
}

// @brief partition フラグを設定する．
void
Exact::set_partition_flag(bool flag)
{
  mDoPartition = flag;
}

// @brief デバッグフラグを設定する．
void
Exact::set_debug_flag(bool flag)
{
  mDebug = flag;
}

// @brief mMaxDepth を設定する．
void
Exact::set_max_depth(int depth)
{
  mMaxDepth = depth;
}

bool
Exact::mDoPartition = true;

bool
Exact::mDebug = false;

int
Exact::mMaxDepth = 0;

END_NAMESPACE_YM_MINCOV
