
/// @file McRowList.cc
/// @brief McRowList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/McRowList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McRowList
//////////////////////////////////////////////////////////////////////

// @brief 等価比較演算子
bool
operator==(
  const McRowList& list1,
  const McRowList& list2
)
{
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  auto end1 = list1.end();
  auto end2 = list2.end();

  while ( it1 != end1 && it2 != end2 ) {
    if ( *it1 != *it2 ) {
      return false;
    }
    ++ it1;
    ++ it2;
  }
  if ( it1 == end1 && it2 == end2 ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 包含関係を調べる．
bool
check_containment(
  const McRowList& list1,
  const McRowList& list2
)
{
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  auto end1 = list1.end();
  auto end2 = list2.end();

  while ( it1 != end1 && it2 != end2 ) {
    auto cpos1 = *it1;
    auto cpos2 = *it2;
    if ( cpos1 > cpos2 ) {
      // list1 になくて list2 にある要素がある．
      return false;
    }
    if ( cpos1 == cpos2 ) {
      ++ it2;
    }
    ++ it1;
  }
  if ( it1 == end1 ) {
    if ( it2 == end2 ) {
      return true;
    }
    else {
      // list1 には要素がなく list2 に要素が残っている．
      return false;
    }
  }
  else {
    // it2 == end2 のはずなので OK
    return true;
  }
}

END_NAMESPACE_YM_MINCOV
