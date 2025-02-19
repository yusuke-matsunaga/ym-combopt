
/// @file Isx.cc
/// @brief Isx の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Isx.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_COLORING

//////////////////////////////////////////////////////////////////////
// クラス Isx
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Isx::Isx(
  const UdGraph& graph
) : ColGraph{graph},
    mCandMark(node_num(), false),
    mAdjCount(node_num(), 0)
{
  mCandList.reserve(node_num());
  mIndepSet.reserve(node_num());
}

// @brief デストラクタ
Isx::~Isx()
{
}

// @brief independent set extraction を用いた coloring を行う．
SizeType
Isx::coloring(
  SizeType limit,
  vector<SizeType>& color_map
)
{
  SizeType remain_num = node_num();
  while ( remain_num > limit ) {
    get_indep_set();

    // mIndepSet の各ノードに新しい色を割り当てる．
    set_color(mIndepSet, new_color());

    remain_num -= mIndepSet.size();
  }

  return get_color_map(color_map);
}

// @brief maximal independent set を選ぶ．
void
Isx::get_indep_set()
{
  // 未彩色のノードを cand_list に入れる．
  init_cand_list();

  // ノードを一つづつ選択し mIndepSet に入れる．
  mIndepSet.clear();
  SizeType node_id = random_select(mCandList);
  while ( node_id != -1 ) {
    mIndepSet.push_back(node_id);

    // cand_list を更新する．
    update_cand_list(node_id);

    if ( mCandList.empty() ) {
      break;
    }

    // mCandList 中で隣接数が最小のものを選ぶ．
    vector<SizeType> tmp_list;
    SizeType min_num = node_num();
    for ( auto node_id: mCandList ) {
      SizeType c = mAdjCount[node_id];
      if ( min_num >= c ) {
	if ( min_num > c ) {
	  min_num = c;
	  tmp_list.clear();
	}
	tmp_list.push_back(node_id);
      }
    }
    node_id = random_select(tmp_list);
  }
  //sort(mIndepSet.begin(), mIndepSet.end());
}

// @brief mCandList, mCandMark を初期化する．
void
Isx::init_cand_list()
{
  for ( auto node_id: Range(node_num()) ) {
    if ( color(node_id) == 0 ) {
      mCandList.push_back(node_id);
      mCandMark[node_id] = true;
      mAdjCount[node_id] = 0;
    }
  }

  for ( auto node_id: mCandList ) {
    for ( auto node1_id: adj_list(node_id) ) {
      ++ mAdjCount[node1_id];
    }
  }
}

// @brief 候補リストを更新する．
void
Isx::update_cand_list(
  SizeType node_id
)
{
  // node_id と隣接するノードの cand_mark をはずす．
  mCandMark[node_id] = false;
  for ( auto node1_id: adj_list(node_id) ) {
    if ( mCandMark[node1_id] ) {
      mCandMark[node1_id] = false;
      for ( auto node2_id: adj_list(node1_id) ) {
	-- mAdjCount[node2_id];
      }
    }
  }

  SizeType rpos = 0;
  SizeType wpos = 0;
  SizeType n = mCandList.size();
  for ( ; rpos < n; ++ rpos ) {
    auto node1_id = mCandList[rpos];
    if ( mCandMark[node1_id] ) {
      if ( mAdjCount[node1_id] == 0 ) {
	mCandMark[node1_id] = false;
	mIndepSet.push_back(node1_id);
      }
      else {
	mCandList[wpos] = node1_id;
	++ wpos;
      }
    }
  }
  if ( wpos < n ) {
    mCandList.erase(mCandList.begin() + wpos, mCandList.end());
  }
}

// @brief ランダムに選ぶ．
SizeType
Isx::random_select(
  const vector<SizeType>& cand_list
)
{
  SizeType n = cand_list.size();
  ASSERT_COND( n > 0 );

  std::uniform_int_distribution<SizeType> rd(0, n - 1);
  SizeType r = rd(mRandGen);
  return cand_list[r];
}

END_NAMESPACE_YM_COLORING
