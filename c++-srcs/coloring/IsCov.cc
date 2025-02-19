
/// @file IsCov.cc
/// @brief IsCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "IsCov.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_COLORING

//////////////////////////////////////////////////////////////////////
// クラス IsCov
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IsCov::IsCov(
  const UdGraph& graph
) : mGraph{graph}
{
}

// @brief デストラクタ
IsCov::~IsCov()
{
}

// @brief independent set cover を行う．
SizeType
IsCov::covering(
  SizeType limit,
  vector<SizeType>& color_map
)
{
  SizeType remain_num = mGraph.node_num();
  while ( remain_num > limit ) {
    vector<SizeType> cand_list;
    cand_list.reserve(remain_num);
    for ( auto node_id: Range(mGraph.node_num()) ) {
      if ( mGraph.color(node_id) == 0 ) {
	cand_list.push_back(node_id);
      }
    }
    vector<bool> cur_mark(mGraph.node_num(), false);
    vector<SizeType> iset;
    while ( !cand_list.empty() ) {
      SizeType node_id = select_node(cand_list, cur_mark);
      cur_mark[node_id] = true;
      iset.push_back(node_id);
      update_cand_list(cand_list, node_id);
    }
    SizeType num = iset.size();
    ASSERT_COND( num > 0 );
    SizeType cur_col = 0;
    for ( auto node_id: iset ) {
      if ( cur_col == 0 ) {
	cur_col = mGraph.new_color();
      }
      mGraph.set_color(node_id, cur_col);
    }
    remain_num -= num;
  }

  return mGraph.get_color_map(color_map);
}

// @brief 候補集合に加えるノードを選ぶ．
SizeType
IsCov::select_node(
  const vector<SizeType>& cand_list,
  const vector<bool>& cur_mark
)
{
  vector<SizeType> min_list;
  SizeType min_num = mGraph.node_num();
  for ( auto node_id: cand_list ) {
    int c = mGraph.adj_list(node_id).size();
    if ( min_num >= c ) {
      if ( min_num > c ) {
	min_num = c;
	min_list.clear();
      }
      min_list.push_back(node_id);
    }
  }
  SizeType n = min_list.size();
  ASSERT_COND( n > 0 );
  if ( n == 1 ) {
    return min_list[0];
  }
  else {
    std::uniform_int_distribution<SizeType> rd(0, n - 1);
    SizeType r = rd(mRandGen);
    return min_list[r];
  }
}

// @brief 候補リストを更新する．
void
IsCov::update_cand_list(
  vector<SizeType>& cand_list,
  SizeType node_id
)
{
  // node_id に隣接するノードに印を付ける．
  vector<bool> mark(mGraph.node_num(), false);
  mark[node_id] = true;
  for ( auto node1_id: mGraph.adj_list(node_id) ) {
    mark[node1_id] = true;
  }

  SizeType n = cand_list.size();
  SizeType rpos = 0;
  SizeType wpos = 0;
  for ( rpos = 0; rpos < n; ++ rpos ) {
    SizeType node1_id = cand_list[rpos];
    if ( !mark[node1_id] ) {
      cand_list[wpos] = node1_id;
      ++ wpos;
    }
  }
  if ( wpos < n ) {
    cand_list.erase(cand_list.begin() + wpos, cand_list.end());
  }
}

END_NAMESPACE_YM_COLORING
