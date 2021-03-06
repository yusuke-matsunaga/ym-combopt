#ifndef YM_MCBLOCK_H
#define YM_MCBLOCK_H

/// @file ym/McBlock.h
/// @brief McBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "ym/McMatrix.h"
#include "ym/McColList.h"
#include "ym/McRowList.h"
#include "ym/McHeadList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McBlock McBlock.h "McBlock.h"
/// @brief McMatrix 上のブロックを表すクラス
//////////////////////////////////////////////////////////////////////
class McBlock
{
public:

  /// @brief コンストラクタ
  /// @param[in] matrix 対象の行列
  ///
  /// 行列全体を対象のブロックとする．
  McBlock(McMatrix& matrix);

  /// @brief コンストラクタ
  /// @param[in] matrix 対象の行列
  /// @param[in] row_list 行番号のリスト
  /// @param[in] col_list 列番号のリスト
  ///
  /// 指定された行と列のみを対象のブロックとする．
  McBlock(McMatrix& matrix,
	  const vector<int>& row_list,
	  const vector<int>& col_list);

  /// @brief コピーコンストラクタは禁止
  McBlock(const McBlock& src) = delete;

  /// @brief 代入演算子は禁止
  McBlock&
  operator=(const McBlock& src) = delete;

  /// @brief デストラクタ
  ~McBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  int
  row_size() const;

  /// @brief 行の先頭のリストを返す．
  const McHeadList&
  row_head_list() const;

  /// @brief 行の先頭のリストを返す．
  McHeadList&
  row_head_list();

  /// @brief 実効的な行数を返す．
  int
  row_num() const;

  /// @brief 列数を返す．
  int
  col_size() const;

  /// @brief 列の先頭のリストを返す．
  const McHeadList&
  col_head_list() const;

  /// @brief 列の先頭のリストを返す．
  McHeadList&
  col_head_list();

  /// @brief 実効的な列数を返す．
  int
  col_num() const;

  /// @brief 列のコストを取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  int
  col_cost(int col_pos) const;

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

  /// @brief 列を選択し，被覆される行を削除する．
  /// @param[in] col_pos 選択した列
  void
  select_col(int col_pos);

  /// @brief 列を選択しない．
  /// @param[in] col_pos 選択しない列
  void
  deselect_col(int col_pos);

  /// @brief 簡単化を行う．
  /// @param[out] selected_cols 簡単化中で選択された列の集合を追加する配列
  void
  reduce(vector<int>& selected_cols);

  /// @brief ブロック分割する．
  /// @param[out] remainder 分割された他方のブロックを入れる変数
  /// @retval true 分割できた．
  /// @retval false 分割できなかった．
  bool
  partition(McBlock& remainder);

  /// @brief 内容を表示する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行支配を探し，行を削除する．
  /// @return 削除された行があったら true を返す．
  bool
  row_dominance();

  /// @brief 列支配を探し，列を削除する．
  /// @return 削除された列があったら true を返す．
  bool
  col_dominance();

  /// @brief 必須列を探し，列を選択する．
  /// @param[out] selected_cols 選択された列を追加する列集合
  /// @return 選択された列があったら true を返す．
  bool
  essential_col(vector<int>& selected_cols);

  /// @brief 列に接続している行に印をつける．
  /// @param[in] col_pos 開始する列番号
  /// @param[inout] row_mark 行の印
  /// @param[inout] col_mark 列の印
  /// @return 印の付けられた列数を返す．
  int
  mark_rows(int col_pos,
	    vector<bool>& row_mark,
	    vector<bool>& col_mark);

  /// @brief 行に接続している列に印をつける．
  /// @param[in] row_pos 開始する行番号
  /// @param[inout] row_mark 行の印
  /// @param[inout] col_mark 列の印
  /// @return 印の付けられた列数を返す．
  int
  mark_cols(int row_pos,
	    vector<bool>& row_mark,
	    vector<bool>& col_mark);

  /// @brief 行を削除する．
  /// @param[in] row_pos 削除する行番号
  void
  delete_row(int row_pos);

  /// @brief 列を削除する．
  /// @param[in] col_pos 削除する列番号
  void
  delete_col(int col_pos);

  /// @brief 行を復元する．
  void
  restore_row(McHead* row_head);

  /// @brief 列を復元する．
  void
  restore_col(McHead* col_head);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の行列
  McMatrix& mMatrix;

  // 有効な行のリスト
  McHeadList mRowHeadList;

  // 有効な列のリスト
  McHeadList mColHeadList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を返す．
inline
int
McBlock::row_size() const
{
  return mMatrix.row_size();
}

// @brief 行の先頭のリストを返す．
inline
const McHeadList&
McBlock::row_head_list() const
{
  return mRowHeadList;
}

// @brief 行の先頭のリストを返す．
inline
McHeadList&
McBlock::row_head_list()
{
  return mRowHeadList;
}

// @brief 実効的な行数を返す．
inline
int
McBlock::row_num() const
{
  return mRowHeadList.num();
}

// @brief 列数を返す．
inline
int
McBlock::col_size() const
{
  return mMatrix.col_size();
}

/// @brief 列の先頭のリストを返す．
inline
const McHeadList&
McBlock::col_head_list() const
{
  return mColHeadList;
}

// @brief 列の先頭のリストを返す．
inline
McHeadList&
McBlock::col_head_list()
{
  return mColHeadList;
}

// @brief 実効的な列数を返す．
inline
int
McBlock::col_num() const
{
  return mColHeadList.num();
}

// @brief 列のコストを取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McBlock::col_cost(int col_pos) const
{
  return mMatrix.col_cost(col_pos);
}

// @brief 列を選択しない．
// @param[in] col_pos 選択しない列
inline
void
McBlock::deselect_col(int col_pos)
{
  delete_col(col_pos);
}

END_NAMESPACE_YM_MINCOV

#endif // MCBLOCK_H
