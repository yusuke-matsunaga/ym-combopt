
/// @file McColList.cc
/// @brief McColList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/McColList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McColList
//////////////////////////////////////////////////////////////////////

// @brief 等価比較演算子
// @param[in] list1, list2 オペランド
bool
operator==(
  const McColList& list1,
  const McColList& list2
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
  const McColList& list1,
  const McColList& list2
)
{
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  auto end1 = list1.end();
  auto end2 = list2.end();

  while ( it1 != end1 && it2 != end2 ) {
    auto rpos1 = *it1;
    auto rpos2 = *it2;
    if ( rpos1 > rpos2 ) {
      // list1 になくて list2 にある要素がある．
      return false;
    }
    if ( rpos1 == rpos2 ) {
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
