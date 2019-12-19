﻿#ifndef YM_MCMATRIXIMPL_H
#define YM_MCMATRIXIMPL_H

/// @file ym/McMatrixImpl.h
/// @brief McMatrixImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/mincov_nsdef.h"
#include "ym/McHead.h"
#include "ym/McHeadList.h"
#include "ym/McColList.h"
#include "ym/McRowList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McMatrixImpl McMatrixImpl.h "ym/McMatrixImpl.h"
/// @brief mincov 用の行列を表すクラス
///
/// * 単純には m x n のブーリアン行列
/// * 1 の要素のみを持つので，1の要素が相対的に少ないスパースの時に効率がよい．
/// * 要素は行方向・列方向にそれぞれ双方向リストでつながっている．
/// * さらに，現在アクティブな行，列のヘッダがそれぞれ双方向リストでつながっている．
//////////////////////////////////////////////////////////////////////
class McMatrixImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @param[in] elem_list 要素のリスト
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrixImpl(int row_size,
	       int col_size,
	       const vector<pair<int, int>>& elem_list = vector<pair<int, int>>());

  /// @brief コンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] cost_array コストの配列
  /// @param[in] elem_list 要素のリスト
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrixImpl(int row_size,
	       const vector<int>& cost_array,
	       const vector<pair<int, int>>& elem_list = vector<pair<int, int>>());

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  McMatrixImpl(const McMatrixImpl& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const McMatrixImpl&
  operator=(const McMatrixImpl& src);

  /// @brief デストラクタ
  ~McMatrixImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  int
  row_size() const;

  /// @brief アクティブな行の数
  int
  active_row_num() const;

  /// @brief アクティブな行のヘッダのリスト
  const McHeadList&
  row_head_list() const;

  /// @brief 行方向のリストを返す．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  McRowList
  row_list(int row_pos) const;

  /// @brief 行の要素数を返す．
  /// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
  int
  row_elem_num(int row_pos) const;

  /// @brief 行の削除フラグを調べる．
  bool
  row_deleted(int row_pos) const;

  /// @brief 列数を返す．
  int
  col_size() const;

  /// @brief アクティブな列の数
  int
  active_col_num() const;

  /// @brief アクティブな列のヘッダのリスト
  const McHeadList&
  col_head_list() const;

  /// @brief 列方向のリストを返す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  McColList
  col_list(int col_pos) const;

  /// @brief 列の要素数を返す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  int
  col_elem_num(int col_pos) const;

  /// @brief 列の削除フラグを調べる．
  bool
  col_deleted(int col_pos) const;

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
  // 内容を変更する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
  /// @param[in] col_pos 選択する列番号
  void
  select_col(int col_pos);

  /// @brief 行列を縮約する．
  /// @param[out] selected_cols この縮約で選択された列を格納するベクタ
  /// @param[out] deleted_cols この縮約で削除された列を格納するベクタ
  /// @param[in] col_comp 列の比較関数オブジェクト
  /// @retval true 縮約された．
  /// @retval false 縮約されなかった．
  ///
  /// * この関数は行支配，列支配，必須列を１回だけ適用する．
  /// * この関数が false を返すまで繰り返す必要がある．
  /// * selected_cols, deleted_cols はこの関数内で初期化されず，
  ///   追加されるだけなので注意．
  bool
  reduce(vector<int>& selected_cols,
	 vector<int>& deleted_cols,
	 const McColComp& col_comp);

  /// @brief 行を削除する．
  /// @param[in] row_pos 削除する行番号
  void
  delete_row(int row_pos);

  /// @brief 列を削除する．
  /// @param[in] col_pos 削除する列番号
  void
  delete_col(int col_pos);

  /// @brief 行に dirty フラグをつける．
  void
  set_row_dirty(int row_pos);

  /// @brief 列に dirty フラグをつける．
  void
  set_col_dirty(int col_pos);

  /// @brief 削除スタックにマーカーを書き込む．
  void
  save();

  /// @brief 直前のマーカーまで処理を戻す．
  void
  restore();


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
  copy(const McMatrixImpl& src);

  /// @brief 行支配による縮約を行う．
  /// @retval true 縮約が行われた．
  /// @retval false 縮約が行われなかった．
  bool
  row_dominance();

  /// @brief 列支配による縮約を行う．
  /// @param[out] deleted_cols この縮約で削除された列を格納するベクタ
  /// @param[in] col_comp 列の比較関数オブジェクト
  /// @retval true 縮約が行われた．
  /// @retval false 縮約が行われなかった．
  bool
  col_dominance(vector<int>& deleted_cols,
		const McColComp& col_comp);

  /// @brief 必須列による縮約を行う．
  /// @param[out] selected_cols この縮約で選択された列を格納するベクタ
  /// @retval true 縮約が行われた．
  /// @retval false 縮約が行われなかった．
  bool
  essential_col(vector<int>& selected_cols);

  /// @brief 行を復元する．
  void
  restore_row(McHead* row_head);

  /// @brief 列を復元する．
  void
  restore_col(McHead* col_head);

  /// @brief スタックが空の時 true を返す．
  bool
  stack_empty();

  /// @brief スタックに削除した行/列のヘッダと積む．
  void
  push(McHead* head);

  /// @brief スタックから取り出す．
  McHead*
  pop();

  /// @brief 行の削除フラグをセットする．
  /// @param[in] row_pos 行番号
  /// @param[in] flag フラグの値
  void
  set_row_deleted(int row_pos,
		  bool flag);

  /// @brief 列の削除フラグをセットする．
  /// @param[in] col_pos 列番号
  /// @param[in] flag フラグの値
  void
  set_col_deleted(int col_pos,
		  bool flag);

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

  // 確保したセルのリスト
  vector<McCell*> mCellList;

  // フリーリストの先頭
  McCell* mFreeTop{nullptr};

  // 行数
  int mRowSize{0};

  // 行のヘッダの配列
  // サイズは mRowSize
  McHead* mRowHeadArray{nullptr};

  // 現在アクティブな行のヘッダのリスト
  McHeadList mRowHeadList;

  // 行の先頭を表すダミーセルの配列
  // サイズは mRowSize
  McCell** mRowArray{nullptr};

  // 列数
  int mColSize{0};

  // 列のヘッダの配列
  // サイズは mColSize
  McHead* mColHeadArray{nullptr};

  // 現在アクティブな列のヘッダのリスト
  McHeadList mColHeadList;

  // 列の先頭を表すダミーセルの配列
  // サイズは mColSize
  McCell** mColArray{nullptr};

  // コストの配列
  // サイズは mColSize;
  int* mCostArray{0};

  // 削除の履歴を覚えておくスタック
  McHead** mDelStack{nullptr};

  // mDelStack のポインタ
  int mStackTop{0};

  // 作業用に使う行のマーク配列
  // サイズは mRowSize
  mutable
  int* mRowMark{nullptr};

  // 作業用に使う列のマーク配列
  // サイズは mColSize
  mutable
  int* mColMark{nullptr};

  // 作業用に使う配列
  // サイズは max(mRowSize, mColSize)
  mutable
  int* mDelList{nullptr};

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を返す．
inline
int
McMatrixImpl::row_size() const
{
  return mRowSize;
}

// @brief アクティブな行の数
inline
int
McMatrixImpl::active_row_num() const
{
  return mRowHeadList.num();
}

// @brief 行のヘッダのリスト
inline
const McHeadList&
McMatrixImpl::row_head_list() const
{
  return mRowHeadList;
}

// @brief 行方向のリストを返す．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
inline
McRowList
McMatrixImpl::row_list(int row_pos) const
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  auto dummy = mRowArray[row_pos];
  return McRowList(dummy->row_next(), dummy);
}

// @brief 行の要素数を返す．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
inline
int
McMatrixImpl::row_elem_num(int row_pos) const
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  return mRowHeadArray[row_pos].num();
}

// @brief 行の削除フラグを調べる．
// @param[in] row_pos 行番号
inline
bool
McMatrixImpl::row_deleted(int row_pos) const
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  return mRowHeadArray[row_pos].is_deleted();
}

// @brief 行の削除フラグをセットする．
// @param[in] row_pos 行番号
// @param[in] flag フラグの値
inline
void
McMatrixImpl::set_row_deleted(int row_pos,
			      bool flag)
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  mRowHeadArray[row_pos].set_deleted(flag);
}

// @brief 行に dirty フラグをつける．
inline
void
McMatrixImpl::set_row_dirty(int row_pos)
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

  mRowHeadArray[row_pos].set_dirty(false);
}

// @brief 列数を返す．
inline
int
McMatrixImpl::col_size() const
{
  return mColSize;
}

// @brief アクティブな列の数
inline
int
McMatrixImpl::active_col_num() const
{
  return mColHeadList.num();
}

// @brief 列のヘッダのリスト
inline
const McHeadList&
McMatrixImpl::col_head_list() const
{
  return mColHeadList;
}

// @brief 列方向のリストを返す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
McColList
McMatrixImpl::col_list(int col_pos) const
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  auto dummy = mColArray[col_pos];
  return McColList(dummy->col_next(), dummy);
}

// @brief 列の要素数を返す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McMatrixImpl::col_elem_num(int col_pos) const
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  return mColHeadArray[col_pos].num();
}

// @brief 列の削除フラグを調べる．
inline
bool
McMatrixImpl::col_deleted(int col_pos) const
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  return mColHeadArray[col_pos].is_deleted();
}

// @brief 列の削除フラグをセットする．
// @param[in] col_pos 列番号
// @param[in] flag フラグの値
inline
void
McMatrixImpl::set_col_deleted(int col_pos,
			      bool flag)
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  mColHeadArray[col_pos].set_deleted(flag);
}

// @brief 列に dirty フラグをつける．
inline
void
McMatrixImpl::set_col_dirty(int col_pos)
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  mColHeadArray[col_pos].set_dirty(true);
}

// @brief 列のコストを取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McMatrixImpl::col_cost(int col_pos) const
{
  return mCostArray[col_pos];
}

// @brief 列のコストの配列を取り出す．
inline
const int*
McMatrixImpl::col_cost_array() const
{
  return mCostArray;
}

// @brief スタックが空の時 true を返す．
inline
bool
McMatrixImpl::stack_empty()
{
  return mStackTop == 0;
}

// @brief スタックに削除した行/列のヘッダを積む．
inline
void
McMatrixImpl::push(McHead* head)
{
  mDelStack[mStackTop] = head;
  ++ mStackTop;
}

// @brief スタックから取り出す．
inline
McHead*
McMatrixImpl::pop()
{
  -- mStackTop;
  return mDelStack[mStackTop];
}

END_NAMESPACE_YM_MINCOV

#endif // YM_MCMATRIXIMPL_H
