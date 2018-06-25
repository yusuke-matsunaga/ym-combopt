
/// @file McBlock.cc
/// @brief McBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "McBlock.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[int] matrix 対象の行列
//
// 行列全体を対象のブロックとする．
McBlock::McBlock(const McMatrix& matrix) :
  mMatrix(matrix),
  mDelStack(nullptr)
{
  mStackTop = 0;
}

// @brief デストラクタ
McBlock::~McBlock()
{
  delete [] mDelStack;
}

// @brief 列を選択し，被覆される行を削除する．
// @param[in] col_pos 選択した列
void
McBlock::select_col(int col_pos)
{
  auto col1 = col(col_pos);
  ASSERT_COND( !col1->mDeleted );

  vector<int> row_pos_list;
  row_pos_list.reserve(col1->num());
  for ( auto cell: col1->col_list() ) {
    row_pos_list.push_back(cell->row_pos());
  }
  delete_col(col_pos);
  for ( auto row_pos: row_pos_list ) {
    delete_row(row_pos);
  }
}

// @brief 行を削除する．
// @param[in] row_pos 削除する行番号
void
McBlock::delete_row(int row_pos)
{
  auto row1 = row(row_pos);
  if ( row1->mDeleted ) {
    return;
  }
  mRowList.exclude(row1);

  for ( auto cell = row1->mDummy.mRightLink;
	cell != &row1->mDummy; cell = cell->mRightLink ) {
    McCell::col_delete(cell);
    int col_pos = cell->col_pos();
    auto col1 = col(col_pos);
    if ( col1->dec_num() == 0 ) {
      delete_col(col_pos);
    }
  }

  push_row(row_pos);
}

// @brief 行を復元する．
void
McBlock::restore_row(int row_pos)
{
  auto row1 = row(row_pos);
  mRowList.restore(row1);

  for ( auto cell = row1->mDummy.mRightLink;
	cell != &row1->mDummy; cell = cell->mRightLink ) {
    McCell::col_insert(cell);
  }
}

// @brief 列を削除する．
// @param[in] col_pos 削除する列番号
void
McBlock::delete_col(int col_pos)
{
  auto col1 = col(col_pos);
  if ( col1->mDeleted ) {
    return;
  }
  mColList.exclude(col1);

  for ( auto cell = col1->mDummy.mDownLink;
	cell != &col1->mDummy; cell = cell->mDownLink ) {
    McCell::row_delete(cell);
    int row_pos = cell->row_pos();
    auto row1 = row(row_pos);
    if ( row1->dec_num() == 0 ) {
      delete_row(row_pos);
    }
  }

  push_col(col_pos);
}

// @brief 列を復元する．
void
McBlock::restore_col(int col_pos)
{
  auto col1 = col(col_pos);
  ASSERT_COND( col1->mDeleted );

  mColList.restore(col1);

  for ( auto cell = col1->mDummy.mDownLink;
	cell != &col1->mDummy; cell = cell->mDownLink ) {
    McCell::row_insert(cell);
  }
}

// @brief 簡単化を行う．
// @param[out] selected_cols 簡単化中で選択された列の集合を追加する配列
void
McBlock::reduce(vector<int>& selected_cols)
{
  if ( mcmatrix_debug > 0 ) {
    cout << "McMatrix::reduce(): "
	 << row_num() << " x " << col_num() << endl;
  }

  int no_change = 0;
  for ( ; ; ) {
    // 列支配を探し，列の削除を行う．
    if ( col_dominance() ) {
      no_change = 0;
      if ( mcmatrix_debug > 0 ) {
	cout << " after col_dominance: "
	     << row_num() << " x " << col_num() << endl;
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
	     << row_num() << " x " << col_num() << endl;
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
	     << row_num() << " x " << col_num() << endl;
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
  for ( auto row1: row_list() ) {
    row1->mWork = 0;
  }

  for ( auto row1: row_list() ) {
    if ( row1->mWork ) continue;

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    int min_num = row_size() + 1;
    const McHead* min_col = nullptr;
    for ( auto cell: row1->row_list() ) {
      int col_pos = cell->col_pos();
      auto col1 = col(col_pos);
      int col_num = col1->num();
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col1;
      }
    }
    // min_col に要素を持つ行のうち row1 に支配されている行を求める．
    for ( auto cell: min_col->col_list() ) {
      auto row2 = row(cell->row_pos());
      if ( row2 == row1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( row2->num() < row1->num() ) {
	// 要素数が少ない行も比較しない．
	continue;
      }
      if ( row2->mWork ) {
	// 削除された行も比較しない.
	continue;
      }

      // row1 に含まれる要素をすべて row2 が含んでいる場合
      // row1 が row2 を支配している．
      if ( check_containment(row2->row_list(), row1->row_list()) ) {
	int row_pos = row2->pos();
	delete_row(row_pos);
	row2->mWork = 1;
	change = true;
	if ( mcmatrix_debug > 1 ) {
	  cout << "Row#" << row_pos << " is dominated by Row#" << row1->pos() << endl;
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

  for ( auto col1: col_list() ) {
    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    int min_num = col_size() + 1;
    const McHead* min_row = nullptr;
    for ( auto cell: col1->col_list() ) {
      int row_pos = cell->row_pos();
      auto row1 = row(row_pos);
      int row_num = row1->num();
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row1;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for ( auto cell: min_row->row_list() ) {
      auto col2 = col(cell->col_pos());
      if ( col2 == col1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( col2->num() < col1->num() ) {
	// ただし col1 よりも要素数の少ない列は調べる必要はない．
	continue;
      }
      if ( col_cost(col2->pos()) > col_cost(col1->pos()) ) {
	// col2 のコストが col のコストより高ければ調べる必要はない．
	continue;
      }

      // col1 に含まれる要素を col2 がすべて含んでいる場合
      // col2 は col1 を支配している．
      if ( check_containment(col2->col_list(), col1->col_list() ) ) {
	delete_col(col1->pos());
	if ( mcmatrix_debug > 1 ) {
	  cout << "Col#" << col1->pos() << " is dominated by Col#"
	       << col2->pos() << endl;
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
  for ( auto col1: col_list() ) {
    col1->mWork = 0;
  }

  int old_size = selected_cols.size();
  for ( auto row1: row_list() ) {
    if ( row1->num() == 1 ) {
      auto cell = row1->row_front();
      int col_pos = cell->col_pos();
      ASSERT_COND( !col(col_pos)->mDeleted );

      auto col1 = col(col_pos);
      if ( col1->mWork == 0 ) {
	col1->mWork = 1;
	selected_cols.push_back(col_pos);
	if ( mcmatrix_debug > 1 ) {
	  cout << "Col#" << col_pos << " is essential" << endl;
	}
      }
    }
  }
  int size = selected_cols.size();
  for ( int i = old_size; i < size; ++ i) {
    int col_pos = selected_cols[i];
    select_col(col_pos);
  }

  return size > old_size;
}

END_NAMESPACE_YM_MINCOV
