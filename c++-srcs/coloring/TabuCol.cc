
/// @file TabuCol.cc
/// @brief TabuCol の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "TabuCol.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス TabuCol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TabuCol::TabuCol(
  const UdGraph& graph,
  SizeType k
) : ColGraph{graph},
    mK(k)
{
  init();
}

// @brief コンストラクタ
TabuCol::TabuCol(
  const UdGraph& graph,
  const vector<SizeType>& color_map,
  SizeType k
) : ColGraph{graph, color_map},
    mK(k)
{
  init();
}

// @brief 内部データ構造の初期化を行う．
void
TabuCol::init()
{
  SizeType n = node_num();
  mGammaTable = new int[n * mK];
  mTabuMatrix = new SizeType[n * mK];
}

// @brief デストラクタ
TabuCol::~TabuCol()
{
  delete [] mGammaTable;
  delete [] mTabuMatrix;
}

// @brief 彩色が可能か調べる．
bool
TabuCol::coloring(
  SizeType iter_limit,
  SizeType L,
  double alpha,
  vector<SizeType>& color_map)
{
  gen_random_solution();

  for ( mIter = 0; mIter < iter_limit; ++ mIter ) {
    SizeType nc = conflict_num();
    if ( nc == 0 ) {
      // 衝突なく彩色ができたら終わる．
      break;
    }

    // 最良ムーブを取り出す．
    auto p = get_move();
    SizeType node_id = p.first;
    SizeType col = p.second;

    // 逆のムーブをタブーリストに加える．
    SizeType tenure = L + static_cast<SizeType>(alpha * nc);
    add_tabu(node_id, col, tenure);

    // ムーブに従って色を変える．
    SizeType old_col = color(node_id);
    ASSERT_COND( old_col != col );

    set_color(node_id, col);

    for ( auto node1_id: adj_list(node_id) ) {
      -- mGammaTable[encode(node1_id, old_col)];
      ++ mGammaTable[encode(node1_id, col)];
    }
  }

  get_color_map(color_map);

  return conflict_num() == 0;
}

// @brief 初期解を作る．
void
TabuCol::gen_random_solution()
{
  SizeType n = node_num();

  // ランダムに色を割り当てる．
  for ( auto node_id: node_list() ) {
    std::uniform_int_distribution<SizeType> rd_int(1, mK);
    SizeType color = rd_int(mRandGen);
    set_color(node_id, color);
  }

  // mGammaTable を初期化する．
  for ( auto i: Range(n * mK) ) {
    mGammaTable[i] = 0;
  }
  for ( auto node_id: node_list() ) {
    SizeType c = color(node_id);
    for ( auto node1_id: adj_list(node_id) ) {
      ++ mGammaTable[encode(node1_id, c)];
    }
  }

  // mTabuMatrix を初期化する．
  for ( auto i: Range(n * mK) ) {
    mTabuMatrix[i] = 0;
  }
}

// @brief γ(node, col) が最小となる move を得る．
pair<SizeType, SizeType>
TabuCol::get_move()
{
  SizeType nc = conflict_num();
  SizeType min_val = node_num() + 1;
  vector<pair<SizeType, SizeType>> cand_list;
  for ( auto node_id: node_list() ) {
    int g = gamma(node_id, color(node_id));
    if ( g == 0 ) {
      // conflict していない節点は除外する．
      continue;
    }
    for ( auto col1: Range(1, mK + 1) ) {
      if ( col1 == color(node_id) ) {
	// 同じ色も除外する．
	continue;
      }
      int d = gamma(node_id, col1) - g;
      if ( check_tabu(node_id, col1) ) {
	if ( min_val >= d ) {
	  if ( min_val > d ) {
	    min_val = d;
	    cand_list.clear();
	  }
	  cand_list.push_back(make_pair(node_id, col1));
	}
      }
      else {
	// タブーリストで禁止されていた．
	if ( d == - nc ) {
	  // だけどこれ一発で解決するなら許しちゃう．
	  return make_pair(node_id, col1);
	}
      }
    }
  }

  SizeType n = cand_list.size();
  ASSERT_COND( n > 0 );
  if ( n == 1 ) {
    return cand_list[0];
  }
  else {
    std::uniform_int_distribution<SizeType> rd_int(0, n - 1);
    SizeType r = rd_int(mRandGen);
    return cand_list[r];
  }
}

// @brief conflict vertices の個数を数える．
SizeType
TabuCol::conflict_num() const
{
  SizeType n = 0;
  for ( auto node_id: node_list() ) {
    int g = gamma(node_id, color(node_id));
#if 0
    if ( g > 0 ) {
      ++ n;
    }
#else
    n += g;
#endif
  }
  return n;
}

END_NAMESPACE_YM_UDGRAPH
