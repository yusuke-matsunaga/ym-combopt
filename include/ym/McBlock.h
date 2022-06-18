#ifndef YM_MCBLOCK_H
#define YM_MCBLOCK_H

/// @file ym/McBlock.h
/// @brief McBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
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
  ///
  /// 行列全体を対象のブロックとする．
  McBlock(
    McMatrix& matrix ///< [in] 対象の行列
  );

  /// @brief コンストラクタ
  ///
  /// 指定された行と列のみを対象のブロックとする．
  McBlock(
    McMatrix& matrix,                 ///< [in] 対象の行列
    const vector<SizeType>& row_list, ///< [in] 行番号のリスト
    const vector<SizeType>& col_list  ///< [in] 列番号のリスト
  );

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
  SizeType
  row_size() const
  {
    return mMatrix.row_size();
  }

  /// @brief 行の先頭のリストを返す．
  const McHeadList&
  row_head_list() const
  {
    return mRowHeadList;
  }

  /// @brief 行の先頭のリストを返す．
  McHeadList&
  row_head_list()
  {
    return mRowHeadList;
  }

  /// @brief 実効的な行数を返す．
  SizeType
  row_num() const
  {
    return mRowHeadList.num();
  }

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mMatrix.col_size();
  }

  /// @brief 列の先頭のリストを返す．
  const McHeadList&
  col_head_list() const
  {
    return mColHeadList;
  }

  /// @brief 列の先頭のリストを返す．
  McHeadList&
  col_head_list()
  {
    return mColHeadList;
  }

  /// @brief 実効的な列数を返す．
  SizeType
  col_num() const
  {
    return mColHeadList.num();
  }

  /// @brief 列のコストを取り出す．
  int
  col_cost(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mMatrix.col_cost(col_pos);
  }

  /// @brief 列集合のコストを返す．
  int
  cost(
    const vector<SizeType>& col_list ///< [in] 列のリスト
  ) const;

  /// @brief 列集合がカバーになっているか検証する．
  /// @retval true col_list がカバーになっている．
  /// @retval false col_list でカバーされていない行がある．
  bool
  verify(
    const vector<SizeType>& col_list ///< [in] 列のリスト
  ) const;

  /// @brief 列を選択し，被覆される行を削除する．
  void
  select_col(
    SizeType col_pos ///< [in] 選択した列
  );

  /// @brief 列を選択しない．
  void
  deselect_col(
    SizeType col_pos ///< [in] 選択しない列
  )
  {
    delete_col(col_pos);
  }

  /// @brief 簡単化を行う．
  void
  reduce(
    vector<SizeType>& selected_cols ///< [in] 簡単化中で選択された列の集合を追加する配列
  );

  /// @brief ブロック分割する．
  /// @retval true 分割できた．
  /// @retval false 分割できなかった．
  bool
  partition(
    McBlock& remainder ///< [in] 分割された他方のブロックを入れる変数
  );

  /// @brief 内容を表示する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


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
  /// @return 選択された列があったら true を返す．
  bool
  essential_col(
    vector<SizeType>& selected_cols ///< [out] 選択された列を追加する列集合
  );

  /// @brief 列に接続している行に印をつける．
  /// @return 印の付けられた列数を返す．
  Sizetype
  mark_rows(
    SizeType col_pos,       ///< [in] 開始する列番号
    vector<bool>& row_mark, ///< [inout] 行の印
    vector<bool>& col_mark  ///< [inout] 列の印
  );

  /// @brief 行に接続している列に印をつける．
  /// @return 印の付けられた列数を返す．
  int
  mark_cols(
    Sizetype row_pos,       ///< [in] 開始する行番号
    vector<bool>& row_mark, ///< [inout] 行の印
    vector<bool>& col_mark  ///< [inout] 列の印
  );

  /// @brief 行を削除する．
  void
  delete_row(
    SizeType row_pos ///< [in] 削除する行番号
  );

  /// @brief 列を削除する．
  void
  delete_col(
    SizeType col_pos ///< [in] 削除する列番号
  );

  /// @brief 行を復元する．
  void
  restore_row(
    McHead* row_head
  );

  /// @brief 列を復元する．
  void
  restore_col(
    McHead* col_head
  );


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

#if 0
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
#endif

END_NAMESPACE_YM_MINCOV

#endif // MCBLOCK_H
