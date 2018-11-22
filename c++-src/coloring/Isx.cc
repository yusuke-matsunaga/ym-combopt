
/// @file Isx.cc
/// @brief Isx の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "Isx.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス Isx
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
Isx::Isx(const UdGraph& graph) :
  ColGraph(graph),
  mCandMark(node_num(), false),
  mAdjCount(node_num(), 0)
{
  mCandList.reserve(node_num());
  mTmpList.reserve(node_num());
  mIndepSet.reserve(node_num());
}

// @brief デストラクタ
Isx::~Isx()
{
}

// @brief independent set extraction を用いた coloring を行う．
// @param[in] limit 残りのノード数がこの値を下回ったら処理をやめる．
// @param[out] color_map 彩色結果を収める配列
// @return 彩色数を返す．
//
// ここでは部分的な彩色を行う．
int
Isx::coloring(int limit,
	      vector<int>& color_map)
{
  int remain_num = node_num();
  while ( remain_num > limit ) {
    get_indep_set();

    // mIndepSet の各ノードに新しい色を割り当てる．
    set_color(mIndepSet, new_color());

    remain_num -= mIndepSet.size();
  }

  return get_color_map(color_map);
}

// @brief maximal independent set を選ぶ．
//
// - 結果は mIndepSet に格納される．
// - mRandGen を用いてランダムに選ぶ．
void
Isx::get_indep_set()
{
  // 未彩色のノードを cand_list に入れる．
  init_cand_list();

  // ノードを一つづつ選択し mIndepSet に入れる．
  mIndepSet.clear();
  int node_id = random_select(mCandList);
  while ( node_id != -1 ) {
    mIndepSet.push_back(node_id);

    // cand_list を更新する．
    update_cand_list(node_id);

    node_id = select_node();
  }
  //sort(mIndepSet.begin(), mIndepSet.end());
}

// @brief mCandList, mCandMark を初期化する．
void
Isx::init_cand_list()
{
  for ( auto node_id: Range<>(node_num()) ) {
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

// @brief 候補集合に加えるノードを選ぶ．
//
// - 現在の候補集合に隣接していないノードの内，隣接ノード数の少ないものを選ぶ．
// - 追加できるノードがない場合は -1 を返す．
int
Isx::select_node()
{
  ASSERT_COND( mCandList.size() > 0 );

  mTmpList.clear();
  int min_num = node_num();
  for ( auto node_id: mCandList ) {
    int c = mAdjCount[node_id];
    if ( min_num >= c ) {
      if ( min_num > c ) {
	min_num = c;
	mTmpList.clear();
      }
      mTmpList.push_back(node_id);
    }
  }

  int n = mTmpList.size();
  if ( n == 0 ) {
    return -1;
  }

  return random_select(mTmpList);
}

// @brief 候補リストを更新する．
// @param[in] node_id 新たに加わったノード
void
Isx::update_cand_list(int node_id)
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

  int rpos = 0;
  int wpos = 0;
  int n = mCandList.size();
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
int
Isx::random_select(const vector<int>& cand_list)
{
  int n = cand_list.size();
  ASSERT_COND( n > 0 );

  std::uniform_int_distribution<int> rd(0, n - 1);
  int r = rd(mRandGen);
  return cand_list[r];
}

END_NAMESPACE_YM_UDGRAPH
