
/// @file TabuCol.cc
/// @brief TabuCol の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "TabuCol.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス TabuCol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
// @param[in] k 彩色数
TabuCol::TabuCol(const UdGraph& graph,
		 int k) :
  ColGraph(graph),
  mK(k)
{
  init();
}

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
// @param[in] color_map 部分的な彩色結果
// @param[in] k 彩色数
TabuCol::TabuCol(const UdGraph& graph,
		 const vector<int>& color_map,
		 int k) :
  ColGraph(graph, color_map),
  mK(k)
{
  init();
}

// @brief 内部データ構造の初期化を行う．
void
TabuCol::init()
{
  int n = node_num();
  mGammaTable = new int[n * mK];
  mTabuMatrix = new int[n * mK];
}

// @brief デストラクタ
TabuCol::~TabuCol()
{
  delete [] mGammaTable;
  delete [] mTabuMatrix;
}

// @brief 彩色が可能か調べる．
// @param[in] iter_limit 最大の繰り返し回数
// @param[in] L タブー期間の基本パラメータ
// @param[in] alpha タブ期間の節点数依存パラメータ
// @param[out] color_map 彩色結果を入れる配列
// @retval true 彩色できた．
// @retval false 彩色できなかった．
//
// color_map[i] には i 番目の節点の色が入る．
bool
TabuCol::coloring(int iter_limit,
		  int L,
		  double alpha,
		  vector<int>& color_map)
{
  gen_random_solution();

  for ( mIter = 0; mIter < iter_limit; ++ mIter ) {
    int nc = conflict_num();
    if ( nc == 0 ) {
      break;
    }

    // 最良ムーブを取り出す．
    auto p = get_move();
    int node_id = p.first;
    int col = p.second;

    // 逆のムーブをタブーリストに加える．
    int tenure = L + static_cast<int>(alpha * nc);
    add_tabu(node_id, col, tenure);

    // ムーブに従って色を変える．
    int old_col = color(node_id);

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
  int n = node_num();

  // ランダムに色を割り当てる．
  for ( auto node_id: node_list() ) {
    int color = (mRandGen.int32() % mK) + 1;
    set_color(node_id, color);
  }

  // mGammaTable を初期化する．
  for ( auto i: Range(n * mK) ) {
    mGammaTable[i] = 0;
  }
  for ( auto node_id: node_list() ) {
    int c = color(node_id);
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
pair<int, int>
TabuCol::get_move()
{
  int nc = conflict_num();
  int min_val = node_num() + 1;
  vector<pair<int, int> > cand_list;
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
	if ( d == -nc ) {
	  // だけどこれ一発で解決するなら許しちゃう．
	  return make_pair(node_id, col1);
	}
      }
    }
  }

  int n = cand_list.size();
  ASSERT_COND( n > 0 );
  if ( n == 1 ) {
    return cand_list[0];
  }
  else {
    int r = mRandGen.int32() % n;
    return cand_list[r];
  }
}

// @brief conflict vertices の個数を数える．
int
TabuCol::conflict_num() const
{
  int n = 0;
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
