
/// @file McBlock.cc
/// @brief McBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/McBlock.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

int mcblock_debug = 0;

//////////////////////////////////////////////////////////////////////
// クラス McBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[int] matrix 対象の行列
//
// 行列全体を対象のブロックとする．
McBlock::McBlock(McMatrix& matrix) :
  mMatrix(matrix)
{
  vector<McHead*> row_head_list;
  row_head_list.reserve(row_size());
  for ( auto row_pos: Range(row_size()) ) {
    auto row_head1 = mMatrix._row_head(row_pos);
    if ( row_head1->num() > 0 ) {
      row_head_list.push_back(row_head1);
    }
  }
  mRowHeadList.set(row_head_list);

  vector<McHead*> col_head_list;
  col_head_list.reserve(col_size());
  for ( auto col_pos: Range(col_size()) ) {
    auto col_head1 = mMatrix._col_head(col_pos);
    if ( col_head1->num() > 0 ) {
      col_head_list.push_back(col_head1);
    }
  }
  mColHeadList.set(col_head_list);
}

// @brief コンストラクタ
// @param[int] matrix 対象の行列
//
// 指定された行と列のみを対象のブロックとする．
McBlock::McBlock(McMatrix& matrix,
		 const vector<int>& row_list,
		 const vector<int>& col_list) :
  mMatrix(matrix)
{
  vector<McHead*> row_head_list;
  row_head_list.reserve(row_list.size());
  for ( auto row_pos: row_list ) {
    auto row_head1 = mMatrix._row_head(row_pos);
    row_head_list.push_back(row_head1);
  }
  mRowHeadList.set(row_head_list);

  vector<McHead*> col_head_list;
  col_head_list.reserve(col_list.size());
  for ( auto col_pos: col_list ) {
    auto col_head1 = mMatrix._col_head(col_pos);
    col_head_list.push_back(col_head1);
  }
  mColHeadList.set(col_head_list);
}

// @brief デストラクタ
McBlock::~McBlock()
{
}

// @brief 列集合のコストを返す．
// @param[in] col_list 列のリスト
int
McBlock::cost(const vector<int>& col_list) const
{
  int sum_cost = 0;
  for ( auto col_pos: col_list ) {
    sum_cost += col_cost(col_pos);
  }
  return sum_cost;
}

// @brief 列集合がカバーになっているか検証する．
// @param[in] col_list 列のリスト
// @retval true col_list がカバーになっている．
// @retval false col_list でカバーされていない行がある．
bool
McBlock::verify(const vector<int>& col_list) const
{
  // カバーされた行の印
  vector<bool> row_mark(row_size(), false);

  // col_list の列でカバーされた行に印をつける．
  for ( auto col_pos: col_list ) {
    for ( auto row_pos: mMatrix.col_list(col_pos) ) {
      row_mark[row_pos] = true;
    }
  }

  // 印の付いていない行があったらエラー
  for ( auto row_pos: mRowHeadList ) {
    if ( !row_mark[row_pos] ) {
      return false;
    }
  }
  return true;
}

// @brief 内容を表示する．
void
McBlock::print(ostream& s) const
{
  for ( auto row_pos: mRowHeadList ) {
    s << "Row#" << row_pos << "[" << row_elem_num(row_pos) << "]:";
    int num = 0;
    for ( auto col_pos: row_list(row_pos) ) {
      s << " " << col_pos;
      ++ num;
    }
    s << endl;
    if ( num != row_elem_num(row_pos) ) {
      s << "row_elem_num(" << row_pos << ") = "
	<< row_elem_num(row_pos) << ", which really should be " << num << endl;
    }
  }
  for ( auto col_pos: mColHeadList ) {
    s << "Col#" << col_pos << "[" << col_elem_num(col_pos) << "]:";
    int num = 0;
    for ( auto row_pos: col_list(col_pos) ) {
      s << " " << row_pos;
      ++ num;
    }
    s << endl;
    if ( num != col_elem_num(col_pos) ) {
      s << "col_elem_num(" << col_pos << ") = "
	<< col_elem_num(col_pos) << ", which really should be " << num << endl;
    }
  }
}

// @brief 列を選択し，被覆される行を削除する．
// @param[in] col_pos 選択した列
void
McBlock::select_col(int col_pos)
{
  auto col_head1 = mMatrix._col_head(col_pos);
  ASSERT_COND( !col_head1->is_deleted() );

  for ( auto row_pos: col_list(col_pos) ) {
    delete_row(row_pos);
  }

  ASSERT_COND( col_head1->num() == 0 );
  delete_col(col_pos);
}

// @brief 行を削除する．
// @param[in] row_pos 削除する行番号
void
McBlock::delete_row(int row_pos)
{
  auto row_head1 = mMatrix._row_head(row_pos);
  ASSERT_COND( !row_head1->is_deleted() );

  mRowHeadList.exclude(row_head1);
  mMatrix.push(row_head1);
  for ( auto cell = row_head1->row_begin();
	cell != row_head1->row_end(); cell = cell->row_next() ) {
    auto col_head1 = mMatrix._col_head(cell->col_pos());
    col_head1->col_delete(cell);
  }
}

// @brief 行を復元する．
void
McBlock::restore_row(McHead* row_head1)
{
  ASSERT_COND( row_head1->is_deleted() );

  mRowHeadList.restore(row_head1);
  for ( auto cell = row_head1->row_begin();
	cell != row_head1->row_end(); cell = cell->row_next() ) {
    auto col_head1 = mMatrix._col_head(cell->col_pos());
    col_head1->col_restore(cell);
  }
}

// @brief 列を削除する．
// @param[in] col_pos 削除する列番号
void
McBlock::delete_col(int col_pos)
{
  auto col_head1 = mMatrix._col_head(col_pos);
  ASSERT_COND( !col_head1->is_deleted() );

  mColHeadList.exclude(col_head1);
  mMatrix.push(col_head1);
  for ( auto cell = col_head1->col_begin();
	cell != col_head1->col_end(); cell = cell->col_next() ) {
    auto row_head1 = mMatrix._row_head(cell->row_pos());
    row_head1->row_delete(cell);
  }
}

// @brief 列を復元する．
void
McBlock::restore_col(McHead* col_head1)
{
  ASSERT_COND( col_head1->is_deleted() );

  mColHeadList.restore(col_head1);
  for ( auto cell = col_head1->col_begin();
	cell != col_head1->col_end(); cell = cell->col_next() ) {
    auto row_head1 = mMatrix._row_head(cell->row_pos());
    row_head1->row_restore(cell);
  }
}

// @brief 簡単化を行う．
// @param[out] selected_cols 簡単化中で選択された列の集合を追加する配列
void
McBlock::reduce(vector<int>& selected_cols)
{
  if ( mcblock_debug > 0 ) {
    cout << "McMatrix::reduce() start: " << row_num() << " x " << col_num() << endl;
    print(cout);
  }

  int no_change = 0;
  for ( ; ; ) {
    // 列支配を探し，列の削除を行う．
    if ( col_dominance() ) {
      no_change = 0;
      if ( mcblock_debug > 0 ) {
	cout << " after col_dominance: " << row_num() << " x " << col_num()  << endl;
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
      if ( mcblock_debug > 0 ) {
	cout << " after essential_col: " << row_num() << " x " << col_num()  << endl;
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
      if ( mcblock_debug > 0 ) {
	cout << " after row_dominance: "  << row_num() << " x " << col_num() << endl;
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

// @brief 行支配を探し，行を削除する．
// @return 削除された行があったら true を返す．
bool
McBlock::row_dominance()
{
  bool change = false;

  // 削除する行番号のリスト
  vector<int> del_list;
  del_list.reserve(row_num());
  for ( auto row_pos1: row_head_list() ) {
    if ( mMatrix.mRowMark[row_pos1] ) {
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
      if ( mMatrix.mRowMark[row_pos2] ) {
	// 削除された行も比較しない.
	continue;
      }

      // row1 に含まれる要素をすべて row2 が含んでいる場合
      // row1 が row2 を支配している．
      if ( check_containment(row_list(row_pos2), row_list(row_pos1)) ) {
	mMatrix.mRowMark[row_pos2] = 1;
	del_list.push_back(row_pos2);
	change = true;
	if ( mcblock_debug > 1 ) {
	  cout << "Row#" << row_pos2 << " is dominated by Row#"
	       << row_pos1 << endl;
	}
      }
    }
  }

  // 実際に削除する．
  for ( auto row: del_list ) {
    delete_row(row);
    mMatrix.mRowMark[row] = 0;
  }

  ASSERT_COND( mMatrix.check_mark_sanity() );

  return change;
}

// @brief 列支配を探し，列を削除する．
// @return 削除された列があったら true を返す．
bool
McBlock::col_dominance()
{
  bool change = false;

  // 要素を持たない列を削除する．
  for ( auto col_pos1: col_head_list() ) {
    if ( col_elem_num(col_pos1) == 0 ) {
      delete_col(col_pos1);
    }
  }

  // 削除する列番号のリスト
  vector<int> del_list;
  del_list.reserve(col_num());
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
      if ( mMatrix.mColMark[col_pos2] ) {
	// 削除済みならスキップ
	continue;
      }
      if ( col_elem_num(col_pos2) < col_elem_num(col_pos1) ) {
	// ただし col1 よりも要素数の少ない列は調べる必要はない．
	continue;
      }
      if ( col_cost(col_pos2) > col_cost(col_pos1) ) {
	// col2 のコストが col1 のコストより高ければ調べる必要はない．
	continue;
      }

      // col_head1 に含まれる要素を col_head2 がすべて含んでいる場合
      // col_head2 は col_head1 を支配している．
      if ( check_containment(col_list(col_pos2), col_list(col_pos1)) ) {
	mMatrix.mColMark[col_pos1] = 1;
	del_list.push_back(col_pos1);
	if ( mcblock_debug > 1 ) {
	  cout << "Col#" << col_pos1 << " is dominated by Col#"
	       << col_pos2 << endl;
	}
	change = true;
	break;
      }
    }
  }

  // 実際に削除する．
  for ( auto col: del_list ) {
    delete_col(col);
    mMatrix.mColMark[col] = 0;
  }

  ASSERT_COND( mMatrix.check_mark_sanity() );

  return change;
}

// @brief 必須列を探し，列を選択する．
// @param[out] selected_cols 選択された列を追加する列集合
// @return 選択された列があったら true を返す．
bool
McBlock::essential_col(vector<int>& selected_cols)
{
  int old_size = selected_cols.size();
  for ( auto row_pos1: row_head_list() ) {
    if ( row_elem_num(row_pos1) == 1 ) {
      int col_pos = row_list(row_pos1).front();
      if ( mMatrix.mColMark[col_pos] ) {
	continue;
      }

      auto col_head1 = mMatrix._col_head(col_pos);
      ASSERT_COND( !col_head1->is_deleted() );

      mMatrix.mColMark[col_pos] = 1;
      selected_cols.push_back(col_pos);
      if ( mcblock_debug > 1 ) {
	cout << "Col#" << col_pos << " is essential" << endl;
      }
    }
  }
  int size = selected_cols.size();
  for ( auto i: Range(old_size, size) ) {
    int col_pos = selected_cols[i];
    select_col(col_pos);
    mMatrix.mColMark[col_pos] = 0;
  }

  ASSERT_COND( mMatrix.check_mark_sanity() );

  return size > old_size;
}

// @brief ブロック分割する．
// @param[out] remainder 分割された他方のブロックを入れる変数
// @retval true 分割できた．
// @retval false 分割できなかった．
bool
McBlock::partition(McBlock& remainder)
{
  vector<bool> row_mark(row_size(), false);
  vector<bool> col_mark(col_size(), false);

  // 最初の行に接続している列にマークをつける．
  auto row0 = row_head_list().front();
  int row0_pos = row0->pos();
  row_mark[row0_pos] = true;
  int nc1 = mark_cols(row0->pos(), row_mark, col_mark);
  int nc = col_num();
  if ( nc1 == nc ) {
    // 分割できなかった．
    return false;
  }

  // row_mark, col_mark に印のない行と列を remainder に移す．
  vector<McHead*> row_list1;
  vector<McHead*> row_list2;
  row_list1.reserve(row_num());
  row_list2.reserve(row_num());
  for ( auto row_pos: mRowHeadList ) {
    auto row_head1 = mMatrix._row_head(row_pos);
    if ( row_mark[row_pos] ) {
      row_list1.push_back(row_head1);
    }
    else {
      row_list2.push_back(row_head1);
    }
  }

  vector<McHead*> col_list1;
  vector<McHead*> col_list2;
  col_list1.reserve(col_num());
  col_list2.reserve(col_num());
  for ( auto col_pos: mColHeadList ) {
    auto col_head1 = mMatrix._col_head(col_pos);
    if ( col_mark[col_pos] ) {
      col_list1.push_back(col_head1);
    }
    else {
      col_list2.push_back(col_head1);
    }
  }

  mRowHeadList.set(row_list1);
  mColHeadList.set(col_list1);

  remainder.mRowHeadList.set(row_list2);
  remainder.mColHeadList.set(col_list2);
  return true;
}

// @brief 列に接続している行に印をつける．
// @param[in] col_pos 開始する列番号
// @param[inout] row_mark 行の印
// @param[inout] col_mark 列の印
// @return 印の付けられた列数を返す．
int
McBlock::mark_rows(int col_pos,
		   vector<bool>& row_mark,
		   vector<bool>& col_mark)
{
  int nc = 0;
  for ( auto row_pos: col_list(col_pos) ) {
    if ( !row_mark[row_pos] ) {
      row_mark[row_pos] = true;
      nc += mark_cols(row_pos, row_mark, col_mark);
    }
  }
  return nc;
}

// @brief 行に接続している列に印をつける．
// @param[in] row_pos 開始する行番号
// @param[inout] row_mark 行の印
// @param[inout] col_mark 列の印
// @return 印の付けられた列数を返す．
int
McBlock::mark_cols(int row_pos,
		   vector<bool>& row_mark,
		   vector<bool>& col_mark)
{
  int nc = 0;
  for ( auto col_pos: row_list(row_pos) ) {
    if ( !col_mark[col_pos] ) {
      col_mark[col_pos] = true;
      ++ nc;
      nc += mark_rows(col_pos, row_mark, col_mark);
    }
  }
  return nc;
}

// @brief 削除スタックにマーカーを書き込む．
void
McBlock::save()
{
  mMatrix.push(nullptr);
}

// @brief 直前のマーカーまで処理を戻す．
void
McBlock::restore()
{
  while ( !mMatrix.stack_empty() ) {
    McHead* head = mMatrix.pop();
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

END_NAMESPACE_YM_MINCOV
