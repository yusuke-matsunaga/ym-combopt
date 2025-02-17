#ifndef YM_MCMATRIX_H
#define YM_MCMATRIX_H

/// @file ym/McMatrix.h
/// @brief McMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MinCov.h"
#include "mincov/mincov_nsdef.h"
#include "mincov/McHead.h"
#include "mincov/McHeadList.h"
#include "mincov/McRowList.h"
#include "mincov/McColList.h"
#include "mincov/McColComp.h"


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
  using ElemType = MinCov::ElemType;

public:

  /// @brief コンストラクタ
  McMatrix(
    SizeType row_size,                ///< [in] 行数
    SizeType col_size,		      ///< [in] 列数
    const vector<ElemType>& elem_list ///< [in] 要素のリスト
    = vector<ElemType>{}
  ) : McMatrix{row_size, vector<SizeType>(col_size, 1), elem_list}
  {
  }

  /// @brief コンストラクタ
  ///
  /// 列数は cost_array.size() から得る．
  McMatrix(
    SizeType row_size,                  ///< [in] 行数
    const vector<SizeType>& cost_array, ///< [in] コストの配列
    const vector<ElemType>& elem_list   ///< [in] 要素のリスト
    = vector<ElemType>{}
  );

#if 0
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
#endif

  /// @brief デストラクタ
  ~McMatrix();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  SizeType
  row_size() const
  {
    return mRowSize;
  }

  /// @brief アクティブな行の数
  SizeType
  active_row_num() const
  {
    return mRowHeadList.num();
  }

  /// @brief アクティブな行のヘッダのリスト
  const McHeadList&
  row_head_list() const
  {
    return mRowHeadList;
  }

  /// @brief 行方向のリストを返す．
  McRowList
  row_list(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const
  {
    auto dummy = mRowArray[row_pos];
    return McRowList(dummy->row_next(), dummy);
  }

  /// @brief 行の要素数を返す．
  SizeType
  row_elem_num(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const
  {
    return mRowHeadArray[row_pos].num();
  }

  /// @brief 行の削除フラグを調べる．
  bool
  row_deleted(
    SizeType row_pos ///< [in] 行番号 ( 0 <= row_pos < row_size() )
  ) const
  {
    return mRowHeadArray[row_pos].is_deleted();
  }

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mColSize;
  }

  /// @brief アクティブな列の数
  SizeType
  active_col_num() const
  {
    return mColHeadList.num();
  }

  /// @brief アクティブな列のヘッダのリスト
  const McHeadList&
  col_head_list() const
  {
    return mColHeadList;
  }

  /// @brief 列方向のリストを返す．
  McColList
  col_list(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    auto dummy = mColArray[col_pos];
    return McColList(dummy->col_next(), dummy);
  }

  /// @brief 列の要素数を返す．
  SizeType
  col_elem_num(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mColHeadArray[col_pos].num();
  }

  /// @brief 列の削除フラグを調べる．
  bool
  col_deleted(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mColHeadArray[col_pos].is_deleted();
  }

  /// @brief 列のコストを取り出す．
  SizeType
  col_cost(
    SizeType col_pos ///< [in] 列位置 ( 0 <= col_pos < col_size() )
  ) const
  {
    return mCostArray[col_pos];
  }

  /// @brief 列のコストの配列を取り出す．
  const vector<SizeType>&
  col_cost_array() const
  {
    return mCostArray;
  }

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

  /// @brief 要素を追加する．
  void
  insert_elem(
    const ElemType& elem ///< [in] 追加する要素
  );

  /// @brief 要素のリストを追加する．
  void
  insert_elem_list(
    const vector<ElemType>& elem_list ///< [in] 要素のリスト
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
  )
  {
    mRowHeadArray[row_pos].set_dirty(false);
  }

  /// @brief 列に dirty フラグをつける．
  void
  set_col_dirty(
    SizeType col_pos ///< [in] 列番号
  )
  {
    mColHeadArray[col_pos].set_dirty(true);
  }

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
  ///
  /// 内容はクリアされる．
  void
  resize(
    SizeType row_size, ///< [in] 行数
    SizeType col_size  ///< [in] 列数
  );

  /// @brief 内容をコピーする．
  void
  copy(
    const McMatrix& src
  );

  /// @brief 行支配による縮約を行う．
  /// @retval true 縮約が行われた．
  /// @retval false 縮約が行われなかった．
  bool
  row_dominance();

  /// @brief 列支配による縮約を行う．
  /// @retval true 縮約が行われた．
  /// @retval false 縮約が行われなかった．
  bool
  col_dominance(
    vector<SizeType>& deleted_cols, ///< [out] この縮約で削除された列を格納するベクタ
    const McColComp& col_comp	    ///< [in]  列の比較関数オブジェクト
  );

  /// @brief 必須列による縮約を行う．
  /// @retval true 縮約が行われた．
  /// @retval false 縮約が行われなかった．
  bool
  essential_col(
    vector<SizeType>& selected_cols ///< [out] この縮約で選択された列を格納するベクタ
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

  /// @brief スタックが空の時 true を返す．
  bool
  stack_empty()
  {
    return mStackTop == 0;
  }

  /// @brief スタックに削除した行/列のヘッダと積む．
  void
  push(
    McHead* head
  )
  {
    mDelStack[mStackTop] = head;
    ++ mStackTop;
  }

  /// @brief スタックから取り出す．
  McHead*
  pop()
  {
    -- mStackTop;
    return mDelStack[mStackTop];
  }

  /// @brief 行の削除フラグをセットする．
  void
  set_row_deleted(
    SizeType row_pos, ///< [in] 行番号
    bool flag	      ///< [in] フラグの値
  )
  {
    mRowHeadArray[row_pos].set_deleted(flag);
  }

  /// @brief 列の削除フラグをセットする．
  void
  set_col_deleted(
    SizeType col_pos, ///< [in] 列番号
    bool flag	      ///< [in] フラグの値
  )
  {
    mColHeadArray[col_pos].set_deleted(flag);
  }

  /// @brief mRowMark, mColMark の sanity check
  /// @retval true mRowMark, mColMark の内容が全て 0 だった．
  /// @retval false mRowMark, mColMark に非0の要素が含まれていた．
  bool
  check_mark_sanity();

  /// @brief セルの生成
  McCell*
  alloc_cell(
    SizeType row_pos, ///< [in] 行番号
    SizeType col_pos  ///< [in] 列番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行数
  SizeType mRowSize{0};

  // 行のヘッダの配列
  // サイズは mRowSize
  vector<McHead> mRowHeadArray;

  // 現在アクティブな行のヘッダのリスト
  McHeadList mRowHeadList;

  // 行の先頭を表すダミーセルの配列
  // サイズは mRowSize
  vector<McCell*> mRowArray;

  // 列数
  SizeType mColSize{0};

  // 列のヘッダの配列
  // サイズは mColSize
  vector<McHead> mColHeadArray;

  // 現在アクティブな列のヘッダのリスト
  McHeadList mColHeadList;

  // 列の先頭を表すダミーセルの配列
  // サイズは mColSize
  vector<McCell*> mColArray;

  // コストの配列
  // サイズは mColSize;
  vector<SizeType> mCostArray;

  // 確保したセルの所有権を持つ．
  vector<std::unique_ptr<McCell>> mCellList;

  // 削除の履歴を覚えておくスタック
  vector<McHead*> mDelStack;

  // mDelStack のポインタ
  SizeType mStackTop{0};

  // 作業用に使う行のマーク配列
  // サイズは mRowSize
  mutable
  vector<SizeType> mRowMark;

  // 作業用に使う列のマーク配列
  // サイズは mColSize
  mutable
  vector<SizeType> mColMark;

  // 作業用に使う配列
  // サイズは max(mRowSize, mColSize)
  mutable
  vector<SizeType> mDelList;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MCMATRIX_H
