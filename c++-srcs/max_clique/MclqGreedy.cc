
/// @file MclqGreedy.cc
/// @brief max clique を greedy で解くヒューリスティック
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "MclqGraph.h"
#include "ym/HeapTree.h"


BEGIN_NAMESPACE_YM_MAXCLIQUE

BEGIN_NONAMESPACE

// @brief max_clique 用の比較関数
class MclqCompare
{
public:

  /// @brief コンストラクタ
  MclqCompare(
    const vector<SizeType>& num_array ///< [in] 値の配列
  ) : mNumArray{num_array}
  {
  }

  /// @brief 比較関数
  int
  operator()(
    SizeType id1,
    SizeType id2
  ) const
  {
    auto num1 = mNumArray[id1];
    auto num2 = mNumArray[id2];
    if ( num1 > num2 ) {
      return -1;
    }
    if ( num1 < num2 ) {
      return 1;
    }
    return 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の配列
  const vector<SizeType>& mNumArray;

};

END_NONAMESPACE


// @brief greedy ヒューリスティックで解を求める．
vector<SizeType>
MclqGreedy(
  const MclqGraph& graph
)
{
  SizeType nn = graph.node_num();

  // アクティブな隣接ノード数
  vector<SizeType> num_array(nn, 0);
  for ( SizeType id = 0; id < nn; ++ id ) {
    num_array[id] = graph.adj_num(id);
  }

  MclqCompare comp(num_array);

  // ノードをヒープに積む．
  using MclqHeap = HeapTree<SizeType, MclqCompare>;
  MclqHeap node_heap(comp, nn);

  // node_heap に入っているノード番号のリスト
  vector<SizeType> node_list;
  node_list.reserve(nn);

  // node_heap に入っているノードのマーク
  vector<bool> node_mark(nn, false);
  for ( SizeType id = 0; id < nn; ++ id ) {
    node_heap.put_item(id);
    node_list.push_back(id);
    node_mark[id] = true;
  }

  // 作業用のフラグ配列
  vector<bool> mark(nn, false);

  // 結果を保持する配列
  vector<SizeType> node_set;
  node_set.reserve(nn);

  // 未処理のノードのうちアクティブな隣接ノード数が最大のものを取り出し，解に加える．
  while ( !node_heap.empty() ) {
    auto best_id = node_heap.get_min();
    node_mark[best_id] = false;
    node_set.push_back(best_id);

    // best_node に隣接しているノードにマークをつける．
    for ( auto id1: graph.adj_list(best_id) ) {
      mark[id1] = true;
    }

    // マークのついていないノードを削除する．
    // node_list を直接操作するのは面倒なので
    // 一旦 tmp_list にコピーする．
    vector<SizeType> tmp_list;
    tmp_list.reserve(node_list.size());
    for ( auto id: node_list ) {
      if ( id != best_id ) {
	ASSERT_COND( node_mark[id] );
	tmp_list.push_back(id);
      }
    }
    node_list.clear();
    for ( auto id: tmp_list ) {
      if ( mark[id] ) {
	node_list.push_back(id);
      }
      else {
	// このノードを削除する．
	node_heap.delete_item(id);
	node_mark[id] = false;
	// さらにこのノードに隣接しているノードの隣接数を減らす．
	for ( auto id1: graph.adj_list(id) ) {
	  if ( node_mark[id1] ) {
	    -- num_array[id1];
	    node_heap.update(id1);
	  }
	}
      }
    }

    // マークを消す．
    for ( auto id1: graph.adj_list(best_id) ) {
      // best_id に隣接しているノードのマークを消す．
      mark[id1] = false;
    }
  }

  return node_set;
}

END_NAMESPACE_YM_UDGRAPH
