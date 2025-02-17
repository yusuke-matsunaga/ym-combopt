
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/McMatrix.h"
#include "mincov/McColComp.h"
#include "ym/Range.h"

//#define VERIFY_MCMATRIX 1


BEGIN_NAMESPACE_YM_MINCOV

int mcmatrix_debug = 0;

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

// @brief コンストラクタ
McMatrix::McMatrix(
  SizeType row_size,
  const vector<SizeType>& cost_array,
  const vector<ElemType>& elem_list
) : mRowSize{row_size},
    mRowHeadArray(mRowSize),
    mRowArray(mRowSize, nullptr),
    mColSize{cost_array.size()},
    mColHeadArray(mColSize),
    mColArray(mColSize, nullptr),
    mCostArray{cost_array},
    mDelStack(mRowSize + mColSize),
    mDelList(std::max(mRowSize, mColSize))
{
  for ( auto row_pos: Range(mRowSize) ) {
    mRowHeadArray[row_pos].init(row_pos, false);
    mRowArray[row_pos] = alloc_cell(row_pos, -1);
  }

  for ( auto col_pos: Range(mColSize) ) {
    mColHeadArray[col_pos].init(col_pos, true);
    mColArray[col_pos] = alloc_cell(-1, col_pos);
  }

  // 要素を設定する．
  insert_elem_list(elem_list);
}

#if 0
// @brief コピーコンストラクタ
McMatrix::McMatrix(
  const McMatrix& src
)
{
  // サイズを設定する．
  resize(src.row_size(), src.col_size());

  // 内容をコピーする．
  copy(src);
}

// @brief コピー代入演算子
McMatrix&
McMatrix::operator=(
  const McMatrix& src
)
{
  if ( this != &src ) {
    resize(src.row_size(), src.col_size());
    copy(src);
  }
  return *this;
}
#endif

// @brief デストラクタ
McMatrix::~McMatrix()
{
}

// @brief 要素を追加する．
void
McMatrix::insert_elem(
  const ElemType& elem
)
{
  auto row_pos = elem.row_pos;
  auto col_pos = elem.col_pos;

  // 行方向の挿入位置を求める．
  auto row_head = &mRowHeadArray[row_pos];
  auto row_dummy = mRowArray[row_pos];
  McCell* pcell;
  McCell* ncell;
  if ( row_head->num() == 0 || row_dummy->row_prev()->col_pos() < col_pos ) {
    // 末尾への追加
    ncell = row_dummy;
    pcell = ncell->mLeftLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->col_pos() >= col_pos が成り立っている．
    for ( pcell = row_dummy; ; pcell = ncell ) {
      ncell = pcell->mRightLink;
      if ( ncell->col_pos() == col_pos ) {
	// 列番号が重複しているので無視する．
	return;
      }
      if ( ncell->col_pos() > col_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      ASSERT_COND( ncell != row_dummy );
    }
  }

  auto cell = alloc_cell(row_pos, col_pos);
  cell->mLeftLink = pcell;
  pcell->mRightLink = cell;
  cell->mRightLink = ncell;
  ncell->mLeftLink = cell;
  row_head->inc_num();
  if ( row_head->num() == 1 ) {
    mRowHeadList.insert(row_head);
  }

  // 列方向の挿入位置を求める．
  auto col_head = &mColHeadArray[col_pos];
  auto col_dummy = mColArray[col_pos];
  if ( col_head->num() == 0 || col_dummy->col_prev()->row_pos() < row_pos ) {
    // 末尾への追加
    ncell = col_dummy;
    pcell = ncell->mUpLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->row_pos() >= row_pos が成り立っている．
    for ( pcell = col_dummy; ; pcell = ncell ) {
      ncell = pcell->mDownLink;
      if ( ncell->row_pos() == row_pos ) {
	// 列番号が重複しているので無視する．
	ASSERT_NOT_REACHED;
	return;
      }
      if ( ncell->row_pos() > row_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      ASSERT_COND( ncell != col_dummy );
    }
  }
  cell->mUpLink = pcell;
  pcell->mDownLink = cell;
  cell->mDownLink = ncell;
  ncell->mUpLink = cell;
  col_head->inc_num();
  if ( col_head->num() == 1 ) {
    mColHeadList.insert(col_head);
  }
}

// @brief 要素を追加する．
void
McMatrix::insert_elem_list(
  const vector<ElemType>& elem_list
)
{
  for ( auto& elem: elem_list ) {
    insert_elem(elem);
  }
}

// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
void
McMatrix::select_col(
  SizeType col_pos
)
{
  for ( auto row_pos: col_list(col_pos) ) {
    delete_row(row_pos);
  }

  ASSERT_COND( col_elem_num(col_pos) == 0 );
  delete_col(col_pos);
}

// @brief 行列を縮約する．
bool
McMatrix::reduce(
  vector<SizeType>& selected_cols,
  vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  if ( mcmatrix_debug > 0 ) {
    cout << "McMatrix::reduce() start: "
	 << active_row_num() << " x " << active_col_num() << endl;
    print(cout);
  }

  bool reduced = false;

  // 列支配を探し，列の削除を行う．
  if ( col_dominance(deleted_cols, col_comp) ) {
    reduced = true;
    if ( mcmatrix_debug > 0 ) {
      cout << " after col_dominance: "
	   << active_row_num() << " x " << active_col_num()  << endl;
      print(cout);
    }
  }

  // 必須列を探し，列の選択を行う．
  if ( essential_col(selected_cols) ) {
    reduced = true;
    if ( mcmatrix_debug > 0 ) {
      cout << " after essential_col: "
	   << active_row_num() << " x " << active_col_num()  << endl;
      print(cout);
    }
  }

  // 行支配を探し，行の削除を行う．
  if ( row_dominance() ) {
    reduced = true;
  }

  return reduced;
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

// @brief 行支配による縮約を行う．
bool
McMatrix::row_dominance()
{
  bool change = false;

  // 削除する行番号のリスト
  SizeType del_wpos = 0;
  for ( auto row_pos1: row_head_list() ) {
    if ( mRowMark[row_pos1] ) {
      // すでに削除の印がついていたらスキップ
      continue;
    }

    // row_pos1 の dirty フラグ
    bool dirty1 = mRowHeadArray[row_pos1].is_dirty();

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    SizeType min_num = row_size() + 1;
    SizeType min_col = 0;
    for ( auto col_pos: row_list(row_pos1) ) {
      SizeType col_num = col_elem_num(col_pos);
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col_pos;
      }
    }
    // min_col に要素を持つ行のうち row1 に支配されている行を求める．
    for ( auto row_pos2: col_list(min_col) ) {
      if ( row_pos2 == row_pos1 ) {
	// 自分自身は比較しない．
	continue;
      }

      if ( row_elem_num(row_pos2) < row_elem_num(row_pos1) ) {
	// 要素数が少ない行も比較しない．
	continue;
      }
      if ( mRowMark[row_pos2] ) {
	// 削除された行も比較しない.
	continue;
      }

      // どちらかが dirty でなければチェックする必要はない．
      if ( !dirty1 && !mRowHeadArray[row_pos2].is_dirty() ) {
	continue;
      }

      // row1 に含まれる要素をすべて row2 が含んでいる場合
      // row1 が row2 を支配している．
      if ( check_containment(row_list(row_pos2), row_list(row_pos1)) ) {
	mRowMark[row_pos2] = 1;
	mDelList[del_wpos] = row_pos2;
	++ del_wpos;
	change = true;
	if ( mcmatrix_debug > 1 ) {
	  cout << "Row#" << row_pos2 << " is dominated by Row#"
	       << row_pos1 << endl;
	}
      }
    }
  }
  // 全ての行の dirty フラグを降ろす．
  for ( auto row_pos1: row_head_list() ) {
    mRowHeadArray[row_pos1].set_dirty(false);
  }

  // 実際に削除する．
  for ( auto i: Range(del_wpos) ) {
    auto row = mDelList[i];
    delete_row(row);
    mRowMark[row] = 0;
  }

  ASSERT_COND( check_mark_sanity() );

  return change;
}

// @brief 列支配による縮約を行う．
bool
McMatrix::col_dominance(
  vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  SizeType del_wpos = 0;
  for ( auto col_pos1: col_head_list() ) {
    if ( col_elem_num(col_pos1) == 0 ) {
      // 要素を持たない列は無条件で削除する．
      mDelList[del_wpos] = col_pos1;
      ++ del_wpos;
      continue;
    }

    // col_pos1 の dirty フラグ
    bool dirty1 = mColHeadArray[col_pos1].is_dirty();

    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    SizeType min_num = col_size() + 1;
    SizeType min_row = 0;
    for ( auto row_pos: col_list(col_pos1) ) {
      SizeType row_num = row_elem_num(row_pos);
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row_pos;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for ( auto col_pos2: row_list(min_row) ) {
      if ( col_pos2 == col_pos1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( mColMark[col_pos2] ) {
	// 削除済みならスキップ
	continue;
      }
      if ( col_elem_num(col_pos2) < col_elem_num(col_pos1) ) {
	// ただし col1 よりも要素数の少ない列は調べる必要はない．
	continue;
      }

      // どちらかが dirty でなければチェックする必要はない．
      if ( !dirty1 && !mColHeadArray[col_pos2].is_dirty() ) {
	continue;
      }

      // col1 に含まれる要素を col2 がすべて含んでいる場合
      // col2 は col_head1 を支配している．
      if ( check_containment(col_list(col_pos2), col_list(col_pos1)) ) {
	if ( col_comp(col_pos1, col_pos2) ) {
	  // col1 を col2 を置き換えてコストが上がらない場合には col1 を削除できる．
	  mColMark[col_pos1] = 1;
	  mDelList[del_wpos] = col_pos1;
	  ++ del_wpos;
	  if ( mcmatrix_debug > 1 ) {
	    cout << "Col#" << col_pos1 << " is dominated by Col#"
		 << col_pos2 << endl;
	  }
	  break;
	}
      }
    }
  }
  // すべての列の dirty フラグを降ろしておく．p
  for ( auto col_pos1: col_head_list() ) {
    mColHeadArray[col_pos1].set_dirty(false);
  }

  // 実際に削除する．
  deleted_cols.reserve(deleted_cols.size() + del_wpos);
  for ( auto i: Range(del_wpos) ) {
    auto col = mDelList[i];
    delete_col(col);
    deleted_cols.push_back(col);
    mColMark[col] = 0;
  }

  ASSERT_COND( check_mark_sanity() );

  return del_wpos > 0;
}

// @brief 必須列による縮約を行う．
bool
McMatrix::essential_col(
  vector<SizeType>& selected_cols
)
{
  SizeType old_size = selected_cols.size();
  for ( auto row_pos1: row_head_list() ) {
    if ( row_elem_num(row_pos1) == 1 ) {
      SizeType col_pos = row_list(row_pos1).front();
      if ( !mColMark[col_pos] ) {
	mColMark[col_pos] = 1;
	selected_cols.push_back(col_pos);
	if ( mcmatrix_debug > 1 ) {
	  cout << "Col#" << col_pos << " is essential" << endl;
	}
      }
    }
  }
  SizeType size = selected_cols.size();
  for ( auto i: Range(old_size, size) ) {
    SizeType col_pos = selected_cols[i];
    select_col(col_pos);
    mColMark[col_pos] = 0;
  }

  ASSERT_COND( check_mark_sanity() );

  return size > old_size;
}

// @brief 行を削除する．
void
McMatrix::delete_row(
  SizeType row_pos
)
{
  // ヘッダを削除する．
  auto row_head = &mRowHeadArray[row_pos];
  mRowHeadList.exclude(row_head);
  push(row_head);

  auto dummy = mRowArray[row_pos];
  for ( auto cell = dummy->row_next();
	cell != dummy; cell = cell->row_next() ) {
    // cell を列方向のリンクから切り離す．
    auto prev = cell->col_prev();
    auto next = cell->col_next();
    ASSERT_COND( prev->col_next() == cell );
    ASSERT_COND( next->col_prev() == cell );
    prev->mDownLink = next;
    next->mUpLink = prev;
    // cell の列の要素数を1つ減らす．
    mColHeadArray[cell->col_pos()].dec_num();
  }
}

// @brief 行を復元する．
void
McMatrix::restore_row(
  McHead* row_head
)
{
  // ヘッダを復元する．
  mRowHeadList.restore(row_head);

  // 行の要素を復元する．
  SizeType row_pos = row_head->pos();
  auto dummy = mRowArray[row_pos];
  for ( auto cell = dummy->row_next();
	cell != dummy; cell = cell->row_next() ) {
    // cell を列方向のリンクに戻す．
    auto prev = cell->col_prev();
    auto next = cell->col_next();
    ASSERT_COND( prev->col_next() == next );
    ASSERT_COND( next->col_prev() == prev );
    prev->mDownLink = cell;
    next->mUpLink = cell;
    // cell の列の要素数を1つ増やす．
    mColHeadArray[cell->col_pos()].inc_num();
  }
}

// @brief 列を削除する．
void
McMatrix::delete_col(
  SizeType col_pos
)
{
  // ヘッダを削除する．
  auto col_head = &mColHeadArray[col_pos];
  mColHeadList.exclude(col_head);
  push(col_head);

  auto dummy = mColArray[col_pos];
  for ( auto cell = dummy->col_next();
	cell != dummy; cell = cell->col_next() ) {
    // cell を行方向のリンクから切り離す．
    auto prev = cell->row_prev();
    auto next = cell->row_next();
    ASSERT_COND( prev->row_next() == cell );
    ASSERT_COND( next->row_prev() == cell );
    prev->mRightLink = next;
    next->mLeftLink = prev;
    // cell の行の要素数を1つ減らす．
    mRowHeadArray[cell->row_pos()].dec_num();
  }
}

// @brief 列を復元する．
void
McMatrix::restore_col(
  McHead* col_head
)
{
  // ヘッダを復元する．
  mColHeadList.restore(col_head);

  // この列の要素を復元する．
  SizeType col_pos = col_head->pos();
  auto dummy = mColArray[col_pos];
  for ( auto cell = dummy->col_next();
	cell != dummy; cell = cell->col_next() ) {
    // cell を行方向のリンクに戻す．
    auto prev = cell->row_prev();
    auto next = cell->row_next();
    ASSERT_COND( prev->row_next() == next );
    ASSERT_COND( next->row_prev() == prev) ;
    prev->mRightLink = cell;
    next->mLeftLink = cell;
    // cell の行の要素数を1つ増やす．
    mRowHeadArray[cell->row_pos()].inc_num();
  }
}

// @brief 削除スタックにマーカーを書き込む．
void
McMatrix::save()
{
  push(nullptr);
}

// @brief 直前のマーカーまで処理を戻す．
void
McMatrix::restore()
{
  while ( !stack_empty() ) {
    McHead* head = pop();
    if ( head == nullptr ) {
      break;
    }
    if ( head->is_row() ) {
      restore_row(head);
    }
    else {
      restore_col(head);
    }
  }
}

// @brief サイズを変更する．
void
McMatrix::resize(
  SizeType row_size,
  SizeType col_size
)
{
  mCellList.clear();
  mRowHeadArray.clear();
  mRowArray.clear();
  mRowMark.clear();
  mColHeadArray.clear();
  mColArray.clear();
  mCostArray.clear();
  mColMark.clear();

  mRowSize = row_size;
  mColSize = col_size;

  mRowHeadArray.resize(mRowSize);
  mRowArray.resize(mRowSize, nullptr);
  mRowMark.resize(mRowSize, 0);
  for ( auto row_pos: Range(mRowSize) ) {
    mRowHeadArray[row_pos].init(row_pos, false);
    mRowArray[row_pos] = alloc_cell(row_pos, -1);
  }

  mColHeadArray.resize(mColSize);
  mColArray.resize(mColSize, nullptr);
  mCostArray.resize(mColSize, 1);
  mColMark.resize(mColSize, 0);
  for ( auto col_pos: Range(mColSize) ) {
    mColHeadArray[col_pos].init(col_pos, true);
    mColArray[col_pos] = alloc_cell(-1, col_pos);
  }

  mDelStack.resize(row_size + col_size);
  mStackTop = 0;

  auto rc_max = std::max(mRowSize, mColSize);
  mDelList.resize(rc_max);

  ASSERT_COND( check_mark_sanity() );
}

// @brief 内容をコピーする．
void
McMatrix::copy(
  const McMatrix& src
)
{
  ASSERT_COND( row_size() == src.row_size() );
  ASSERT_COND( col_size() == src.col_size() );

  for ( auto row_pos: Range_<SizeType>(row_size()) ) {
    for ( auto col_pos: row_list(row_pos) ) {
      insert_elem(ElemType{row_pos, col_pos});
    }
  }

  for ( auto col_pos: Range(mColSize) ) {
    mCostArray[col_pos] = src.mCostArray[col_pos];
  }
}

// @brief 列集合のコストを返す．
SizeType
McMatrix::cost(
  const vector<SizeType>& col_list
) const
{
  SizeType cur_cost = 0;
  for ( auto col: col_list ) {
    cur_cost += col_cost(col);
  }
  return cur_cost;
}

// @brief 列集合がカバーになっているか検証する．
bool
McMatrix::verify(
  const vector<SizeType>& colpos_list
) const
{
  // カバーされた行の印
  vector<bool> row_mark(row_size(), false);

  // col_list の列でカバーされた行に印をつける．
  for ( auto col_pos: colpos_list ) {
    for ( auto row_pos: col_list(col_pos) ) {
      row_mark[row_pos] = true;
    }
  }

  // 印の付いていない行があったらエラー
  for ( auto row_pos: Range(row_size()) ) {
    if ( !row_mark[row_pos] ) {
      return false;
    }
  }
  return true;
}

// @brief mRowMark, mColMark の sanity check
bool
McMatrix::check_mark_sanity()
{
  for ( auto row: Range(row_size()) ) {
    if ( mRowMark[row] != 0 ) {
      return false;
    }
  }
  for ( auto col: Range(col_size()) ) {
    if ( mColMark[col] != 0 ) {
      return false;
    }
  }
  return true;
}

// @brief セルの生成
McCell*
McMatrix::alloc_cell(
  SizeType row_pos,
  SizeType col_pos
)
{
  auto cell = new McCell(row_pos,  col_pos);
  mCellList.push_back(std::unique_ptr<McCell>{cell});
  return cell;
}

// @brief 内容を出力する．
void
McMatrix::print(
  ostream& s
) const
{
  for ( auto col_pos: Range(col_size()) ) {
    if ( col_cost(col_pos) != 1 ) {
      s << "Col#" << col_pos << ": " << col_cost(col_pos) << endl;
    }
  }
  for ( auto row_pos: Range(row_size()) ) {
    s << "Row#" << row_pos << ":";
    for ( auto col_pos: row_list(row_pos) ) {
      s << " " << col_pos;
    }
    s << endl;
  }
}

END_NAMESPACE_YM_MINCOV
