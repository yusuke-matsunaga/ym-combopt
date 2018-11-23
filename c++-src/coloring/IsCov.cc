
/// @file IsCov.cc
/// @brief IsCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "IsCov.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス IsCov
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
IsCov::IsCov(const UdGraph& graph) :
  mGraph(graph)
{
}

// @brief デストラクタ
IsCov::~IsCov()
{
}

// @brief independent set cover を行う．
int
IsCov::covering(int limit,
		vector<int>& color_map)
{
  int remain_num = mGraph.node_num();
  while ( remain_num > limit ) {
    vector<int> cand_list;
    cand_list.reserve(remain_num);
    for ( auto node_id: Range(mGraph.node_num()) ) {
      if ( mGraph.color(node_id) == 0 ) {
	cand_list.push_back(node_id);
      }
    }
    vector<bool> cur_mark(mGraph.node_num(), false);
    vector<int> iset;
    for ( ; ; ) {
      int node_id = select_node(cand_list, cur_mark);
      if ( node_id == -1 ) {
	break;
      }
      cur_mark[node_id] = true;
      iset.push_back(node_id);
      update_cand_list(cand_list, node_id);
    }
    int num = iset.size();
    ASSERT_COND( num > 0 );
    int cur_col = 0;
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
// @param[in] cur_mark 現在の候補集合を表す配列
//
// - cur_mark[node_id] = true のノードが候補集合の要素
// - 現在の候補集合に隣接していないノードの内，隣接ノード数の少ないものを選ぶ．
// - 追加できるノードがない場合は -1 を返す．
int
IsCov::select_node(const vector<int>& cand_list,
		   const vector<bool>& cur_mark)
{
  vector<int> min_list;
  int min_num = mGraph.node_num();
  for ( auto node_id: cand_list ) {
    int c = mGraph.adj_list(node_id).num();
    if ( min_num >= c ) {
      if ( min_num > c ) {
	min_num = c;
	min_list.clear();
      }
      min_list.push_back(node_id);
    }
  }
  int n = min_list.size();
  if ( n == 0 ) {
    // 追加できるノードがない．
    return -1;
  }
  else if ( n == 1 ) {
    return min_list[0];
  }
  else {
    std::uniform_int_distribution<int> rd(0, n - 1);
    int r = rd(mRandGen);
    return min_list[r];
  }
}

// @brief 候補リストを更新する．
// @param[inout] cand_list 候補リスト
// @param[in] node_id 新たに加わったノード
void
IsCov::update_cand_list(vector<int>& cand_list,
			int node_id)
{
  // node_id に隣接するノードに印を付ける．
  vector<bool> mark(mGraph.node_num(), false);
  mark[node_id] = true;
  for ( auto node1_id: mGraph.adj_list(node_id) ) {
    mark[node1_id] = true;
  }

  int n = cand_list.size();
  int rpos = 0;
  int wpos = 0;
  for ( rpos = 0; rpos < n; ++ rpos ) {
    int node1_id = cand_list[rpos];
    if ( !mark[node1_id] ) {
      cand_list[wpos] = node1_id;
      ++ wpos;
    }
  }
  if ( wpos < n ) {
    cand_list.erase(cand_list.begin() + wpos, cand_list.end());
  }
}

END_NAMESPACE_YM_UDGRAPH
