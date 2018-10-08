
/// @file Isx2.cc
/// @brief Isx2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "Isx2.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

BEGIN_NONAMESPACE

// ２つの vector<int> を辞書式順序で比較する．
int
vect_comp(const vector<int>& a,
	  const vector<int>& b)
{
  int n1 = a.size();
  int n2 = b.size();
  for ( int i = 0; i < n1 && i < n2; ++ i ) {
    int d = a[i] - b[i];
    if ( d < 0 ) {
      return -1;
    }
    else if ( d > 0 ) {
      return 1;
    }
  }
  // ここに来ているということは min(n1, n2) までは同一
  if ( n1 == 0 && n2 == 0 ) {
    return 0;
  }
  if ( n1 > 0 ) {
    return 1;
  }
  else {
    return -1;
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Isx2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
Isx2::Isx2(const UdGraph& graph) :
  ColGraph(graph),
  mCandMark(node_num(), false),
  mAdjCount(node_num(), 0)
{
  mCandList.reserve(node_num());
  mTmpList.reserve(node_num());
  mIndepSet.reserve(node_num());

  mRandRatio = 0.5;
}

// @brief デストラクタ
Isx2::~Isx2()
{
}

// @brief independent set extraction を用いた coloring を行う．
// @param[in] limit 残りのノード数がこの値を下回ったら処理をやめる．
// @param[out] color_map 彩色結果を収める配列
// @return 彩色数を返す．
//
// ここでは部分的な彩色を行う．
int
Isx2::coloring(int limit,
	      vector<int>& color_map)
{
  int remain_num = node_num();
  int dlimit = 100;
  int slimit = static_cast<int>(edge_num() * 2.0 / (node_num() - 1.0));

  while ( remain_num > limit ) {
    {
      cout << "# of remaining nodes: " << remain_num << endl;
    }

    int dcount = 0;
    mIndepSetList.clear();
    while ( dcount < dlimit && mIndepSetList.size() < slimit ) {
      get_indep_set();

      // mIndepSetList に昇順で挿入する．
      if ( add_indep_set(mIndepSet) ) {
	dcount = 0;
      }
      else {
	++ dcount;
      }
    }

    // pairwise disjoint な極大集合を求める．
    int max_num = 0;
    int max_nodes = 0;
    vector<int> max_iset;
    for ( int i = 0; i < 100; ++ i ) {
      vector<int> tmp_set;
      get_max_disjoint_set(tmp_set);
      int num = tmp_set.size();
      int nodes = 0;
      for ( auto i: tmp_set ) {
	nodes += mIndepSetList[i].size();
      }
      if ( max_num < num ) {
	max_num = num;
	max_nodes = nodes;
	max_iset = tmp_set;
      }
    }

    {
      cout << "choose " << max_iset.size() << " disjoint sets" << endl;
    }

    // 選ばれた独立集合に基づいて彩色を行う．
    for ( auto i: max_iset ) {
      set_color(mIndepSetList[i], new_color());
      remain_num -= mIndepSetList[i].size();
    }
  }

  return get_color_map(color_map);
}

// @brief maximal independent set を選ぶ．
//
// - 結果は mIndepSet に格納される．
// - mRandGen を用いてランダムに選ぶ．
void
Isx2::get_indep_set()
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

// @brief 独立集合を mIndepSetList に追加する．
// @param[in] indep_set 追加する独立集合
// @retval true 正常に追加した．
// @retval false すでに同じ内容の独立集合が存在した．
bool
Isx2::add_indep_set(const vector<int>& indep_set)
{
  vector<vector<int> >::iterator wpos = mIndepSetList.begin();
  for ( ; wpos != mIndepSetList.end(); ++ wpos) {
    int c = vect_comp(*wpos, indep_set);
    if ( c == 0 ) {
      // 同じものは登録しない．
      return false;
    }
    else if ( c == 1 ) {
      break;
    }
  }

  mIndepSetList.insert(wpos, indep_set);

  return true;
}

BEGIN_NONAMESPACE

struct IsetLt
{
  IsetLt(const vector<vector<int> >& iset_list) :
    mIndepSetList(iset_list)
  {
  }

  bool
  operator()(int a,
	     int b)
  {
    if ( mIndepSetList[a].size() > mIndepSetList[b].size() ) {
      return true;
    }
    else {
      return false;
    }
  }

  const vector<vector<int> >& mIndepSetList;
};

END_NONAMESPACE

// @brief pairwise disjoint な極大集合を求める．
// @param[out] max_iset 結果を集合番号を収めるベクタ
void
Isx2::get_max_disjoint_set(vector<int>& max_iset)
{
  int i0 = mRandGen.int32() % mIndepSetList.size();
  max_iset.push_back(i0);

  vector<bool> check_vec(node_num(), false);
  const vector<int>& iset0 = mIndepSetList[i0];
  for ( auto i: iset0 ) {
    check_vec[i] = true;
  }
  // iset0 と disjoint な集合の番号を cand_list に入れる．
  vector<int> cand_list;
  cand_list.reserve(mIndepSetList.size());
  for ( int i: Range(mIndepSetList.size()) ) {
    if ( i == i0 ) {
      continue;
    }
    const vector<int>& iset = mIndepSetList[i];
    bool disjoint = true;
    for ( auto j: iset ) {
      if ( check_vec[j] ) {
	disjoint = false;
	break;
      }
    }
    if ( disjoint ) {
      cand_list.push_back(i);
    }
  }

  // cand_list をサイズの降順に並べる．
  sort(cand_list.begin(), cand_list.end(), IsetLt(mIndepSetList));

  while ( cand_list.size() > 0 ) {
    int n0 = mIndepSetList[cand_list[0]].size();
    int end = 0;
    for ( ; mIndepSetList[cand_list[end]].size() == n0; ++ end ) { }
    int r = mRandGen.int32() % end;
    int i1 = cand_list[r];
    max_iset.push_back(i1);

    const vector<int>& iset1 = mIndepSetList[i1];
    vector<bool> check_vec(node_num(), false);
    for ( auto i: iset1 ) {
      check_vec[i] = true;
    }
    // iset1 と disjoint な集合を cand_list に残す．
    vector<int>::iterator rpos = cand_list.begin();
    vector<int>::iterator wpos = rpos;
    for ( ; rpos != cand_list.end(); ++ rpos ) {
      const vector<int>& iset = mIndepSetList[*rpos];
      bool disjoint = true;
      for ( auto i: iset ) {
	if ( check_vec[i] ) {
	  disjoint = false;
	  break;
	}
      }
      if ( disjoint ) {
	if ( wpos != rpos ) {
	  *wpos = *rpos;
	}
	++ wpos;
      }
    }
    cand_list.erase(wpos, cand_list.end());
  }
}

// @brief mCandList, mCandMark を初期化する．
void
Isx2::init_cand_list()
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
Isx2::select_node()
{
  ASSERT_COND( mCandList.size() > 0 );

  if ( mRandGen.real1() < mRandRatio ) {
    // 一定の確率でランダムに選ぶ．
    int n = mCandList.size();
    int r = mRandGen.int32() % n;
    return mCandList[r];
  }
  else {
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
}

// @brief 候補リストを更新する．
// @param[in] node_id 新たに加わったノード
void
Isx2::update_cand_list(int node_id)
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

END_NAMESPACE_YM_UDGRAPH
