
/// @file Exact.cc
/// @brief Exact の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "Exact.h"
#include "McMatrix.h"
#include "McRowHead.h"
#include "McColHead.h"
#include "McCell.h"
#include "LbCalc.h"
#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

static
int solve_id = 0;

// 2つの行列が等しいかをチェックする関数
// 等しくなければ例外を送出する．
void
verify_matrix(McMatrix& a,
	      McMatrix& b)
{
  ASSERT_COND( a.row_num() == b.row_num() );
  ASSERT_COND( a.col_num() == b.col_num() );

  const McRowHead* row_a = a.row_front();
  const McRowHead* row_b = b.row_front();
  for ( ; ; ) {
    ASSERT_COND( row_a->pos() == row_b->pos() );
    ASSERT_COND( row_a->num() == row_b->num() );

    const McCell* cell_a = row_a->front();
    const McCell* cell_b = row_b->front();
    for ( ; ; ) {
      ASSERT_COND( cell_a->row_pos() == row_a->pos() );
      ASSERT_COND( cell_b->row_pos() == row_b->pos() );
      ASSERT_COND( cell_a->col_pos() == cell_b->col_pos() );
      cell_a = cell_a->row_next();
      cell_b = cell_b->row_next();
      if ( row_a->is_end(cell_a) ) {
	ASSERT_COND( row_b->is_end(cell_b) );
	break;
      }
    }
    row_a = row_a->next();
    row_b = row_b->next();
    if ( a.is_row_end(row_a) ) {
      ASSERT_COND( b.is_row_end(row_b) );
      break;
    }
  }
  const McColHead* col_a = a.col_front();
  const McColHead* col_b = b.col_front();
  for ( ; ; ) {
    ASSERT_COND( col_a->pos() == col_b->pos() );
    ASSERT_COND( col_a->num() == col_b->num() );
    const McCell* cell_a = col_a->front();
    const McCell* cell_b = col_b->front();
    for ( ; ; ) {
      ASSERT_COND( cell_a->col_pos() == col_a->pos() );
      ASSERT_COND( cell_b->col_pos() == col_b->pos() );
      ASSERT_COND( cell_a->row_pos() == cell_b->row_pos() );

      cell_a = cell_a->col_next();
      cell_b = cell_b->col_next();
      if ( col_a->is_end(cell_a) ) {
	ASSERT_COND( col_b->is_end(cell_b) );
	break;
      }
    }
    col_a = col_a->next();
    col_b = col_b->next();
    if ( a.is_col_end(col_a) ) {
      ASSERT_COND( b.is_col_end(col_b) );
      break;
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Exact
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] matrix 問題の行列
// @param[in] lb_calc_list 下界の計算クラスのリスト
// @param[in] selector 列を選択するクラス
Exact::Exact(const McMatrix& matrix,
	     const vector<LbCalc*>& lb_calc_list,
	     Selector& selector) :
  mMatrix(matrix),
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
  mMatrix(matrix, row_list, col_list),
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
  // 検証用にもとの行列をコピーしておく．
  McMatrix orig_matrix(mMatrix);
  // こちらは McMatrix 自身が持つ復元機能
  mMatrix.save();

  solve_id = 0;

  mBest = UINT_MAX;
  mCurSolution.clear();
  bool stat = _solve(0, 0);
  ASSERT_COND( stat );

  solution = mBestSolution;

  // 復元が正しいかチェックする．
  mMatrix.restore();
  verify_matrix(mMatrix, orig_matrix);

  // solution がカバーになっているかチェックする．
  ASSERT_COND( mMatrix.verify(solution) );

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

  mMatrix.reduce(mCurSolution);

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
    int nr = mMatrix.row_num();
    int nc = mMatrix.col_num();
    cout << "[" << depth << "] " << nr << "x" << nc
	 << " sel=" << tmp_cost << " bnd=" << mBest
	 << " lb=" << lb;
  }

  if ( lb >= mBest ) {
    if ( cur_debug ) {
      cout << " bounded" << endl;
    }
    return false;
  }

  if ( mMatrix.row_num() == 0 ) {
    mBest = tmp_cost;
    mBestSolution = mCurSolution;
    if ( cur_debug ) {
      cout << " best" << endl;
    }
    return true;
  }

  vector<int> row_list1;
  vector<int> row_list2;
  vector<int> col_list1;
  vector<int> col_list2;
  if ( mDoPartition && mMatrix.block_partition(row_list1, row_list2, col_list1, col_list2) ) {
    // ブロック分割を行う．
    Exact solver1(mMatrix, row_list1, col_list1, mLbCalcList, mSelector);
    Exact solver2(mMatrix, row_list2, col_list2, mLbCalcList, mSelector);
    if ( cur_debug ) {
      cout << endl
	   << "BLOCK PARTITION" << endl;
      cout << "Matrix#1" << endl;
      solver1.matrix().print(cout);
      cout << "Matrix#2" << endl;
      solver2.matrix().print(cout);
    }
    solver1.mMatrix.save();
    solver2.mMatrix.save();
    int cost_so_far = mMatrix.cost(mCurSolution);
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
    solver1.mMatrix.restore();
    solver2.mMatrix.restore();
    mMatrix.merge(solver1.mMatrix, solver2.mMatrix);

    if ( stat1 ) {
      ASSERT_COND( mMatrix.verify(mCurSolution) );
      if ( mBest > cost_so_far ) {
	mBest = cost_so_far;
	mBestSolution = mCurSolution;
	return true;
      }
    }
    return false;
  }

  // 次の分岐のための列をとってくる．
  int col = mSelector(mMatrix);

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(mMatrix);
  vector<int> orig_solution(mCurSolution);
#endif

  int cur_n = mCurSolution.size();
  mMatrix.save();

  // その列を選択したときの最良解を求める．
  mMatrix.select_col(col);
  mCurSolution.push_back(col);

  if ( cur_debug ) {
    cout << " select column#" << col << endl;
  }

  bool stat1 = _solve(lb, depth + 1);

  mMatrix.restore();
  int c = mCurSolution.size() - cur_n;
  for ( int i = 0; i < c; ++ i ) {
    mCurSolution.pop_back();
  }

#if defined(VERIFYY_MINCOV)
  verify_matrix(orig_matrix, mMatrix);
  ASSERT_COND( orig_solution == mCurSlution );
#endif

  // 今得た最良解が下界と等しかったら探索を続ける必要はない．
  if ( lb >= mBest ) {
    return true;
  }

  // その列を選択しなかったときの最良解を求める．
  mMatrix.delete_col(col);

  if ( cur_debug ) {
    cout << "delete column#" << col << endl;
  }

  bool stat2 = _solve(lb, depth + 1);

  return stat1 || stat2;
}

// @brief 内部の行列を返す．
const McMatrix&
Exact::matrix() const
{
  return mMatrix;
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
