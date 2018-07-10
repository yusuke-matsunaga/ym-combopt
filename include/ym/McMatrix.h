#ifndef YM_MCMATRIX_H
#define YM_MCMATRIX_H

/// @file ym/McMatrix.h
/// @brief McMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"
#include "ym/McHead.h"
#include "ym/McColList.h"
#include "ym/McRowList.h"
#include "ym/UnitAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McMatrix McMatrix.h "ym/McMatrix.h"
/// @brief mincov 用の行列を表すクラス
///
/// * 単純には m x n のブーリアン行列
/// * 1 の要素のみを持つので，1の要素が相対的に少ないスパースの時に効率がよい．
/// * 要素は行方向・列方向にそれぞれ双方向リストでつながっている．
//////////////////////////////////////////////////////////////////////
class McMatrix
{
  friend class McBlock;

public:

  /// @brief コンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @param[in] cost_array コストの配列
  /// @param[in] elem_list 要素のリスト
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrix(int row_size,
	   int col_size,
	   const vector<int>& cost_array,
	   const vector<pair<int, int>>& elem_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  McMatrix(const McMatrix& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const McMatrix&
  operator=(const McMatrix& src);

  /// @brief デストラクタ
  ~McMatrix();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  int
  row_size() const;

  /// @brief 行方向のリストを返す．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  McRowList
  row_list(int row_pos) const;

  /// @brief 行の要素数を返す．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  int
  row_elem_num(int row_pos) const;

  /// @brief 列数を返す．
  int
  col_size() const;

  /// @brief 列方向のリストを返す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  McColList
  col_list(int col_pos) const;

  /// @brief 列の要素数を返す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  int
  col_elem_num(int col_pos) const;

  /// @brief 列のコストを取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  int
  col_cost(int col_pos) const;

  /// @brief 列のコストの配列を取り出す．
  const int*
  col_cost_array() const;

  /// @brief 列集合のコストを返す．
  /// @param[in] col_list 列のリスト
  int
  cost(const vector<int>& col_list) const;

  /// @brief 列集合がカバーになっているか検証する．
  /// @param[in] col_list 列のリスト
  /// @retval true col_list がカバーになっている．
  /// @retval false col_list でカバーされていない行がある．
  bool
  verify(const vector<int>& col_list) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  /// @note 行/列のサイズは不変
  void
  clear();

  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  void
  insert_elem(int row_pos,
	      int col_pos);

  /// @brief 要素を追加する．
  /// @param[in] elem_list 要素のリスト
  ///
  /// * 要素は (row_pos, col_pos) のペアで表す．
  void
  insert_elem(const vector<pair<int, int>>& elem_list);


public:
  //////////////////////////////////////////////////////////////////////
  // restore 用のスタックを操作する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スタックが空の時 true を返す．
  bool
  stack_empty();

  /// @brief スタックに削除した行/列のヘッダと積む．
  void
  push(McHead* head);

  /// @brief スタックから取り出す．
  McHead*
  pop();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを変更する．
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  ///
  /// 内容はクリアされる．
  void
  resize(int row_size,
	 int col_size);

  /// @brief 内容をコピーする．
  void
  copy(const McMatrix& src);

  /// @brief 行の先頭を取り出す．
  /// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
  const McHead*
  _row_head(int row_pos) const;

  /// @brief 行の先頭を取り出す．
  /// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
  McHead*
  _row_head(int row_pos);

  /// @brief 列の先頭を取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  const McHead*
  _col_head(int col_pos) const;

  /// @brief 列の先頭を取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  McHead*
  _col_head(int col_pos);

  /// @brief mRowMark, mColMark の sanity check
  /// @retval true mRowMark, mColMark の内容が全て 0 だった．
  /// @retval false mRowMark, mColMark に非0の要素が含まれていた．
  bool
  check_mark_sanity();

  /// @brief セルの生成
  /// @param[in] row_pos 行番号
  /// @param[in] col_pos 列番号
  McCell*
  alloc_cell(int row_pos,
	     int col_pos);

  /// @brief セルの解放
  void
  free_cell(McCell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルを確保するためのアロケータ
  UnitAlloc mCellAlloc;

  // 行数
  int mRowSize;

  // 行の先頭の配列
  // サイズは mRowSize
  McHead* mRowArray;

  // 列数
  int mColSize;

  // 列の先頭の配列
  // サイズは mColSize
  McHead* mColArray;

  // コストの配列
  // サイズは mColSize;
  int* mCostArray;

  // 削除の履歴を覚えておくスタック
  McHead** mDelStack;

  // mDelStack のポインタ
  int mStackTop;

  // 作業用に使う行のマーク配列
  // サイズは mRowSize
  mutable
  int* mRowMark;

  // 作業用に使う列のマーク配列
  // サイズは mColSize
  mutable
  int* mColMark;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を返す．
inline
int
McMatrix::row_size() const
{
  return mRowSize;
}

// @brief 行方向のリストを返す．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
inline
McRowList
McMatrix::row_list(int row_pos) const
{
  auto head = _row_head(row_pos);
  return McRowList(head->row_begin(), head->row_end());
}

// @brief 行の要素数を返す．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
inline
int
McMatrix::row_elem_num(int row_pos) const
{
  return _row_head(row_pos)->num();
}

// @brief 行の先頭を取り出す．
// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
inline
const McHead*
McMatrix::_row_head(int row_pos) const
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  return &mRowArray[row_pos];
}

// @brief 行の先頭を取り出す．
// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
inline
McHead*
McMatrix::_row_head(int row_pos)
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  return &mRowArray[row_pos];
}

// @brief 列数を返す．
inline
int
McMatrix::col_size() const
{
  return mColSize;
}

// @brief 列方向のリストを返す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
McColList
McMatrix::col_list(int col_pos) const
{
  auto head = _col_head(col_pos);
  return McColList(head->col_begin(), head->col_end());
}

// @brief 列の要素数を返す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McMatrix::col_elem_num(int col_pos) const
{
  return _col_head(col_pos)->num();
}

// @brief 列の先頭を取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
const McHead*
McMatrix::_col_head(int col_pos) const
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  return &mColArray[col_pos];
}

// @brief 列の先頭を取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
McHead*
McMatrix::_col_head(int col_pos)
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  return &mColArray[col_pos];
}

// @brief 列のコストを取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McMatrix::col_cost(int col_pos) const
{
  return mCostArray[col_pos];
}

// @brief 列のコストの配列を取り出す．
inline
const int*
McMatrix::col_cost_array() const
{
  return mCostArray;
}

// @brief スタックが空の時 true を返す．
inline
bool
McMatrix::stack_empty()
{
  return mStackTop == 0;
}

// @brief スタックに削除した行/列のヘッダを積む．
inline
void
McMatrix::push(McHead* head)
{
  mDelStack[mStackTop] = head;
  ++ mStackTop;
}

// @brief スタックから取り出す．
inline
McHead*
McMatrix::pop()
{
  -- mStackTop;
  return mDelStack[mStackTop];
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCMATRIX_H
