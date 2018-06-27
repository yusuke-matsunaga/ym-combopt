
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "mincov/McMatrix.h"
#include "ym/Range.h"


//#define VERIFY_MCMATRIX 1


BEGIN_NAMESPACE_YM_MINCOV

int mcmatrix_debug = 0;

//////////////////////////////////////////////////////////////////////
// クラス McMatrix
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] cost_array コストの配列
// @param[in] elem_list 要素のリスト
McMatrix::McMatrix(int row_size,
		   int col_size,
		   const int* cost_array,
		   const vector<pair<int, int>>& elem_list) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(nullptr),
  mColArray(nullptr),
  mCostArray(cost_array),
  mDelQueue(nullptr),
  mDelStack(nullptr)
{
  resize(row_size, col_size);

  for ( auto row_col: elem_list ) {
    insert_elem(row_col.first, row_col.second);
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
McMatrix::McMatrix(const McMatrix& src) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(nullptr),
  mColArray(nullptr),
  mDelQueue(nullptr),
  mDelStack(nullptr)
{
  resize(src.row_size(), src.col_size());

  copy(src);
}

#if 0
// @brief 部分的なコピーコンストラクタ
// @param[in] src コピー元のオブジェクト
// @param[in] rowid_list コピーする行番号のリスト
// @param[in] colid_list コピーする列番号のリスト
McMatrix::McMatrix(McMatrix& src,
		   const vector<int>& row_pos_list,
		   const vector<int>& col_pos_list) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(nullptr),
  mColArray(nullptr)
{
  resize(src.row_size(), src.col_size());

  vector<McHead*> row_list;
  row_list.reserve(row_pos_list.size());
  for ( auto row_pos: row_pos_list ) {
    auto row_head = src.row_head(row_pos);
    mRowArray[row_pos] = row_head;
    row_list.push_back(row_head);
  }
  //mRowList.set(row_list);

  vector<McHead*> col_list;
  col_list.reserve(col_pos_list.size());
  for ( auto col_pos: col_pos_list ) {
    auto col_head = src.col_head(col_pos);
    mColArray[col_pos] = col_head;
    col_list.push_back(col_head);
  }
  //mColList.set(col_list);

  mCostArray = src.mCostArray;
}
#endif

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
  delete [] mRowArray;
  delete [] mColArray;
  delete [] mDelQueue;
  delete [] mDelStack;
}

// @brief 内容をクリアする．
void
McMatrix::clear()
{
  mCellAlloc.destroy();

  delete [] mRowArray;
  delete [] mColArray;
  delete [] mDelQueue;
  delete [] mDelStack;

  mRowArray = nullptr;
  mColArray = nullptr;
  mDelQueue = nullptr;
  mDelStack = nullptr;
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

    mRowArray = new McHead[mRowSize];
    for ( auto row_pos: Range(mRowSize) ) {
      mRowArray[row_pos].init(row_pos);
    }

    mColArray = new McHead[mColSize];
    for ( auto col_pos: Range(mColSize) ) {
      mColArray[col_pos].init(col_pos);
    }

    delete [] mDelQueue;
    mDelQueue = new int[row_size + col_size];
    mQueueTop = 0;
    mQueueEnd = 0;

    delete [] mDelStack;
    mDelStack = new int[row_size + col_size];
    mStackTop = 0;
  }
}

// @brief 内容をコピーする．
void
McMatrix::copy(const McMatrix& src)
{
  ASSERT_COND(row_size() == src.row_size() );
  ASSERT_COND(col_size() == src.col_size() );

  for ( auto row_pos: Range(row_size()) ) {
    for ( auto src_cell: row_head(row_pos)->row_list() ) {
      auto col_pos = src_cell->col_pos();
      insert_elem(row_pos, col_pos);
    }
  }

  mCostArray = src.mCostArray;
}

#if 0
// @brief 分割した行列をもとに戻す．
void
McMatrix::merge(McMatrix& matrix1,
		McMatrix& matrix2)
{
#if 0
  mRowList.merge(matrix1.row_list(), matrix2.row_list());
  mColList.merge(matrix1.col_list(), matrix2.col_list());
#endif
}
#endif

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

#if 0
// @brief ブロック分割を行う．
// @param[in] row_list1 1つめのブロックの行番号のリスト
// @param[in] row_list2 2つめのブロックの行番号のリスト
// @param[in] col_list1 1つめのブロックの列番号のリスト
// @param[in] col_list2 2つめのブロックの列番号のリスト
// @retval true ブロック分割が行われた．
// @retval false ブロック分割が行えなかった．
bool
McMatrix::block_partition(vector<int>& row_list1,
			  vector<int>& row_list2,
			  vector<int>& col_list1,
			  vector<int>& col_list2) const
{
#if 0
  // マークを消す．
  for ( auto row1: row_list() ) {
    row1->mWork = 0U;
  }
  for ( auto col1: col_list() ) {
    col1->mWork = 0U;
  }

  // 最初の行から到達可能な行と列にマークをつける．
  auto row0 = row_list().front();
  int nc1 = mark_cols(row0);

  int nr = row_num();
  int nc = col_num();

  if ( nc == nc1 ) {
    return false;
  }

  ymuint bitmask = 0U;
  if ( nc1 > nc - nc1 ) {
    bitmask = 1U;
  }

  row_list1.clear();
  row_list1.reserve(nr);
  row_list2.clear();
  row_list2.reserve(nr);
  for ( auto row1: row_list() ) {
    if ( row1->mWork ^ bitmask ) {
      row_list1.push_back(row1->pos());
    }
    else {
      row_list2.push_back(row1->pos());
    }
  }

  col_list1.clear();
  col_list1.reserve(nc);
  col_list2.clear();
  col_list2.reserve(nc);
  for ( auto col1: col_list() ) {
    if ( col1->mWork ^ bitmask ) {
      col_list1.push_back(col1->pos());
    }
    else {
      col_list2.push_back(col1->pos());
    }
  }
#endif

  return true;
}
#endif

#if 0
// @brief col に接続している行をマークする．
// @param[in] col 対象の列
// @return マークされた列数を返す．
int
McMatrix::mark_rows(const McHead* col) const
{
  int nc = 0;
  for ( auto cell: col->col_list() ) {
    int row_pos = cell->row_pos();
    auto row1 = row(row_pos);
    if ( row1->mWork == 0U ) {
      row1->mWork = 1U;
      nc += mark_cols(row1);
    }
  }
  return nc;
}

// @brief row に接続している列をマークする．
// @param[in] row 対象の行
// @return マークされた列数を返す．
int
McMatrix::mark_cols(const McHead* row) const
{
  int nc = 0;
  for ( auto cell: row->row_list() ) {
    int col_pos = cell->col_pos();
    auto col1 = col(col_pos);
    if ( col1->mWork == 0U ) {
      col1->mWork = 1U;
      ++ nc;
      nc += mark_rows(col1);
    }
  }
  return nc;
}
#endif

// @brief 列集合がカバーになっているか検証する．
// @param[in] col_list 列のリスト
// @retval true col_list がカバーになっている．
// @retval false col_list でカバーされていない行がある．
bool
McMatrix::verify(const vector<int>& col_list) const
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
  for ( auto row_pos: Range(row_size()) ) {
    if ( !row_mark[row_pos] ) {
      return false;
    }
  }
  return true;
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
McMatrix::insert_elem(int row_pos,
		      int col_pos)
{
  auto cell = alloc_cell(row_pos, col_pos);

  auto row1 = row_head(row_pos);
  bool stat1 = row1->row_search(cell);
  if ( !stat1 ) {
    // 列番号が重複しているので無視する．
    free_cell(cell);
    return;
  }

  McCell::row_insert(cell);
  row1->inc_num();

  auto col1 = col_head(col_pos);
  bool stat2 = col1->col_search(cell);
  ASSERT_COND( stat2 );

  McCell::col_insert(cell);
  col1->inc_num();
}

#if 0
// @brief 列を選択し，被覆される行を削除する．
void
McMatrix::select_col(int col_pos)
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
#endif

#if 0
// @brief 削除スタックにマーカーを書き込む．
void
McMatrix::save()
{
  push_marker();
}

// @brief 直前のマーカーまで処理を戻す．
void
McMatrix::restore()
{
  while ( !stack_empty() ) {
    int tmp = pop();
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
#endif

#if 0
// @brief 行を削除する．
void
McMatrix::delete_row(int row_pos)
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
McMatrix::restore_row(int row_pos)
{
  auto row1 = row(row_pos);
  mRowList.restore(row1);

  for ( auto cell = row1->mDummy.mRightLink;
	cell != &row1->mDummy; cell = cell->mRightLink ) {
    McCell::col_insert(cell);
  }
}

// @brief 列を削除する．
void
McMatrix::delete_col(int col_pos)
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
McMatrix::restore_col(int col_pos)
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
// @param[out] selected_cols 簡単化中で選択された列の集合
void
McMatrix::reduce(vector<int>& selected_cols)
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


BEGIN_NONAMESPACE

struct RowLt
{
  bool
  operator()(const McHead* a,
	     const McHead* b)
  {
    return a->num() < b->num();
  }
};

END_NONAMESPACE


// @brief 行支配を探し，行を削除する．
// @return 削除された行があったら true を返す．
bool
McMatrix::row_dominance()
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


BEGIN_NONAMESPACE

struct ColLt
{
  bool
  operator()(const McHead* a,
	     const McHead* b)
  {
    return a->num() < b->num();
  }
};

END_NONAMESPACE


// @brief 列支配を探し，列を削除する．
// @return 削除された列があったら true を返す．
bool
McMatrix::col_dominance()
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
McMatrix::essential_col(vector<int>& selected_cols)
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
#endif

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
    for ( auto cell: row_head(row_pos)->row_list() ) {
      s << " " << cell->col_pos();
    }
    s << endl;
  }
}

END_NAMESPACE_YM_MINCOV
