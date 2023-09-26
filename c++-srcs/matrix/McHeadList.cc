
/// @file McHeadList.cc
/// @brief McHeadList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/McHeadList.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McHeadList
//////////////////////////////////////////////////////////////////////

// @brief 内容をセットする．
void
McHeadList::set(const vector<McHead*>& head_list)
{
  mNum = 0;
  McHead* prev_head = &mDummy;
  for ( auto head: head_list ) {
    prev_head->mNext = head;
    head->mPrev = prev_head;
    prev_head = head;
    ++ mNum;
  }
  prev_head->mNext = &mDummy;
  mDummy.mPrev = prev_head;
}

// @brief 要素を追加する．
void
McHeadList::insert(McHead* head)
{
  McHead* prev = mDummy.mPrev; // 末尾
  McHead* next = &mDummy;
  int pos = head->pos();
  if ( prev == next || prev->pos() < pos ) {
    // 末尾に追加
    // よくあるパタンなので最初に調べる．
    ;
  }
  else {
    for ( prev = &mDummy; ; prev = next ) {
      next = prev->mNext;
      ASSERT_COND( next->pos() != pos );
      if ( next->pos() > pos ) {
	// prev と next の間に挿入する．
	break;
      }
      ASSERT_COND( next != &mDummy );
    }
  }
  prev->mNext = head;
  head->mPrev = prev;
  head->mNext = next;
  next->mPrev = head;
  ++ mNum;
}

// @brief 要素を削除する．
// @param[in] head 削除する要素
//
// row がこのリストに含まれていると仮定する．
void
McHeadList::exclude(McHead* head)
{
  ASSERT_COND( !head->is_deleted() );
  head->set_deleted(true);
  -- mNum;

  McHead* prev = head->mPrev;
  McHead* next = head->mNext;
  prev->mNext = next;
  next->mPrev = prev;
}

// @brief exclude() で削除した行を復元する．
void
McHeadList::restore(McHead* head)
{
  ASSERT_COND( head->is_deleted() );
  head->set_deleted(false);
  ++ mNum;

  McHead* prev = head->mPrev;
  McHead* next = head->mNext;
  prev->mNext = head;
  next->mPrev = head;
}

// @brief 分割したリストをマージして元にもどす．
// @param[in] src1, src2 分割したリスト
//
// src1, src2 の内容は破棄される．
void
McHeadList::merge(McHeadList& src1,
		  McHeadList& src2)
{
  vector<McHead*> row_list;
  row_list.reserve(src1.num() + src2.num());
  McHead* head1 = src1.mDummy.mNext;
  McHead* head2 = src2.mDummy.mNext;
  McHead* end1 = &src1.mDummy;
  McHead* end2 = &src2.mDummy;
  while ( head1 != end1 && head2 != end2 ) {
    if ( head1->pos() < head2->pos() ) {
      row_list.push_back(head1);
      head1 = head1->mNext;
    }
    else if ( head1->pos() > head2->pos() ) {
      row_list.push_back(head2);
      head2 = head2->mNext;
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
  for ( ; head1 != end1; head1 = head1->mNext ) {
    row_list.push_back(head1);
  }
  for ( ; head2 != end2; head2 = head2->mNext ) {
    row_list.push_back(head2);
  }
  set(row_list);
}

// @brief 等価比較演算子
// @param[in] list1, list2 オペランド
bool
operator==(const McHeadList& list1,
	   const McHeadList& list2)
{
  if ( list1.num() != list2.num() ) {
    return false;
  }

  McHeadIterator it1 = list1.begin();
  McHeadIterator end1 = list1.end();
  McHeadIterator it2 = list2.begin();
  McHeadIterator end2 = list2.end();
  for ( ; ; ) {
    if ( *it1 != *it2 ) {
      return false;
    }

    ++ it1;
    ++ it2;
    if ( it1 == end1 ) {
      if ( it2 != end2 ) {
	return false;
      }
      break;
    }
  }

  return true;
}

END_NAMESPACE_YM_MINCOV
