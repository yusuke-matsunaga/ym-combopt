
/// @file MaxClique.cc
/// @brief MaxClique の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "MaxClique.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MaxClique
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

bool debug = false;

// left の要素のうち right にも含まれるもののみを残す．
void
merge_nlist(
  vector<SizeType>& left,
  const vector<SizeType>& right
)
{
  if ( debug ) {
    cout << "mrege_nlist" << endl;
    cout << " left = ";
    for ( auto p = left.begin(); p != left.end(); ++ p) {
      cout << " " << *p;
    }
    cout << endl;
    cout << " right = ";
    for ( auto p = right.begin(); p != right.end(); ++ p) {
      cout << " " << *p;
    }
    cout << endl;
  }

  auto l_rpos = left.begin();
  auto l_wpos = left.begin();
  auto l_end = left.end();
  auto r_rpos = right.begin();
  auto r_end = right.end();
  for ( ; ; ) {
    if ( l_rpos == l_end ) {
      break;
    }
    SizeType l_id = *l_rpos;
    if ( r_rpos == r_end ) {
      left.erase(l_wpos, l_end);
      break;
    }
    SizeType r_id = *r_rpos;
    if ( l_id < r_id ) {
      ++ l_rpos;
    }
    else if ( l_id == r_id ) {
      if ( l_rpos != l_wpos ) {
	*l_wpos = l_id;
      }
      ++ l_rpos;
      ++ l_wpos;
      ++ r_rpos;
    }
    else { // l_id > r_id
      ++ r_rpos;
    }
  }

  if ( debug ) {
    cout << " result = ";
    for ( auto p = left.begin(); p != left.end(); ++ p) {
      cout << " " << *p;
    }
    cout << endl;
  }
}

END_NONAMESPACE


// @brief 最大クリークを求める．
double
MaxClique::solve(
  vector<SizeType>& ans
)
{
  ans.clear();

  double cost = 0.0;
  SizeType n = mCostArray.size();
  vector<bool> mark(n, false);
  for ( ; ; ) {
    SizeType min_num = UINT_MAX;
    SizeType min_row = 0;
    bool found = false;
    for ( SizeType i = 0; i < n; ++ i ) {
      if ( mark[i] ) {
	continue;
      }
      SizeType num = mNlistArray[i].size();
      if ( min_num > num ) {
	min_num = num;
	min_row = i;
	found = true;
      }
    }
    if ( !found ) {
      break;
    }

    ans.push_back(min_row);
    mark[min_row] = true;

    for ( auto row: mNlistArray[min_row] ) {
      mark[row] = true;
    }
    cost += mCostArray[min_row];
  }
  return cost;
}

END_NAMESPACE_YM_MINCOV
