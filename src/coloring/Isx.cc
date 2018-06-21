
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
    color_nodes(mIndepSet);

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
  {
    int r = mRandGen.int32() % mCandList.size();
    int node0 = mCandList[r];
    mIndepSet.push_back(node0);
  }
  while ( !mCandList.empty() ) {
    int node_id = select_node();
    ASSERT_COND( node_id != -1 );

    mIndepSet.push_back(node_id);

    // cand_list を更新する．
    update_cand_list(node_id);
  }
  sort(mIndepSet.begin(), mIndepSet.end());
}

// @brief mCandList, mCandMark を初期化する．
void
Isx::init_cand_list()
{
  for ( auto node_id: Range(node_num()) ) {
    if ( color(node_id) == 0 ) {
      mCandList.push_back(node_id);
      mCandMark[node_id] = true;
    }
  }

  for ( auto node_id: mCandList ) {
    mAdjCount[node_id] = 0;
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

  ASSERT_COND( n > 0 );

  int r = mRandGen.int32() % n;
  return mTmpList[r];
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

  // cand_mark に従って cand_list を更新する．
  int n = mCandList.size();
  int rpos = 0;
  int wpos = 0;
  for ( rpos = 0; rpos < n; ++ rpos ) {
    int node1_id = mCandList[rpos];
    if ( mCandMark[node1_id] ) {
      mCandList[wpos] = node1_id;
      ++ wpos;
    }
  }
  if ( wpos < n ) {
    mCandList.erase(mCandList.begin() + wpos, mCandList.end());
  }
}

// @brief ノード集合に新しい色を割り当てる．
// @param[in] node_set ノード集合
void
Isx::color_nodes(const vector<int>& node_set)
{
  int num = node_set.size();
  ASSERT_COND( num > 0 );

  int node_id = node_set[0];
  int cur_col = set_new_color(node_id);
  for ( auto i: Range(1, num) ) {
    int node_id = node_set[i];
    set_color(node_id, cur_col);
  }
}

END_NAMESPACE_YM_UDGRAPH
