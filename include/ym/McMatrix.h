#ifndef YM_MCMATRIX_H
#define YM_MCMATRIX_H

/// @file ym/McMatrix.h
/// @brief McMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"
#include "ym/McMatrixImpl.h"
#include "ym/McHead.h"
#include "ym/McHeadList.h"
#include "ym/McColComp.h"
#include "ym/McColList.h"
#include "ym/McRowList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McMatrix McMatrix.h "ym/McMatrix.h"
/// @brief mincov 用の行列を表すクラス
///
/// * 単純には m x n のブーリアン行列
/// * 1 の要素のみを持つので，1の要素が相対的に少ないスパースの時に効率がよい．
/// * 要素は行方向・列方向にそれぞれ双方向リストでつながっている．
/// * さらに，現在アクティブな行，列のヘッダがそれぞれ双方向リストでつながっている．
//////////////////////////////////////////////////////////////////////
class McMatrix
{
public:

  /// @brief 空のコンストラクタ
  McMatrix(
  ) : mImpl{new McMatrixImpl{0, 0}}
  {
  }

  /// @brief コンストラクタ
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrix(
    SizeType row_size,                                ///< [in] 行数
    SizeType col_size,				      ///< [in] 列数
    const vector<pair<SizeType, SizeType>>& elem_list ///< [in] 要素のリスト
    = vector<pair<SizeType, SizeType>>{}
  ) : mImpl{new McMatrixImpl{row_size, col_size, elem_list}}
  {
  }

  /// @brief コンストラクタ
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrix(
    SizeType row_size,                                ///< [in] 行数
    const vector<int>& cost_array,                    ///< [in] コストの配列
    const vector<pair<SizeType, SizeType>>& elem_list ///< [in] 要素のリスト
    = vector<pair<SizeType, SizeType>>{}
  ) : mImpl{new McMatrixImpl{row_size, cost_array, elem_list}}
  {
  }

  /// @brief コピーコンストラクタ
  McMatrix(
    const McMatrix& src ///< [in] コピー元のオブジェクト
  ) : mImpl{new McMatrixImpl(*src.mImpl)}
  {
  }

  /// @brief コピー代入演算子
  McMatrix&
  operator=(
    const McMatrix& src ///< [in] コピー元のオブジェクト
  )
  {
    mImpl = unique_ptr<McMatrixImpl>{new McMatrixImpl{*src.mImpl}};

    return *this;
  }

  /// @brief ムーブコンストラクタ
  McMatrix(
    McMatrix&& src ///< [in] ムーブ元のオブジェクト
  ) = default;

  /// @brief ムーブ代入演算子
  McMatrix&
  operator=(
    McMatrix&& src ///< [in] ムーブ元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~McMatrix() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  SizeType
  row_size() const
  {
    return mImpl->row_size();
  }

  /// @brief アクティブな行の数
  SizeType
  active_row_num() const
  {
    return mImpl->active_row_num();
  }

  /// @brief アクティブな行のヘッダのリスト
  const McHeadList&
  row_head_list() const
  {
    return mImpl->row_head_list();
  }

  /// @brief 行方向のリストを返す．
  McRowList
  row_list(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const
  {
    return mImpl->row_list(row_pos);
  }

  /// @brief 行の要素数を返す．
  int
  row_elem_num(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const
  {
    return mImpl->row_elem_num(row_pos);
  }

  /// @brief 行の削除フラグを調べる．
  bool
  row_deleted(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const
  {
    return mImpl->row_deleted(row_pos);
  }

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mImpl->col_size();
  }

  /// @brief アクティブな列の数
  SizeType
  active_col_num() const
  {
    return mImpl->active_col_num();
  }

  /// @brief アクティブな列のヘッダのリスト
  const McHeadList&
  col_head_list() const
  {
    return mImpl->col_head_list();
  }

  /// @brief 列方向のリストを返す．
  McColList
  col_list(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mImpl->col_list(col_pos);
  }

  /// @brief 列の要素数を返す．
  SizeType
  col_elem_num(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mImpl->col_elem_num(col_pos);
  }

  /// @brief 列の削除フラグを調べる．
  bool
  col_deleted(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mImpl->col_deleted(col_pos);
  }

  /// @brief 列のコストを取り出す．
  SizeType
  col_cost(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mImpl->col_cost(col_pos);
  }

  /// @brief 列のコストの配列を取り出す．
  const int*
  col_cost_array() const
  {
    return mImpl->col_cost_array();
  }

  /// @brief 列集合のコストを返す．

  int
  cost(
    const vector<SizeType>& col_list ///< [in] 列のリスト
  ) const
  {
    return mImpl->cost(col_list);
  }

  /// @brief 列集合がカバーになっているか検証する．
  /// @retval true col_list がカバーになっている．
  /// @retval false col_list でカバーされていない行がある．
  bool
  verify(
    const vector<SizeType>& col_list ///< [in] 列のリスト
  ) const
  {
    return mImpl->verify(col_list);
  }

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const
  {
    mImpl->print(s);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  /// @note 行/列のサイズは不変
  void
  clear()
  {
    mImpl->clear();
  }

  /// @brief サイズを変更する．
  ///
  /// 内容はクリアされる．
  void
  resize(
    SizeType row_size, ///< [in] 行数
    SizeType col_size  ///< [in] 列数
  )
  {
    mImpl = unique_ptr<McMatrixImpl>{new McMatrixImpl(row_size, col_size)};
  }

  /// @brief 要素を追加する．
  void
  insert_elem(
    SizeType row_pos, ///< [in] 追加する要素の行番号
    SizeType col_pos  ///< [in] 追加する要素の列番号
  )
  {
    mImpl->insert_elem(row_pos, col_pos);
  }

  /// @brief 要素を追加する．
  ///
  /// * 要素は (row_pos, col_pos) のペアで表す．
  void
  insert_elem(
    const vector<pair<SizeType, SizeType>>& elem_list ///< [in] 要素のリスト
  )
  {
    mImpl->insert_elem(elem_list);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を変更する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
  void
  select_col(
    SizeType col_pos ///< [in] 選択する列番号
  )
  {
    mImpl->select_col(col_pos);
  }

  /// @brief 行列を縮約する．
  /// @retval true 縮約された．
  /// @retval false 縮約されなかった．
  ///
  /// * この関数は行支配，列支配，必須列を１回だけ適用する．
  /// * この関数が false を返すまで繰り返す必要がある．
  /// * selected_cols, deleted_cols はこの関数内で初期化されず，
  ///   追加されるだけなので注意．
  bool
  reduce(
    vector<SizeType>& selected_cols, ///< [out] この縮約で選択された列を格納するベクタ
    vector<SizeType>& deleted_cols,  ///< [out] この縮約で削除された列を格納するベクタ
    const McColComp& col_comp	     ///< [in]  列の比較関数オブジェクト
    = McColComp{}
  )
  {
    return mImpl->reduce(selected_cols, deleted_cols, col_comp);
  }

  /// @brief 変化がなくなるまで reduce() を呼ぶ．
  void
  reduce_loop(
    vector<SizeType>& selected_cols,  ///< [out] この縮約で選択された列を格納するベクタ
    vector<SizeType>& deleted_cols,   ///< [out] この縮約で削除された列を格納するベクタ
    const McColComp& col_comp	      ///< [in]  列の比較関数オブジェクト
    = McColComp()
  )
  {
    for ( ; ; ) {
      if ( !reduce(selected_cols, deleted_cols, col_comp) ) {
	break;
      }
    }
  }

  /// @brief 行を削除する．
  void
  delete_row(
    SizeType row_pos ///< [in] 削除する行番号
  )
  {
    mImpl->delete_row(row_pos);
  }

  /// @brief 列を削除する．
  void
  delete_col(
    SizeType col_pos ///< [in] 削除する列番号
  )
  {
    mImpl->delete_col(col_pos);
  }

  /// @brief 行に dirty フラグをつける．
  void
  set_row_dirty(
    SizeType row_pos ///< [in] 行番号
  )
  {
    mImpl->set_row_dirty(row_pos);
  }

  /// @brief 列に dirty フラグをつける．
  void
  set_col_dirty(
    SizeType col_pos ///< [in] 列番号
  )
  {
    mImpl->set_col_dirty(col_pos);
  }

  /// @brief 削除スタックにマーカーを書き込む．
  void
  save()
  {
    mImpl->save();
  }

  /// @brief 直前のマーカーまで処理を戻す．
  void
  restore()
  {
    mImpl->restore();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  unique_ptr<McMatrixImpl> mImpl;

};

#if 0
//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
McMatrix::McMatrix() :
  mImpl{new McMatrixImpl(0, 0)}
{
}

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] elem_list 要素のリスト
//
// * elem_list は (row_pos, col_pos) の pair のリスト
inline
McMatrix::McMatrix(int row_size,
		   int col_size,
		   const vector<pair<int, int>>& elem_list) :
  mImpl{new McMatrixImpl(row_size, col_size, elem_list)}
{
}

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] cost_array コストの配列
// @param[in] elem_list 要素のリスト
//
// * elem_list は (row_pos, col_pos) の pair のリスト
inline
McMatrix::McMatrix(int row_size,
		   const vector<int>& cost_array,
		   const vector<pair<int, int>>& elem_list) :
  mImpl{new McMatrixImpl(row_size, cost_array, elem_list)}
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
McMatrix::McMatrix(const McMatrix& src) :
  mImpl{new McMatrixImpl(*src.mImpl)}
{
}

// @brief コピー代入演算子
// @param[in] src コピー元のオブジェクト
inline
McMatrix&
McMatrix::operator=(const McMatrix& src)
{
  mImpl = unique_ptr<McMatrixImpl>{new McMatrixImpl(*src.mImpl)};

  return *this;
}

// @brief デストラクタ
inline
McMatrix::~McMatrix()
{
}

// @brief 行数を返す．
inline
int
McMatrix::row_size() const
{
  return mImpl->row_size();
}

// @brief アクティブな行の数
inline
int
McMatrix::active_row_num() const
{
  return mImpl->active_row_num();
}

// @brief 行のヘッダのリスト
inline
const McHeadList&
McMatrix::row_head_list() const
{
  return mImpl->row_head_list();
}

// @brief 行方向のリストを返す．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
inline
McRowList
McMatrix::row_list(int row_pos) const
{
  return mImpl->row_list(row_pos);
}

// @brief 行の要素数を返す．
// @param[in] row_pos 行番号 ( 0 <= row_pos < row_size() )
inline
int
McMatrix::row_elem_num(int row_pos) const
{
  return mImpl->row_elem_num(row_pos);
}

// @brief 行の削除フラグを調べる．
// @param[in] row_pos 行番号
inline
bool
McMatrix::row_deleted(int row_pos) const
{
  return mImpl->row_deleted(row_pos);
}

// @brief 列数を返す．
inline
int
McMatrix::col_size() const
{
  return mImpl->col_size();
}

// @brief アクティブな列の数
inline
int
McMatrix::active_col_num() const
{
  return mImpl->active_col_num();
}

// @brief 列のヘッダのリスト
inline
const McHeadList&
McMatrix::col_head_list() const
{
  return mImpl->col_head_list();
}

// @brief 列方向のリストを返す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
McColList
McMatrix::col_list(int col_pos) const
{
  return mImpl->col_list(col_pos);
}

// @brief 列の要素数を返す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McMatrix::col_elem_num(int col_pos) const
{
  return mImpl->col_elem_num(col_pos);
}

// @brief 列の削除フラグを調べる．
inline
bool
McMatrix::col_deleted(int col_pos) const
{
  return mImpl->col_deleted(col_pos);
}

// @brief 列のコストを取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
int
McMatrix::col_cost(int col_pos) const
{
  return mImpl->col_cost(col_pos);
}

// @brief 列のコストの配列を取り出す．
inline
const int*
McMatrix::col_cost_array() const
{
  return mImpl->col_cost_array();
}

// @brief 列集合のコストを返す．
// @param[in] col_list 列のリスト
inline
int
McMatrix::cost(const vector<int>& col_list) const
{
  return mImpl->cost(col_list);
}

// @brief 列集合がカバーになっているか検証する．
// @param[in] col_list 列のリスト
// @retval true col_list がカバーになっている．
// @retval false col_list でカバーされていない行がある．
inline
bool
McMatrix::verify(const vector<int>& col_list) const
{
  return mImpl->verify(col_list);
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
inline
void
McMatrix::print(ostream& s) const
{
  mImpl->print(s);
}

// @brief 内容をクリアする．
// @note 行/列のサイズは不変
inline
void
McMatrix::clear()
{
  mImpl->clear();
}

// @brief サイズを変更する．
// @param[in] row_size 行数
// @param[in] col_size 列数
//
// 内容はクリアされる．
inline
void
McMatrix::resize(int row_size,
		 int col_size)
{
  mImpl = unique_ptr<McMatrixImpl>{new McMatrixImpl(row_size, col_size)};
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
inline
void
McMatrix::insert_elem(int row_pos,
		      int col_pos)
{
  mImpl->insert_elem(row_pos, col_pos);
}

// @brief 要素を追加する．
// @param[in] elem_list 要素のリスト
//
// * 要素は (row_pos, col_pos) のペアで表す．
inline
void
McMatrix::insert_elem(const vector<pair<int, int>>& elem_list)
{
  mImpl->insert_elem(elem_list);
}

// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
// @param[in] col_pos 選択する列番号
inline
void
McMatrix::select_col(int col_pos)
{
  mImpl->select_col(col_pos);
}

// @brief 行列を縮約する．
// @param[out] selected_cols この縮約で選択された列を格納するベクタ
// @param[out] deleted_cols この縮約で削除された列を格納するベクタ
// @param[in] col_comp 列の比較関数オブジェクト
// @retval true 縮約された．
// @retval false 縮約されなかった．
//
// * この関数は行支配，列支配，必須列を１回だけ適用する．
// * この関数が false を返すまで繰り返す必要がある．
// * selected_cols, deleted_cols はこの関数内で初期化されず，
//   追加されるだけなので注意．
inline
bool
McMatrix::reduce(vector<int>& selected_cols,
		 vector<int>& deleted_cols,
		 const McColComp& col_comp)
{
  return mImpl->reduce(selected_cols, deleted_cols, col_comp);
}

// @brief 変化がなくなるまで reduce() を呼ぶ．
// @param[out] selected_cols この縮約で選択された列を格納するベクタ
// @param[out] deleted_cols この縮約で削除された列を格納するベクタ
// @param[in] col_comp 列の比較関数オブジェクト
inline
void
McMatrix::reduce_loop(vector<int>& selected_cols,
		      vector<int>& deleted_cols,
		      const McColComp& col_comp)
{
  for ( ; ; ) {
    if ( !reduce(selected_cols, deleted_cols, col_comp) ) {
      break;
    }
  }
}

// @brief 行を削除する．
// @param[in] row_pos 削除する行番号
inline
void
McMatrix::delete_row(int row_pos)
{
  mImpl->delete_row(row_pos);
}

// @brief 列を削除する．
// @param[in] col_pos 削除する列番号
inline
void
McMatrix::delete_col(int col_pos)
{
  mImpl->delete_col(col_pos);
}

// @brief 行に dirty フラグをつける．
inline
void
McMatrix::set_row_dirty(int row_pos)
{
  mImpl->set_row_dirty(row_pos);
}

// @brief 列に dirty フラグをつける．
inline
void
McMatrix::set_col_dirty(int col_pos)
{
  mImpl->set_col_dirty(col_pos);
}

// @brief 削除スタックにマーカーを書き込む．
inline
void
McMatrix::save()
{
  mImpl->save();
}

// @brief 直前のマーカーまで処理を戻す．
inline
void
McMatrix::restore()
{
  mImpl->restore();
}
#endif

END_NAMESPACE_YM_MINCOV

#endif // YM_MCMATRIX_H
