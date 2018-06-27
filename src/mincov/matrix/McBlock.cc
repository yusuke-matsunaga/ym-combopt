
/// @file McBlock.cc
/// @brief McBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "mincov/McBlock.h"
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
    auto row_head1 = row_head(row_pos);
    if ( row_head1->num() > 0 ) {
      row_head_list.push_back(row_head1);
    }
  }
  mRowHeadList.set(row_head_list);

  vector<McHead*> col_head_list;
  col_head_list.reserve(col_size());
  for ( auto col_pos: Range(col_size()) ) {
    auto col_head1 = col_head(col_pos);
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
    auto row_head1 = row_head(row_pos);
    row_head_list.push_back(row_head1);
  }
  mRowHeadList.set(row_head_list);

  vector<McHead*> col_head_list;
  col_head_list.reserve(col_list.size());
  for ( auto col_pos: col_list ) {
    auto col_head1 = col_head(col_pos);
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
    for ( auto cell: col_head(col_pos)->col_list() ) {
      auto row_pos = cell->row_pos();
      row_mark[row_pos] = true;
    }
  }

  // 印の付いていない行があったらエラー
  for ( auto row_head: mRowHeadList ) {
    if ( !row_mark[row_head->pos()] ) {
      return false;
    }
  }
  return true;
}

// @brief 内容を表示する．
void
McBlock::print(ostream& s) const
{
  for ( auto row_head: mRowHeadList ) {
    s << "Row#" << row_head->pos() << "[" << row_head->num() << "]:";
    int num = 0;
    for ( auto cell: row_head->row_list() ) {
      s << " " << cell->col_pos();
      ++ num;
    }
    s << endl;
    if ( num != row_head->num() ) {
      s << "row_head->num()= " << row_head->num() << ", which really should be " << num << endl;
    }
  }
  for ( auto col_head: mColHeadList ) {
    s << "Col#" << col_head->pos() << "[" << col_head->num() << "]:";
    int num = 0;
    for ( auto cell: col_head->col_list() ) {
      s << " " << cell->row_pos();
      ++ num;
    }
    s << endl;
    if ( num != col_head->num() ) {
      s << "col_head->num()= " << col_head->num() << ", which really should be " << num << endl;
    }
  }
}

// @brief 列を選択し，被覆される行を削除する．
// @param[in] col_pos 選択した列
void
McBlock::select_col(int col_pos)
{
  auto col_head1 = col_head(col_pos);
  ASSERT_COND( !col_head1->mDeleted );

  for ( auto cell: col_head1->col_list() ) {
    queue_row(cell->row_pos());
    //delete_row(cell->row_pos());
  }

  do_delete();
}

// @brief 行を削除する．
// @param[in] row_pos 削除する行番号
void
McBlock::delete_row(int row_pos)
{
  auto row_head1 = row_head(row_pos);
  if ( row_head1->mDeleted ) {
    return;
  }
  mRowHeadList.exclude(row_head1);

  mMatrix.push_row(row_pos);
  for ( auto cell = row_head1->mDummy.mRightLink;
	cell != &row_head1->mDummy; cell = cell->mRightLink ) {
    McCell::col_delete(cell);
    auto col_pos = cell->col_pos();
    auto col_head1 = col_head(col_pos);
    if ( col_head1->dec_num() == 0 ) {
      queue_col(col_pos);
      //delete_col(col_pos);
    }
  }
}

// @brief 行を復元する．
void
McBlock::restore_row(int row_pos)
{
  auto row_head1 = row_head(row_pos);
  ASSERT_COND( row_head1->mDeleted );

  mRowHeadList.restore(row_head1);

  for ( auto cell = row_head1->mDummy.mRightLink;
	cell != &row_head1->mDummy; cell = cell->mRightLink ) {
    McCell::col_insert(cell);
    auto col_pos = cell->col_pos();
    auto col_head1 = col_head(col_pos);
    col_head1->inc_num();
  }
}

// @brief 列を削除する．
// @param[in] col_pos 削除する列番号
void
McBlock::delete_col(int col_pos)
{
  auto col_head1 = col_head(col_pos);
  if ( col_head1->mDeleted ) {
    return;
  }
  mColHeadList.exclude(col_head1);

  mMatrix.push_col(col_pos);
  for ( auto cell = col_head1->mDummy.mDownLink;
	cell != &col_head1->mDummy; cell = cell->mDownLink ) {
    McCell::row_delete(cell);
    auto row_pos = cell->row_pos();
    auto row_head1 = row_head(row_pos);
    if ( row_head1->dec_num() == 0 ) {
      queue_row(row_pos);
      //delete_row(row_pos);
    }
  }
}

// @brief 列を復元する．
void
McBlock::restore_col(int col_pos)
{
  auto col_head1 = col_head(col_pos);
  ASSERT_COND( col_head1->mDeleted );

  mColHeadList.restore(col_head1);

  for ( auto cell = col_head1->mDummy.mDownLink;
	cell != &col_head1->mDummy; cell = cell->mDownLink ) {
    McCell::row_insert(cell);
    auto row_pos = cell->row_pos();
    auto row_head1 = row_head(row_pos);
    row_head1->inc_num();
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

  // 行のマークをクリアする．
  for ( auto row_head1: row_head_list() ) {
    row_head1->mWork = 0;
  }

  for ( auto row_head1: row_head_list() ) {
    if ( row_head1->mWork ) continue;

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    int min_num = row_size() + 1;
    const McHead* min_col = nullptr;
    for ( auto cell: row_head1->row_list() ) {
      int col_pos = cell->col_pos();
      auto col_head1 = col_head(col_pos);
      int col_num = col_head1->num();
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col_head1;
      }
    }
    // min_col に要素を持つ行のうち row1 に支配されている行を求める．
    for ( auto cell: min_col->col_list() ) {
      auto row_head2 = row_head(cell->row_pos());
      if ( row_head2 == row_head1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( row_head2->num() < row_head1->num() ) {
	// 要素数が少ない行も比較しない．
	continue;
      }
      if ( row_head2->mWork ) {
	// 削除された行も比較しない.
	continue;
      }

      // row1 に含まれる要素をすべて row2 が含んでいる場合
      // row1 が row2 を支配している．
      if ( check_containment(row_head2->row_list(), row_head1->row_list()) ) {
	delete_row(row_head2->pos());
	do_delete();
	row_head2->mWork = 1;
	change = true;
	if ( mcblock_debug > 1 ) {
	  cout << "Row#" << row_head2->pos() << " is dominated by Row#" << row_head1->pos() << endl;
	}
      }
    }
  }

  return change;
}

// @brief 列支配を探し，列を削除する．
// @return 削除された列があったら true を返す．
bool
McBlock::col_dominance()
{
  bool change = false;

  for ( auto col_head1: col_head_list() ) {
    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    int min_num = col_size() + 1;
    const McHead* min_row = nullptr;
    for ( auto cell: col_head1->col_list() ) {
      int row_pos = cell->row_pos();
      auto row_head1 = row_head(row_pos);
      int row_num = row_head1->num();
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row_head1;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for ( auto cell: min_row->row_list() ) {
      auto col_head2 = col_head(cell->col_pos());
      if ( col_head2 == col_head1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( col_head2->num() < col_head1->num() ) {
	// ただし col1 よりも要素数の少ない列は調べる必要はない．
	continue;
      }
      if ( col_cost(col_head2->pos()) > col_cost(col_head1->pos()) ) {
	// col_head2 のコストが col_head1 のコストより高ければ調べる必要はない．
	continue;
      }

      // col_head1 に含まれる要素を col_head2 がすべて含んでいる場合
      // col_head2 は col_head1 を支配している．
      if ( check_containment(col_head2->col_list(), col_head1->col_list() ) ) {
	delete_col(col_head1->pos());
	do_delete();
	if ( mcblock_debug > 1 ) {
	  cout << "Col#" << col_head1->pos() << " is dominated by Col#"
	       << col_head2->pos() << endl;
	}
	change = true;
	break;
      }
    }
  }

  return change;
}

// @brief 必須列を探し，列を選択する．
// @param[out] selected_cols 選択された列を追加する列集合
// @return 選択された列があったら true を返す．
bool
McBlock::essential_col(vector<int>& selected_cols)
{
  // マークをクリアする．
  for ( auto col_head1: col_head_list() ) {
    col_head1->mWork = 0;
  }

  int old_size = selected_cols.size();
  for ( auto row_head1: row_head_list() ) {
    if ( row_head1->num() == 1 ) {
      auto cell = row_head1->row_front();
      int col_pos = cell->col_pos();
      ASSERT_COND( !col_head(col_pos)->mDeleted );

      auto col_head1 = col_head(col_pos);
      if ( col_head1->mWork == 0 ) {
	col_head1->mWork = 1;
	selected_cols.push_back(col_pos);
	if ( mcblock_debug > 1 ) {
	  cout << "Col#" << col_pos << " is essential" << endl;
	}
      }
    }
  }
  int size = selected_cols.size();
  for ( auto i: Range(old_size, size) ) {
    int col_pos = selected_cols[i];
    select_col(col_pos);
  }

  return size > old_size;
}

// @brief 行の削除をスケジュールする．
// @param[in] row_pos 削除する行番号
void
McBlock::queue_row(int row_pos)
{
  mMatrix.queue_write(row_pos << 1);
}

// @brief 列の削除をスケジュールする．
// @param[in] col_pos 削除する列番号
void
McBlock::queue_col(int col_pos)
{
  mMatrix.queue_write((col_pos << 1) | 1);
}

// @brief 他の削除から引き起こされた削除を行う．
void
McBlock::do_delete()
{
  while ( !mMatrix.queue_empty() ) {
    int val = mMatrix.queue_read();
    int pos = val >> 1;
    if ( val & 1 ) {
      delete_col(pos);
    }
    else {
      delete_row(pos);
    }
  }
}

// @brief 削除スタックにマーカーを書き込む．
void
McBlock::save()
{
  mMatrix.push_marker();
}

// @brief 直前のマーカーまで処理を戻す．
void
McBlock::restore()
{
  while ( !mMatrix.stack_empty() ) {
    int tmp = mMatrix.pop();
    if ( tmp == 0U ) {
      break;
    }
    if ( tmp & 2U ) {
      int col_pos = tmp >> 2;
      // col_pos の列を元に戻す．
      restore_col(col_pos);
    }
    else {
      int row_pos = tmp >> 2;
      // row_pos の行を元に戻す．
      restore_row(row_pos);
    }
  }
}

END_NAMESPACE_YM_MINCOV
