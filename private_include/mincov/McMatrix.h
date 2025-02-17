#ifndef YM_MCMATRIX_H
#define YM_MCMATRIX_H

/// @file ym/McMatrix.h
/// @brief McMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "mincov/McHeadList.h"
#include "mincov/McColComp.h"
#include "mincov/McColList.h"
#include "mincov/McRowList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McMatrix McMatrix.h "mincov/McMatrix.h"
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
  McMatrix();

  /// @brief コンストラクタ
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrix(
    SizeType row_size,                                ///< [in] 行数
    SizeType col_size,				      ///< [in] 列数
    const vector<pair<SizeType, SizeType>>& elem_list ///< [in] 要素のリスト
    = vector<pair<SizeType, SizeType>>{}
  );

  /// @brief コンストラクタ
  ///
  /// * elem_list は (row_pos, col_pos) の pair のリスト
  McMatrix(
    SizeType row_size,                                ///< [in] 行数
    const vector<SizeType>& cost_array,               ///< [in] コストの配列
    const vector<pair<SizeType, SizeType>>& elem_list ///< [in] 要素のリスト
    = vector<pair<SizeType, SizeType>>{}
  );

  /// @brief コピーコンストラクタ
  McMatrix(
    const McMatrix& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  McMatrix&
  operator=(
    const McMatrix& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  McMatrix(
    McMatrix&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  McMatrix&
  operator=(
    McMatrix&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~McMatrix();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  SizeType
  row_size() const;

  /// @brief アクティブな行の数
  SizeType
  active_row_num() const;

  /// @brief アクティブな行のヘッダのリスト
  const McHeadList&
  row_head_list() const;

  /// @brief 行方向のリストを返す．
  McRowList
  row_list(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const;

  /// @brief 行の要素数を返す．
  SizeType
  row_elem_num(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const;

  /// @brief 行の削除フラグを調べる．
  bool
  row_deleted(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const;

  /// @brief 列数を返す．
  SizeType
  col_size() const;

  /// @brief アクティブな列の数
  SizeType
  active_col_num() const;

  /// @brief アクティブな列のヘッダのリスト
  const McHeadList&
  col_head_list() const;

  /// @brief 列方向のリストを返す．
  McColList
  col_list(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const;

  /// @brief 列の要素数を返す．
  SizeType
  col_elem_num(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const;

  /// @brief 列の削除フラグを調べる．
  bool
  col_deleted(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const;

  /// @brief 列のコストを取り出す．
  SizeType
  col_cost(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const;

  /// @brief 列のコストの配列を取り出す．
  const vector<SizeType>&
  col_cost_array() const;

  /// @brief 列集合のコストを返す．
  SizeType
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

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  /// @note 行/列のサイズは不変
  void
  clear();

  /// @brief サイズを変更する．
  ///
  /// 内容はクリアされる．
  void
  resize(
    SizeType row_size, ///< [in] 行数
    SizeType col_size  ///< [in] 列数
  );

  /// @brief 要素を追加する．
  void
  insert_elem(
    SizeType row_pos, ///< [in] 追加する要素の行番号
    SizeType col_pos  ///< [in] 追加する要素の列番号
  );

  /// @brief 要素を追加する．
  ///
  /// * 要素は (row_pos, col_pos) のペアで表す．
  void
  insert_elem(
    const vector<pair<SizeType, SizeType>>& elem_list ///< [in] 要素のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を変更する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
  void
  select_col(
    SizeType col_pos ///< [in] 選択する列番号
  );

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
  );

  /// @brief 変化がなくなるまで reduce() を呼ぶ．
  void
  reduce_loop(
    vector<SizeType>& selected_cols,  ///< [out] この縮約で選択された列を格納するベクタ
    vector<SizeType>& deleted_cols,   ///< [out] この縮約で削除された列を格納するベクタ
    const McColComp& col_comp	      ///< [in]  列の比較関数オブジェクト
    = McColComp{}
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

  /// @brief 行に dirty フラグをつける．
  void
  set_row_dirty(
    SizeType row_pos ///< [in] 行番号
  );

  /// @brief 列に dirty フラグをつける．
  void
  set_col_dirty(
    SizeType col_pos ///< [in] 列番号
  );

  /// @brief 削除スタックにマーカーを書き込む．
  void
  save();

  /// @brief 直前のマーカーまで処理を戻す．
  void
  restore();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  unique_ptr<McMatrixImpl> mImpl;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MCMATRIX_H
