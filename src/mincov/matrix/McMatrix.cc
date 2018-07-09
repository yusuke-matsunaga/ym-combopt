
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
		   const vector<int>& cost_array,
		   const vector<pair<int, int>>& elem_list) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(nullptr),
  mColArray(nullptr),
  mCostArray(nullptr),
  mDelStack(nullptr),
  mRowMark(nullptr),
  mColMark(nullptr)
{
  // サイズを設定する．
  resize(row_size, col_size);

  // コストを設定する．
  for ( auto col: Range(col_size) ) {
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
  mColSize(0),
  mRowArray(nullptr),
  mColArray(nullptr),
  mCostArray(nullptr),
  mDelStack(nullptr),
  mRowMark(nullptr),
  mColMark(nullptr)
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

  delete [] mRowArray;
  delete [] mColArray;
  delete [] mCostArray;
  delete [] mDelStack;
  delete [] mRowMark;
  delete [] mColMark;

  mRowArray = nullptr;
  mColArray = nullptr;
  mCostArray = nullptr;
  mDelStack = nullptr;
  mRowMark = nullptr;
  mColMark = nullptr;
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
McMatrix::insert_elem(int row_pos,
		      int col_pos)
{
  auto cell = alloc_cell(row_pos, col_pos);

  auto row_head1 = _row_head(row_pos);
  bool stat1 = row_head1->row_insert(cell);
  if ( !stat1 ) {
    // 列番号が重複しているので無視する．
    free_cell(cell);
    return;
  }

  auto col_head1 = _col_head(col_pos);
  bool stat2 = col_head1->col_insert(cell);
  ASSERT_COND( stat2 );
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
    mRowMark = new int[mRowSize];
    for ( auto row_pos: Range(mRowSize) ) {
      mRowArray[row_pos].init(row_pos, false);
      mRowMark[row_pos] = 0;
    }

    mColArray = new McHead[mColSize];
    mCostArray = new int[mColSize];
    mColMark = new int[mColSize];
    for ( auto col_pos: Range(mColSize) ) {
      mColArray[col_pos].init(col_pos, true);
      mCostArray[col_pos] = 1;
      mColMark[col_pos] = 0;
    }

    mDelStack = new McHead*[row_size + col_size];
    mStackTop = 0;

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
    for ( auto src_cell: _row_head(row_pos)->row_list() ) {
      auto col_pos = src_cell->col_pos();
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
    for ( auto cell: _col_head(col_pos)->col_list() ) {
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
    for ( auto cell: _row_head(row_pos)->row_list() ) {
      s << " " << cell->col_pos();
    }
    s << endl;
  }
}

END_NAMESPACE_YM_MINCOV
