
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/McMatrix.h"
#include "ym/Range.h"


//#define VERIFY_MCMATRIX 1


BEGIN_NAMESPACE_YM_MINCOV

int mcmatrix_debug = 0;


//////////////////////////////////////////////////////////////////////
// クラス McColComp
//////////////////////////////////////////////////////////////////////

// @brief col1 の代わりに col2 を使っても全体のコストが上がらない時に true を返す．
// @param[in] matrix 対象の行列
// @param[in] col1, col2 対象の列番号
bool
McColComp::operator()(const McMatrix& matrix,
		      int col1,
		      int col2) const
{
  int cost1 = matrix.col_cost(col1);
  int cost2 = matrix.col_cost(col2);
  return cost1 >= cost2;
}


//////////////////////////////////////////////////////////////////////
// クラス McMatrix
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] elem_list 要素のリスト
McMatrix::McMatrix(int row_size,
		   int col_size,
		   const vector<pair<int, int>>& elem_list) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mRowHeadArray(nullptr),
  mRowArray(nullptr),
  mColSize(0),
  mColHeadArray(nullptr),
  mColArray(nullptr),
  mCostArray(nullptr),
  mDelStack(nullptr),
  mRowMark(nullptr),
  mColMark(nullptr),
  mDelList(nullptr)
{
  // サイズを設定する．
  resize(row_size, col_size);

  // コストを設定する．
  for ( auto col: Range(col_size) ) {
    mCostArray[col] = 1;
  }

  // 要素を設定する．
  insert_elem(elem_list);

}

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] cost_array コストの配列
// @param[in] elem_list 要素のリスト
McMatrix::McMatrix(int row_size,
		   const vector<int>& cost_array,
		   const vector<pair<int, int>>& elem_list) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mRowHeadArray(nullptr),
  mRowArray(nullptr),
  mColSize(0),
  mColHeadArray(nullptr),
  mColArray(nullptr),
  mCostArray(nullptr),
  mDelStack(nullptr),
  mRowMark(nullptr),
  mColMark(nullptr),
  mDelList(nullptr)
{
  // サイズを設定する．
  resize(row_size, cost_array.size());

  // コストを設定する．
  for ( auto col: Range(mColSize) ) {
    mCostArray[col] = cost_array[col];
  }

  // 要素を設定する．
  insert_elem(elem_list);

}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
McMatrix::McMatrix(const McMatrix& src) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mRowHeadArray(nullptr),
  mRowArray(nullptr),
  mColSize(0),
  mColHeadArray(nullptr),
  mColArray(nullptr),
  mCostArray(nullptr),
  mDelStack(nullptr),
  mRowMark(nullptr),
  mColMark(nullptr),
  mDelList(nullptr)
{
  // サイズを設定する．
  resize(src.row_size(), src.col_size());

  // 内容をコピーする．
  copy(src);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
const McMatrix&
McMatrix::operator=(const McMatrix& src)
{
  if ( this != &src ) {
    clear();
    resize(src.row_size(), src.col_size());
    copy(src);
  }
  return *this;
}

// @brief デストラクタ
McMatrix::~McMatrix()
{
  clear();
}

// @brief 内容をクリアする．
void
McMatrix::clear()
{
  mCellAlloc.destroy();

  delete [] mRowHeadArray;
  delete [] mRowArray;
  delete [] mColHeadArray;
  delete [] mColArray;
  delete [] mCostArray;
  delete [] mDelStack;
  delete [] mRowMark;
  delete [] mColMark;
  delete [] mDelList;

  mRowHeadArray = nullptr;
  mRowArray = nullptr;
  mColHeadArray = nullptr;
  mColArray = nullptr;
  mCostArray = nullptr;
  mDelStack = nullptr;
  mRowMark = nullptr;
  mColMark = nullptr;
  mDelList = nullptr;
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
McMatrix::insert_elem(int row_pos,
		      int col_pos)
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

  auto cell = alloc_cell(row_pos, col_pos);

  // cell を行方向のリストに挿入する．
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
	free_cell(cell);
	return;
      }
      if ( ncell->col_pos() > col_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      ASSERT_COND( ncell != row_dummy );
    }
  }
  cell->mLeftLink = pcell;
  pcell->mRightLink = cell;
  cell->mRightLink = ncell;
  ncell->mLeftLink = cell;
  row_head->inc_num();
  if ( row_head->num() == 1 ) {
    mRowHeadList.insert(row_head);
  }

  // cell を列方向のリストに挿入する．
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
// @param[in] elem_list 要素のリスト
//
// * 要素は (row_pos, col_pos) のペアで表す．
void
McMatrix::insert_elem(const vector<pair<int, int>>& elem_list)
{
  for ( auto row_col: elem_list ) {
    insert_elem(row_col.first, row_col.second);
  }
}

// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
// @param[in] col_pos 選択する列番号
void
McMatrix::select_col(int col_pos)
{
  for ( auto row_pos: col_list(col_pos) ) {
    delete_row(row_pos);
  }

  ASSERT_COND( col_elem_num(col_pos) == 0 );
  delete_col(col_pos);
}

// @brief 簡単化を行う．
// @param[out] selected_cols 簡単化中で選択された列の集合を追加する配列
// @param[in] col_comp 列の比較関数オブジェクト
void
McMatrix::reduce(vector<int>& selected_cols,
		 const McColComp& col_comp)
{
  if ( mcmatrix_debug > 0 ) {
    cout << "McMatrix::reduce() start: "
	 << active_row_num() << " x " << active_col_num() << endl;
    print(cout);
  }

  int no_change = 0;
  for ( ; ; ) {
    // 列支配を探し，列の削除を行う．
    if ( col_dominance(col_comp) ) {
      no_change = 0;
      if ( mcmatrix_debug > 0 ) {
	cout << " after col_dominance: "
	     << active_row_num() << " x " << active_col_num()  << endl;
	print(cout);
      }
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }

    // 必須列を探し，列の選択を行う．
    if ( essential_col(selected_cols) ) {
      no_change = 0;
      if ( mcmatrix_debug > 0 ) {
	cout << " after essential_col: "
	     << active_row_num() << " x " << active_col_num()  << endl;
	print(cout);
      }
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }

    // 行支配を探し，行の削除を行う．
    if ( row_dominance() ) {
      no_change = 0;
      if ( mcmatrix_debug > 0 ) {
	cout << " after row_dominance: "
	     << active_row_num() << " x " << active_col_num() << endl;
	print(cout);
      }
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }
  }
}

// @brief 行支配による縮約を行う．
// @retval true 縮約が行われた．
// @retval false 縮約が行われなかった．
bool
McMatrix::row_dominance()
{
  bool change = false;

  // 削除する行番号のリスト
  int del_wpos = 0;
  for ( auto row_pos1: row_head_list() ) {
    if ( mRowMark[row_pos1] ) {
      // すでに削除の印がついていたらスキップ
      continue;
    }

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    int min_num = row_size() + 1;
    int min_col = -1;
    for ( auto col_pos: row_list(row_pos1) ) {
      int col_num = col_elem_num(col_pos);
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
// @param[in] col_comp 列の比較関数オブジェクト
// @retval true 縮約が行われた．
// @retval false 縮約が行われなかった．
bool
McMatrix::col_dominance(const McColComp& col_comp)
{
  bool change = false;

  // 要素を持たない列を削除する．
  for ( auto col_pos1: col_head_list() ) {
    if ( col_elem_num(col_pos1) == 0 ) {
      delete_col(col_pos1);
    }
  }

  int del_wpos = 0;
  for ( auto col_pos1: col_head_list() ) {
    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    int min_num = col_size() + 1;
    int min_row = -1;
    for ( auto row_pos: col_list(col_pos1) ) {
      int row_num = row_elem_num(row_pos);
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
      if ( col_comp(*this, col_pos1, col_pos2) ) {
	// col1 を col2 を置き換えてコストが上がらない．

	// col_head1 に含まれる要素を col_head2 がすべて含んでいる場合
	// col_head2 は col_head1 を支配している．
	if ( check_containment(col_list(col_pos2), col_list(col_pos1)) ) {
	  mColMark[col_pos1] = 1;
	  mDelList[del_wpos] = col_pos1;
	  ++ del_wpos;
	  if ( mcmatrix_debug > 1 ) {
	    cout << "Col#" << col_pos1 << " is dominated by Col#"
		 << col_pos2 << endl;
	  }
	  change = true;
	  break;
	}
      }
    }
  }

  // 実際に削除する．
  for ( auto i: Range(del_wpos) ) {
    auto col = mDelList[i];
    delete_col(col);
    mColMark[col] = 0;
  }

  ASSERT_COND( check_mark_sanity() );

  return change;
}

// @brief 必須列による縮約を行う．
// @param[out] selected_cols この縮約で選択された列を格納するベクタ
// @retval true 縮約が行われた．
// @retval false 縮約が行われなかった．
bool
McMatrix::essential_col(vector<int>& selected_cols)
{
  int old_size = selected_cols.size();
  for ( auto row_pos1: row_head_list() ) {
    if ( row_elem_num(row_pos1) == 1 ) {
      int col_pos = row_list(row_pos1).front();
      if ( !mColMark[col_pos] ) {
	mColMark[col_pos] = 1;
	selected_cols.push_back(col_pos);
	if ( mcmatrix_debug > 1 ) {
	  cout << "Col#" << col_pos << " is essential" << endl;
	}
      }
    }
  }
  int size = selected_cols.size();
  for ( auto i: Range(old_size, size) ) {
    int col_pos = selected_cols[i];
    select_col(col_pos);
    mColMark[col_pos] = 0;
  }

  ASSERT_COND( check_mark_sanity() );

  return size > old_size;
}

// @brief 行を削除する．
// @param[in] row_pos 削除する行番号
void
McMatrix::delete_row(int row_pos)
{
  ASSERT_COND( row_pos >= 0 && row_pos < row_size() );

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
McMatrix::restore_row(McHead* row_head)
{
  // ヘッダを復元する．
  mRowHeadList.restore(row_head);

  // 行の要素を復元する．
  int row_pos = row_head->pos();
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
// @param[in] col_pos 削除する列番号
void
McMatrix::delete_col(int col_pos)
{
  ASSERT_COND( col_pos >= 0 && col_pos < col_size() );

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
McMatrix::restore_col(McHead* col_head)
{
  // ヘッダを復元する．
  mColHeadList.restore(col_head);

  // この列の要素を復元する．
  int col_pos = col_head->pos();
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
// @param[in] row_size 行数
// @param[in] col_size 列数
void
McMatrix::resize(int row_size,
		 int col_size)
{
  if ( mRowSize != row_size || mColSize != col_size ) {
    clear();

    mRowSize = row_size;
    mColSize = col_size;

    mRowHeadArray = new McHead[mRowSize];
    mRowArray = new McCell*[mRowSize];
    mRowMark = new int[mRowSize];
    for ( auto row_pos: Range(mRowSize) ) {
      mRowHeadArray[row_pos].init(row_pos, false);
      mRowArray[row_pos] = alloc_cell(row_pos, -1);
      mRowMark[row_pos] = 0;
    }

    mColHeadArray = new McHead[mColSize];
    mColArray = new McCell*[mColSize];
    mCostArray = new int[mColSize];
    mColMark = new int[mColSize];
    for ( auto col_pos: Range(mColSize) ) {
      mColHeadArray[col_pos].init(col_pos, true);
      mColArray[col_pos] = alloc_cell(-1, col_pos);
      mCostArray[col_pos] = 1;
      mColMark[col_pos] = 0;
    }

    mDelStack = new McHead*[row_size + col_size];
    mStackTop = 0;

    int rc_max = mRowSize;
    if ( rc_max < mColSize ) {
      rc_max = mColSize;
    }
    mDelList = new int[rc_max];

    ASSERT_COND( check_mark_sanity() );
  }
}

// @brief 内容をコピーする．
void
McMatrix::copy(const McMatrix& src)
{
  ASSERT_COND(row_size() == src.row_size() );
  ASSERT_COND(col_size() == src.col_size() );

  for ( auto row_pos: Range(row_size()) ) {
    for ( auto col_pos: row_list(row_pos) ) {
      insert_elem(row_pos, col_pos);
    }
  }

  for ( auto col_pos: Range(mColSize) ) {
    mCostArray[col_pos] = src.mCostArray[col_pos];
  }
}

// @brief 列集合のコストを返す．
// @param[in] col_list 列のリスト
int
McMatrix::cost(const vector<int>& col_list) const
{
  int cur_cost = 0;
  for ( auto col: col_list ) {
    cur_cost += col_cost(col);
  }
  return cur_cost;
}

// @brief 列集合がカバーになっているか検証する．
// @param[in] colpos_list 列のリスト
// @retval true colpos_list がカバーになっている．
// @retval false colpos_list でカバーされていない行がある．
bool
McMatrix::verify(const vector<int>& colpos_list) const
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
// @retval true mRowMark, mColMark の内容が全て 0 だった．
// @retval false mRowMark, mColMark に非0の要素が含まれていた．
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
McMatrix::alloc_cell(int row_pos,
		     int col_pos)
{
  void* p = mCellAlloc.get_memory(sizeof(McCell));
  return new (p) McCell(row_pos, col_pos);
}

// @brief セルの解放
void
McMatrix::free_cell(McCell* cell)
{
  mCellAlloc.put_memory(sizeof(McCell), cell);
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
McMatrix::print(ostream& s) const
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
