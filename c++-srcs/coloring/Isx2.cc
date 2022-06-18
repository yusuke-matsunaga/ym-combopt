
/// @file Isx2.cc
/// @brief Isx2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Isx2.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

BEGIN_NONAMESPACE

bool debug = 0;

// ２つの vector<SizeType> を辞書式順序で比較する．
int
vect_comp(
  const vector<SizeType>& a,
  const vector<SizeType>& b
)
{
  SizeType n1 = a.size();
  SizeType n2 = b.size();
  SizeType n_min = std::min(n1, n2);
  for ( SizeType i = 0; i < n_min; ++ i ) {
    int d = a[i] - b[i];
    if ( d < 0 ) {
      return -1;
    }
    else if ( d > 0 ) {
      return 1;
    }
  }
  // ここに来ているということは min(n1, n2) までは同一
  if ( n1 == n_min ) {
    if ( n2 == n_min ) {
      return 0;
    }
    else {
      return -1;
    }
  }
  else {
    return 1;
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Isx2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Isx2::Isx2(
  const UdGraph& graph
) : ColGraph{graph},
    mCandMark(node_num(), false),
    mAdjCount(node_num(), 0)
{
  mCandList.reserve(node_num());
  mIndepSet.reserve(node_num());

  mRandRatio = 0.5;
}

// @brief デストラクタ
Isx2::~Isx2()
{
}

// @brief independent set extraction を用いた coloring を行う．
SizeType
Isx2::coloring(
  SizeType limit,
  vector<SizeType>& color_map
)
{
  SizeType remain_num = node_num();
  SizeType dlimit = 100;
  SizeType slimit = static_cast<SizeType>(edge_num() * 2.0 / (node_num() - 1.0));

  while ( remain_num > limit ) {
    if ( debug ) {
      cout << "# of remaining nodes: " << remain_num << endl;
    }

    SizeType dcount = 0;
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
    SizeType max_num = 0;
    SizeType max_nodes = 0;
    vector<SizeType> max_iset;
    for ( SizeType i = 0; i < 100; ++ i ) {
      vector<SizeType> tmp_set;
      get_max_disjoint_set(tmp_set);
      SizeType num = tmp_set.size();
      SizeType nodes = 0;
      for ( auto i: tmp_set ) {
	nodes += mIndepSetList[i].size();
      }
      if ( max_num < num ) {
	max_num = num;
	max_nodes = nodes;
	max_iset = tmp_set;
      }
    }

    if ( debug ) {
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
void
Isx2::get_indep_set()
{
  // 未彩色のノードを cand_list に入れる．
  init_cand_list();

  // ノードを一つづつ選択し mIndepSet に入れる．
  mIndepSet.clear();
  {
    std::uniform_int_distribution<SizeType> rd(0, mCandList.size() - 1);
    SizeType r = rd(mRandGen);
    SizeType node0 = mCandList[r];
    mIndepSet.push_back(node0);
  }
  while ( !mCandList.empty() ) {
    SizeType node_id = select_node();
    mIndepSet.push_back(node_id);

    // cand_list を更新する．
    update_cand_list(node_id);
  }
  sort(mIndepSet.begin(), mIndepSet.end());
}

// @brief 独立集合を mIndepSetList に追加する．
bool
Isx2::add_indep_set(
  const vector<SizeType>& indep_set
)
{
  auto wpos = mIndepSetList.begin();
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
  IsetLt(
    const vector<vector<SizeType>>& iset_list
  ) : mIndepSetList{iset_list}
  {
  }

  bool
  operator()(
    SizeType a,
    SizeType b
  )
  {
    if ( mIndepSetList[a].size() > mIndepSetList[b].size() ) {
      return true;
    }
    else {
      return false;
    }
  }

  const vector<vector<SizeType>>& mIndepSetList;
};

END_NONAMESPACE


// @brief pairwise disjoint な極大集合を求める．
void
Isx2::get_max_disjoint_set(
  vector<SizeType>& max_iset
)
{
  std::uniform_int_distribution<SizeType> rd(0, mIndepSetList.size() - 1);
  SizeType i0 = rd(mRandGen);
  max_iset.push_back(i0);

  vector<bool> check_vec(node_num(), false);
  const vector<SizeType>& iset0 = mIndepSetList[i0];
  for ( auto i: iset0 ) {
    check_vec[i] = true;
  }
  // iset0 と disjoint な集合の番号を cand_list に入れる．
  vector<SizeType> cand_list;
  cand_list.reserve(mIndepSetList.size());
  for ( int i: Range(mIndepSetList.size()) ) {
    if ( i == i0 ) {
      continue;
    }
    const vector<SizeType>& iset = mIndepSetList[i];
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
    SizeType n0 = mIndepSetList[cand_list[0]].size();
    SizeType end = 0;
    for ( ; mIndepSetList[cand_list[end]].size() == n0; ++ end ) { }
    std::uniform_int_distribution<SizeType> rd(0, end - 1);
    SizeType r = rd(mRandGen);
    SizeType i1 = cand_list[r];
    max_iset.push_back(i1);

    const vector<SizeType>& iset1 = mIndepSetList[i1];
    vector<bool> check_vec(node_num(), false);
    for ( auto i: iset1 ) {
      check_vec[i] = true;
    }
    // iset1 と disjoint な集合を cand_list に残す．
    auto rpos = cand_list.begin();
    auto wpos = rpos;
    for ( ; rpos != cand_list.end(); ++ rpos ) {
      const auto& iset = mIndepSetList[*rpos];
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
SizeType
Isx2::select_node()
{
  ASSERT_COND( mCandList.size() > 0 );

  std::uniform_real_distribution<double> rd_real(0, 1.0);
  if ( rd_real(mRandGen) < mRandRatio ) {
    // 一定の確率でランダムに選ぶ．
    std::uniform_int_distribution<SizeType> rd_int(0, mCandList.size() - 1);
    SizeType r = rd_int(mRandGen);
    return mCandList[r];
  }
  else {
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

    SizeType n = tmp_list.size();
    ASSERT_COND( n > 0 );
    std::uniform_int_distribution<SizeType> rd_int(0, n - 1);
    SizeType r = rd_int(mRandGen);
    return tmp_list[r];
  }
}

// @brief 候補リストを更新する．
void
Isx2::update_cand_list(
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

  // cand_mark に従って cand_list を更新する．
  SizeType n = mCandList.size();
  SizeType rpos = 0;
  SizeType wpos = 0;
  for ( rpos = 0; rpos < n; ++ rpos ) {
    SizeType node1_id = mCandList[rpos];
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
