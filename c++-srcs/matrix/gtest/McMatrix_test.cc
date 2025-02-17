
/// @file McMatrix_test.cc
/// @brief McMatrix_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "mincov/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

TEST(McMatrixTest, constr1)
{
  SizeType nr = 3;
  SizeType nc = 4;
  auto mtx = McMatrix(nr, nc);

  EXPECT_EQ( nr, mtx.row_size() );
  EXPECT_EQ( 0, mtx.active_row_num() );
  for ( SizeType row_pos = 0; row_pos < nr; ++ row_pos ) {
    auto row_list = mtx.row_list(row_pos);
    EXPECT_EQ( row_list.begin(), row_list.end() );
    EXPECT_EQ( 0, mtx.row_elem_num(row_pos) );
    EXPECT_FALSE( mtx.row_deleted(row_pos) );
  }

  EXPECT_EQ( nc, mtx.col_size() );
  EXPECT_EQ( 0, mtx.active_col_num() );
  for ( SizeType col_pos = 0; col_pos < nc; ++ col_pos ) {
    auto col_list = mtx.col_list(col_pos);
    EXPECT_EQ( col_list.begin(), col_list.end() );
    EXPECT_EQ( 0, mtx.col_elem_num(col_pos) );
    EXPECT_FALSE( mtx.col_deleted(col_pos) );
    EXPECT_EQ( 1, mtx.col_cost(col_pos) );
  }
  auto& cc_array = mtx.col_cost_array();
  EXPECT_EQ( nc, cc_array.size() );
  for ( SizeType col_pos; col_pos < nc; ++ col_pos ) {
    EXPECT_EQ( 1, mtx.col_cost(col_pos) );
  }
}

TEST(McMatrixTest, constr2)
{
  SizeType nr = 3;
  auto cost_array = vector<SizeType>{1, 2, 3, 4};
  auto mtx = McMatrix(nr, cost_array);
  auto nc = cost_array.size();

  EXPECT_EQ( nr, mtx.row_size() );
  EXPECT_EQ( nc, mtx.col_size() );
  for ( SizeType i = 0; i < nc; ++ i ) {
    EXPECT_EQ( cost_array[i], mtx.col_cost(i) );
  }

  SizeType nc_exp = 1 << nc;
  for ( SizeType b = 0; b < nc_exp; ++ b ) {
    vector<SizeType> col_list;
    col_list.reserve(nc);
    SizeType exp_cost = 0;
    for ( SizeType i = 0; i < nc; ++ i ) {
      if ( b & (1 << i) ) {
	col_list.push_back(i);
	exp_cost += cost_array[i];
      }
    }
    auto cost = mtx.cost(col_list);
    EXPECT_EQ( exp_cost, cost );
  }
}

TEST(McMatrixTest, constr3)
{
  SizeType nr = 3;
  SizeType nc = 4;
  auto elem_list = vector<MinCov::ElemType>{
    {0, 0}, {0, 2},
    {1, 1}, {1, 2},
    {2, 0}, {2, 3}
  };
  auto mtx = McMatrix(nr, nc, elem_list);

  EXPECT_EQ( nr, mtx.row_size() );
  EXPECT_EQ( nc, mtx.col_size() );

  EXPECT_EQ( nr, mtx.active_row_num() );
  EXPECT_EQ( nc, mtx.active_col_num() );

  auto row0_list = mtx.row_list(0);
  {
    auto it = row0_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 2, *it );
  }
  auto row1_list = mtx.row_list(1);
  {
    auto it = row1_list.begin();
    EXPECT_EQ( 1, *it );
    ++ it;
    EXPECT_EQ( 2, *it );
  }
  auto row2_list = mtx.row_list(2);
  {
    auto it = row2_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 3, *it );
  }

  auto col0_list = mtx.col_list(0);
  {
    auto it = col0_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 2, *it );
  }
  auto col1_list = mtx.col_list(1);
  {
    auto it = col1_list.begin();
    EXPECT_EQ( 1, *it );
  }
  auto col2_list = mtx.col_list(2);
  {
    auto it = col2_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 1, *it );
  }
  auto col3_list = mtx.col_list(3);
  {
    auto it = col3_list.begin();
    EXPECT_EQ( 2, *it );
  }
}

END_NAMESPACE_YM_MINCOV
