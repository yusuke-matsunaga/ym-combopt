
/// @file Exact.cc
/// @brief Exact の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Exact.h"
#include "ym/McMatrix.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

static
int solve_id = 0;

// 2つの行列が等しいかをチェックする関数
// 等しくなければ例外を送出する．
void
verify_block(
  McMatrix& a,
  McMatrix& b
)
{
  ASSERT_COND( a.row_size() == b.row_size() );
  ASSERT_COND( a.col_size() == b.col_size() );
  ASSERT_COND( a.row_head_list() == b.row_head_list() );
  ASSERT_COND( a.col_head_list() == b.col_head_list() );

  for ( int row_pos: Range(a.row_size()) ) {
    ASSERT_COND( a.row_list(row_pos) == b.row_list(row_pos) );
  }

  for ( int col_pos: Range(a.col_size()) ) {
    ASSERT_COND( a.col_list(col_pos) == b.col_list(col_pos) );
  }
}

//////////////////////////////////////////////////////////////////////
// クラス Exact
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Exact::Exact(
  McMatrix& matrix,
  const vector<LbCalc*>& lb_calc_list,
  Selector& selector
) : mMatrix(matrix),
    mLbCalcList(lb_calc_list),
    mSelector(selector)
{
}

// @brief デストラクタ
Exact::~Exact()
{
}

// @brief 最小被覆問題を解く．
int
Exact::solve(
  vector<SizeType>& solution
)
{
  solve_id = 0;

  mBest = INT_MAX;
  mCurSolution.clear();
  bool stat = _solve(0, 0);
  ASSERT_COND( stat );

  solution = mBestSolution;

  if ( mDebug ) {
    cout << "Total branch: " << solve_id << endl;
  }

  return mBest;
}

// @brief 解を求める再帰関数
bool
Exact::_solve(
  int lb,
  int depth
)
{
  int cur_id = solve_id;
  ++ solve_id;

  vector<SizeType> dummy;
  mMatrix.reduce_loop(mCurSolution, dummy);

  int tmp_cost = mMatrix.cost(mCurSolution);

  for ( auto lb_calc_p: mLbCalcList ) {
    int tmp_lb = (*lb_calc_p)(mMatrix) + tmp_cost;
    if ( lb < tmp_lb ) {
      lb = tmp_lb;
    }
  }

  bool cur_debug = mDebug;
  if ( depth > mMaxDepth ) {
    cur_debug = false;
  }

  if ( cur_debug ) {
    int nr = mMatrix.active_row_num();
    int nc = mMatrix.active_col_num();
    cout << "[" << depth << "] " << nr << "x" << nc
	 << " sel=" << tmp_cost << " bnd=" << mBest
	 << " lb=" << lb
	 << endl;
  }

  if ( lb >= mBest ) {
    // 下界を用いた枝刈り
    if ( cur_debug ) {
      cout << " bounded" << endl;
    }
    return false;
  }

  if ( mMatrix.active_row_num() == 0 ) {
    // 自明な解
    mBest = tmp_cost;
    mBestSolution = mCurSolution;
    if ( cur_debug ) {
      cout << " best" << endl;
    }
    return true;
  }

#if 0
  McBlock block1(mBlock.matrix());
  if ( mDoPartition && mBlock.partition(block1) ) {
    // ブロック分割を行う．
    Exact solver1(block1, mLbCalcList, mSelector);
    Exact solver2(block2, mLbCalcList, mSelector);
    if ( cur_debug ) {
      cout << endl
	   << "BLOCK PARTITION" << endl;
      cout << "Matrix#1" << endl;
      solver1.block().print(cout);
      cout << "Matrix#2" << endl;
      solver2.block().print(cout);
    }

    block1.save();
    block2.save();
    int cost_so_far = mBlock.cost(mCurSolution);
    int lb_rest = 0;
    for ( auto lb_calc_p: mLbCalcList ) {
      int tmp_lb = (*lb_calc_p)(solver1.block());
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
    block1.restore();
    block2.restore();
    mBlock.merge(block1, block2);

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
  SizeType col = mSelector(mMatrix);

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(mMatrix);
  vector<int> orig_solution(mCurSolution);
#endif

  SizeType cur_n = mCurSolution.size();
  mMatrix.save();

  // その列を選択したときの最良解を求める．
  mMatrix.select_col(col);
  mCurSolution.push_back(col);

  if ( cur_debug ) {
    cout << "[" << depth << "]A select column#" << col << endl;
  }

  bool stat1 = _solve(lb, depth + 1);

  mMatrix.restore();
  SizeType c = mCurSolution.size() - cur_n;
  for ( SizeType i = 0; i < c; ++ i ) {
    mCurSolution.pop_back();
  }

#if defined(VERIFYY_MINCOV)
  verify_matrix(orig_matrix, mMatrix);
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
  mMatrix.delete_col(col);

  if ( cur_debug ) {
    cout << "[" << depth << "]B deselect column#" << col << endl;
  }

  bool stat2 = _solve(lb, depth + 1);

  return stat1 || stat2;
}

// @brief 対象のブロックを返す．
const McMatrix&
Exact::matrix() const
{
  return mMatrix;
}

// @brief partition フラグを設定する．
void
Exact::set_partition_flag(
  bool flag
)
{
  mDoPartition = flag;
}

// @brief デバッグフラグを設定する．
void
Exact::set_debug_flag(
  bool flag
)
{
  mDebug = flag;
}

// @brief mMaxDepth を設定する．
void
Exact::set_max_depth(
  int depth
)
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
