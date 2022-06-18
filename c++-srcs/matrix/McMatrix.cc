
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/McMatrix.h"
#include "matrix/McMatrixImpl.h"
#include "ym/McColComp.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McColComp
//////////////////////////////////////////////////////////////////////

// @brief col1 の代わりに col2 を使っても全体のコストが上がらない時に true を返す．
bool
McColComp::operator()(
  SizeType col1,
  SizeType col2
) const
{
  // デフォルトでは常に true を返す．
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス McMatrix
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
McMatrix::McMatrix(
) : mImpl{new McMatrixImpl{0, 0}}
{
}

// @brief コンストラクタ
//
// * elem_list は (row_pos, col_pos) の pair のリスト
McMatrix::McMatrix(
  SizeType row_size,
  SizeType col_size,
  const vector<pair<SizeType, SizeType>>& elem_list
) : mImpl{new McMatrixImpl{row_size, col_size, elem_list}}
{
}

// @brief コンストラクタ
//
// * elem_list は (row_pos, col_pos) の pair のリスト
McMatrix::McMatrix(
  SizeType row_size,
  const vector<SizeType>& cost_array,
  const vector<pair<SizeType, SizeType>>& elem_list
) : mImpl{new McMatrixImpl{row_size, cost_array, elem_list}}
{
}

// @brief コピーコンストラクタ
McMatrix::McMatrix(
  const McMatrix& src
) : mImpl{new McMatrixImpl(*src.mImpl)}
{
}

// @brief コピー代入演算子
McMatrix&
McMatrix::operator=(
  const McMatrix& src
)
{
  mImpl = unique_ptr<McMatrixImpl>{new McMatrixImpl{*src.mImpl}};

  return *this;
}

// @brief ムーブコンストラクタ
McMatrix::McMatrix(
  McMatrix&& src
) : mImpl{std::move(src.mImpl)}
{
}

// @brief ムーブ代入演算子
McMatrix&
McMatrix::operator=(
  McMatrix&& src
)
{
  mImpl.swap(src.mImpl);

  return *this;
}

// @brief デストラクタ
McMatrix::~McMatrix()
{
}

// @brief 行数を返す．
SizeType
McMatrix::row_size() const
{
  return mImpl->row_size();
}

// @brief アクティブな行の数
SizeType
McMatrix::active_row_num() const
{
  return mImpl->active_row_num();
}

// @brief アクティブな行のヘッダのリスト
const McHeadList&
McMatrix::row_head_list() const
{
  return mImpl->row_head_list();
}

// @brief 行方向のリストを返す．
McRowList
McMatrix::row_list(
  SizeType row_pos
) const
{
  return mImpl->row_list(row_pos);
}

// @brief 行の要素数を返す．
SizeType
McMatrix::row_elem_num(
  SizeType row_pos
) const
{
  return mImpl->row_elem_num(row_pos);
}

// @brief 行の削除フラグを調べる．
bool
McMatrix::row_deleted(
  SizeType row_pos
) const
{
  return mImpl->row_deleted(row_pos);
}

// @brief 列数を返す．
SizeType
McMatrix::col_size() const
{
  return mImpl->col_size();
}

// @brief アクティブな列の数
SizeType
McMatrix::active_col_num() const
{
  return mImpl->active_col_num();
}

// @brief アクティブな列のヘッダのリスト
const McHeadList&
McMatrix::col_head_list() const
{
  return mImpl->col_head_list();
}

// @brief 列方向のリストを返す．
McColList
McMatrix::col_list(
  SizeType col_pos
) const
{
  return mImpl->col_list(col_pos);
}

// @brief 列の要素数を返す．
SizeType
McMatrix::col_elem_num(
  SizeType col_pos
) const
{
  return mImpl->col_elem_num(col_pos);
}

// @brief 列の削除フラグを調べる．
bool
McMatrix::col_deleted(
  SizeType col_pos
) const
{
  return mImpl->col_deleted(col_pos);
}

// @brief 列のコストを取り出す．
SizeType
McMatrix::col_cost(
  SizeType col_pos
) const
{
  return mImpl->col_cost(col_pos);
}

// @brief 列のコストの配列を取り出す．
const SizeType*
McMatrix::col_cost_array() const
{
  return mImpl->col_cost_array();
}

// @brief 列集合のコストを返す．
SizeType
McMatrix::cost(
  const vector<SizeType>& col_list
) const
{
  return mImpl->cost(col_list);
}

// @brief 列集合がカバーになっているか検証する．
bool
McMatrix::verify(
  const vector<SizeType>& col_list
) const
{
  return mImpl->verify(col_list);
}

// @brief 内容を出力する．
void
McMatrix::print(
  ostream& s
) const
{
  mImpl->print(s);
}

// @brief 内容をクリアする．
void
McMatrix::clear()
{
  mImpl->clear();
}

// @brief サイズを変更する．
//
// 内容はクリアされる．
void
McMatrix::resize(
  SizeType row_size,
  SizeType col_size
  )
{
  mImpl = unique_ptr<McMatrixImpl>{new McMatrixImpl(row_size, col_size)};
}

// @brief 要素を追加する．
void
McMatrix::insert_elem(
  SizeType row_pos,
  SizeType col_pos
)
{
  mImpl->insert_elem(row_pos, col_pos);
}

// @brief 要素を追加する．
void
McMatrix::insert_elem(
  const vector<pair<SizeType, SizeType>>& elem_list
)
{
  mImpl->insert_elem(elem_list);
}

// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
void
McMatrix::select_col(
  SizeType col_pos
)
{
  mImpl->select_col(col_pos);
}

// @brief 行列を縮約する．
bool
McMatrix::reduce(
  vector<SizeType>& selected_cols,
  vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  return mImpl->reduce(selected_cols, deleted_cols, col_comp);
}

// @brief 変化がなくなるまで reduce() を呼ぶ．
void
McMatrix::reduce_loop(
  vector<SizeType>& selected_cols,
  vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  for ( ; ; ) {
    if ( !reduce(selected_cols, deleted_cols, col_comp) ) {
      break;
    }
  }
}

// @brief 行を削除する．
void
McMatrix::delete_row(
  SizeType row_pos
)
{
  mImpl->delete_row(row_pos);
}

// @brief 列を削除する．
void
McMatrix::delete_col(
  SizeType col_pos
)
{
  mImpl->delete_col(col_pos);
}

// @brief 行に dirty フラグをつける．
void
McMatrix::set_row_dirty(
  SizeType row_pos
)
{
  mImpl->set_row_dirty(row_pos);
}

// @brief 列に dirty フラグをつける．
void
McMatrix::set_col_dirty(
  SizeType col_pos
)
{
  mImpl->set_col_dirty(col_pos);
}

// @brief 削除スタックにマーカーを書き込む．
void
McMatrix::save()
{
  mImpl->save();
}

// @brief 直前のマーカーまで処理を戻す．
void
McMatrix::restore()
{
  mImpl->restore();
}

END_NAMESPACE_YM_MINCOV
