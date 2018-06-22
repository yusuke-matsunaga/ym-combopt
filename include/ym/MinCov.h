#ifndef YM_MINCOV_H
#define YM_MINCOV_H

/// @file ym/MinCov.h
/// @brief MinCov のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MinCov MinCov.h "ym/MinCov.h"
/// @brief 最小被覆問題を表すクラス
//////////////////////////////////////////////////////////////////////
class MinCov
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  MinCov();

  /// @brief サイズを指定したコンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  ///
  /// * 空の行列となる．
  /// * 列のコストはすべて1になる．
  MinCov(int row_size,
	 int col_size);

  /// @brief コスト配列をしていしたコンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_cost_array 列のコストの配列
  ///
  /// * 列のサイズは col_cost_array のサイズから得られる．
  /// * 空の行列となる．
  MinCov(int row_size,
	 const vector<int>& col_cost_array);

  /// @brief コピーコンストラクタ
  MinCov(const MinCov& src) = default;

  /// @brief ムーブコンストラクタ
  MinCov(MinCov&& src) = default;

  /// @brief コピー代入演算子
  MinCov&
  operator=(const MinCov& src) = default;

  /// @brief ムーブ代入演算子
  MinCov&
  operator=(MinCov&& src) = default;

  /// @brief デストラクタ
  ~MinCov();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題のサイズを設定する．
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  ///
  /// * 空の行列となる．
  /// * 列のコストはすべて1になる．
  void
  resize(int row_size,
	 int col_size);

  /// @brief 問題のサイズとコスト配列を設定する．
  /// @param[in] row_size 行数
  /// @param[in] col_cost_array 列のコスト配列
  ///
  /// * 列のサイズは col_cost_array のサイズから得られる．
  /// * 空の行列となる．
  void
  resize(int row_size,
	 const vector<int>& col_cost_array);

  /// @brief 列のコストを設定する
  /// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size() )
  /// @param[in] cost コスト
  void
  set_col_cost(int col_pos,
	       int cost);

  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  ///
  /// * 重複した要素の追加についてはチェックしない．
  void
  insert_elem(int row_pos,
	      int col_pos);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を得る．
  int
  row_size() const;

  /// @brief 列数を得る．
  int
  col_size() const;

  /// @brief 列のコストを得る．
  /// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size )
  int
  col_cost(int col_pos) const;

  /// @brief 要素のリストを得る．
  const vector<pair<int, int>>&
  elem_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 問題を解く関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @param[in] option オプション文字列
  /// @return 解のコスト
  int
  exact(vector<int>& solution,
	const string& option = string());

  /// @brief ヒューリスティックで最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @param[in] algorithm アルゴリズム名
  /// @param[in] option オプション文字列
  /// @return 解のコスト
  int
  heuristic(vector<int>& solution,
	    const string& algorithm = string(),
	    const string& option = string());


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コスト配列を初期化する．
  void
  init_cost_array();

  /// @brief コスト配列を初期化する．
  void
  init_cost_array(const vector<int>& col_cost_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行数
  int mRowSize;

  // 列数
  int mColSize;

  // 列のコスト配列
  // サイズは mColSize
  int* mColCostArray;

  // 要素のリスト
  // (row_pos, col_pos) のペアのリスト
  vector<pair<int, int> > mElemList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
MinCov::MinCov()
{
  mRowSize = 0;
  mColSize = 0;
  mColCostArray = nullptr;
}

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
inline
MinCov::MinCov(int row_size,
	       int col_size) :
  mRowSize(row_size),
  mColSize(col_size)
{
  init_cost_array();
}

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_cost_array 列のコストの配列
inline
MinCov::MinCov(int row_size,
	       const vector<int>& col_cost_array) :
  mRowSize(row_size),
  mColSize(col_cost_array.size())
{
  init_cost_array(col_cost_array);
}

// @brief デストラクタ
inline
MinCov::~MinCov()
{
  delete [] mColCostArray;
}

// @brief 問題のサイズを設定する．
// @param[in] row_size 行数
// @param[in] col_size 列数
inline
void
MinCov::resize(int row_size,
	       int col_size)
{
  mRowSize = row_size;
  mColSize = col_size;
  delete [] mColCostArray;
  init_cost_array();
  mElemList.clear();
}

// @brief 問題のサイズとコスト配列を設定する．
// @param[in] row_size 行数
// @param[in] col_cost_array 列のコスト配列
inline
void
MinCov::resize(int row_size,
	       const vector<int>& col_cost_array)
{
  mRowSize = row_size;
  mColSize = col_cost_array.size();
  delete [] mColCostArray;
  init_cost_array(col_cost_array);
  mElemList.clear();
}

// @brief 列のコストを設定する
// @param[in] col_pos 追加する要素の列番号
// @param[in] cost コスト
inline
void
MinCov::set_col_cost(int col_pos,
		     int cost)
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  mColCostArray[col_pos] = cost;
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
inline
void
MinCov::insert_elem(int row_pos,
		    int col_pos)
{
  mElemList.push_back(make_pair(row_pos, col_pos));
}

// @brief 行数を得る．
inline
int
MinCov::row_size() const
{
  return mRowSize;
}

// @brief 列数を得る．
inline
int
MinCov::col_size() const
{
  return mColSize;
}

// @brief 列のコストを得る．
// @param[in] col_pos 列番号 ( 0 <= col_pos < col_size )
inline
int
MinCov::col_cost(int col_pos) const
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  return mColCostArray[col_pos];
}

// @brief 要素のリストを得る．
inline
const vector<pair<int, int> >&
MinCov::elem_list() const
{
  return mElemList;
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_H
