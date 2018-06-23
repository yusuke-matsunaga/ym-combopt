
/// @file McRowList.cc
/// @brief McRowList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "McRowList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McRowList
//////////////////////////////////////////////////////////////////////

// @brief 等価比較演算子
// @param[in] list1, list2 オペランド
bool
operator==(const McRowList& list1,
	   const McRowList& list2)
{
  McRowIterator it1 = list1.begin();
  McRowIterator it2 = list2.begin();
  McRowIterator end1 = list1.end();
  McRowIterator end2 = list2.end();

  while ( it1 != end1 && it2 != end2 ) {
    auto cell1 = *it1;
    auto cell2 = *it2;
    if ( cell1->col_pos() != cell2->col_pos() ) {
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
// @param[in] list1, list2 オペランド
//
// list1 が list2 の要素をすべて含んでいたら true を返す．
bool
check_containment(const McRowList& list1,
		  const McRowList& list2)
{
  McRowIterator it1 = list1.begin();
  McRowIterator it2 = list2.begin();
  McRowIterator end1 = list1.end();
  McRowIterator end2 = list2.end();

  while ( it1 != end1 && it2 != end2 ) {
    auto cell1 = *it1;
    auto cell2 = *it2;
    if ( cell1->col_pos() > cell2->col_pos() ) {
      // list1 になくて list2 にある要素がある．
      return false;
    }
    if ( cell1->col_pos() < cell2->col_pos() ) {
      ++ it1;
    }
    ++ it2;
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
