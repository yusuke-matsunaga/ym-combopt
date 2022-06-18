#ifndef YM_MINCOV_H
#define YM_MINCOV_H

/// @file ym/MinCov.h
/// @brief MinCov のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2015, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MinCov MinCov.h "ym/MinCov.h"
/// @brief 最小被覆問題を表すクラス
///
/// 集合 $ = { s_1, s_2, ..., s_n } が与えられた時．
/// S の部分集合 S1 = { s_k1, s_k2, ..., s_kn }
/// に対して S1 の要素 s_ki は S1 を「被覆」する，と言う．
/// 与えられた部分集合族 C = { Si } の全てを被覆するような
/// S の部分集合のうち要素数最小のものを C の最小被覆と呼ぶ．
///
/// 場合によっては各集合要素にコストを持たせ，要素数ではなく，
/// 選ばれた要素のコストの総和を最小化する問題もある．
/// もしも負か0のコストを持つ要素があった場合，そのような要素は
/// 必ず最小解に含まれるので簡単に処理できる．
/// そこでコストは1以上と仮定する．
///
/// ここではこの問題を，集合要素を「列」に，被覆条件の部分集合
/// を「行」に対応させた「行列被覆問題」として表している．
//////////////////////////////////////////////////////////////////////
class MinCov
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  MinCov()
  {
    mRowSize = 0;
    mColSize = 0;
  }

  /// @brief サイズを指定したコンストラクタ
  ///
  /// * 空の行列となる．
  /// * 列のコストはすべて1になる．
  MinCov(
    SizeType row_size, ///< [in] 行数
    SizeType col_size  ///< [in] 列数
  ) : mRowSize{row_size},
      mColSize{col_size},
      mColCostArray(col_size, 1)
  {
  }

  /// @brief コスト配列をしていしたコンストラクタ
  ///
  /// * 列のサイズは col_cost_array のサイズから得られる．
  /// * 空の行列となる．
  MinCov(
    SizeType row_size,                     ///< [in] 行数
    const vector<SizeType>& col_cost_array ///< [in] 列のコストの配列
  ) : mRowSize{row_size},
      mColSize{col_cost_array.size()},
      mColCostArray{col_cost_array}
  {
  }

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
  ~MinCov() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題のサイズを設定する．
  ///
  /// * 空の行列となる．
  /// * 列のコストはすべて1になる．
  void
  resize(
    SizeType row_size, ///< [in] 行数
    SizeType col_size  ///< [in] 列数
  )
  {
    mRowSize = row_size;
    mColSize = col_size;
    mColCostArray.clear();
    mColCostArray.resize(col_size, 1);
    mElemList.clear();
  }

  /// @brief 問題のサイズとコスト配列を設定する．
  ///
  /// * 列のサイズは col_cost_array のサイズから得られる．
  /// * 空の行列となる．
  void
  resize(
    SizeType row_size,                ///< [in] 行数
    const vector<SizeType>& col_cost_array ///< [in] 列のコスト配列
  )
  {
    mRowSize = row_size;
    mColSize = col_cost_array.size();
    mColCostArray = col_cost_array;
    mElemList.clear();
  }

  /// @brief 列のコストを設定する
  void
  set_col_cost(
    SizeType col_pos, ///< [in] 列番号 ( 0 <= col_pos < col_size() )
    SizeType cost     ///< [in] コスト
  )
  {
    ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

    mColCostArray[col_pos] = cost;
  }

  /// @brief 要素を追加する．
  ///
  /// * 重複した要素の追加についてはチェックしない．
  void
  insert_elem(
    SizeType row_pos, ///< [in] 追加する要素の行番号
    SizeType col_pos  ///< [in] 追加する要素の列番号
  )
  {
    mElemList.push_back(make_pair(row_pos, col_pos));
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を得る．
  SizeType
  row_size() const
  {
    return mRowSize;
  }

  /// @brief 列数を得る．
  SizeType
  col_size() const
  {
    return mColSize;
  }

  /// @brief 列のコストを得る．
  SizeType
  col_cost(
    SizeType col_pos ///< [in] 列番号 ( 0 <= col_pos < col_size )
  ) const
  {
    ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

    return mColCostArray[col_pos];
  }

  /// @brief 列のコスト配列を得る．
  const vector<SizeType>&
  col_cost_array() const
  {
    return mColCostArray;
  }

  /// @brief 要素のリストを得る．
  const vector<pair<SizeType, SizeType>>&
  elem_list() const
  {
    return mElemList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 問題を解く関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコスト
  SizeType
  exact(
    vector<SizeType>& solution,     ///< [out] 選ばれた列集合
    const string& option = string{} ///< [in] オプション文字列
  );

  /// @brief ヒューリスティックで最小被覆問題を解く．
  /// @return 解のコスト
  SizeType
  heuristic(
    vector<SizeType>& solution,         ///< [out] 選ばれた列集合
    const string& algorithm = string{},	///< [in] アルゴリズム名
    const string& option = string{}	///< [in] オプション文字列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mElemList をチェックする．
  ///
  /// 要素を持たない行があったら警告する．
  void
  sanity_check();

  /// @brief 解を検証する．
  /// @retval true 正しい解だった．
  /// @retval false 被覆されていない行があった．
  bool
  verify_solution(
    const vector<SizeType>& solution, ///< [in] 解のリスト
    vector<SizeType>& uncov_row_list  ///< [in] 被覆されていない行のリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行数
  SizeType mRowSize;

  // 列数
  SizeType mColSize;

  // 列のコスト配列
  // サイズは mColSize
  vector<SizeType> mColCostArray;

  // 要素のリスト
  // (row_pos, col_pos) のペアのリスト
  vector<pair<SizeType, SizeType>> mElemList;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_H
