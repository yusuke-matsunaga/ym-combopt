
/// @file McHead.cc
/// @brief McHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/McHead.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McHead
//////////////////////////////////////////////////////////////////////

// @brief 行方向のリストに挿入する．
// @param[in] cell 挿入する要素
// @retval true 追加が成功した．
// @retval false 同じ要素がすでに存在した．
bool
McHead::row_insert(McCell* cell)
{
  ASSERT_COND( is_row() );

  int col_pos = cell->col_pos();
  McCell* pcell;
  McCell* ncell;
  if ( num() == 0 || row_end()->row_prev()->col_pos() < col_pos ) {
    // 末尾への追加
    ncell = &mDummy;
    pcell = ncell->mLeftLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->col_pos() >= col_pos が成り立っている．
    for ( pcell = &mDummy; ; pcell = ncell ) {
      ncell = pcell->mRightLink;
      if ( ncell->col_pos() == col_pos ) {
	// 列番号が重複しているので無視する．
	return false;
      }
      if ( ncell->col_pos() > col_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      ASSERT_COND( ncell != &mDummy );
    }
  }
  cell->mLeftLink = pcell;
  cell->mRightLink = ncell;

  row_restore(cell);

  return true;
}

// @brief 列方向のリストに挿入する．
// @param[in] cell 挿入する要素
// @retval true 追加が成功した．
// @retval false 同じ要素がすでに存在した．
bool
McHead::col_insert(McCell* cell)
{
  ASSERT_COND( is_col() );

  int row_pos = cell->row_pos();
  McCell* pcell;
  McCell* ncell;
  if ( num() == 0 || col_end()->col_prev()->row_pos() < row_pos ) {
    // 末尾への追加
    ncell = &mDummy;
    pcell = ncell->mUpLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->row_pos() >= row_pos が成り立っている．
    for ( pcell = &mDummy; ; pcell = ncell ) {
      ncell = pcell->mDownLink;
      if ( ncell->row_pos() == row_pos ) {
	// 列番号が重複しているので無視する．
	return false;
      }
      if ( ncell->row_pos() > row_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      ASSERT_COND( ncell != &mDummy );
    }
  }
  cell->mUpLink = pcell;
  cell->mDownLink = ncell;

  col_restore(cell);

  return true;
}

END_NAMESPACE_YM_MINCOV
