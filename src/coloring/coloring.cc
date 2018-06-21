
/// @file coloring.cc
/// @brief coloring の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "Dsatur.h"
#include "IsCov.h"
#include "Isx.h"
#include "Isx2.h"
#include "TabuCol.h"


BEGIN_NAMESPACE_YM

// dsatur で彩色問題を解く．
int
dsatur(const UdGraph& graph,
       vector<int>& color_map)
{
  nsUdGraph::Dsatur dsatsolver(graph);
  return dsatsolver.coloring(color_map);
}

// tabucol で彩色問題を解く．
int
tabucol(const UdGraph& graph,
	vector<int>& color_map)
{
  int k0 = dsatur(graph, color_map);
  int limit = 100000;
  int L = 9;
  double alpha = 0.6;
  int k1 = k0;
  for ( int k = k0; k > 0; -- k ) {
    nsUdGraph::TabuCol tabucol(graph, k);
    vector<int> color_map1;
    if ( tabucol.coloring(limit, L, alpha, color_map1) ) {
      k1 = k;
      color_map = color_map1;
    }
    else {
      break;
    }
  }
  return k1;
}

// @brief 彩色問題を解く
// @param[in] graph 対象のグラフ
// @param[in] algorithm アルゴリズム名
// @param[out] color_map ノードに対する彩色結果(=int)を収める配列
// @return 彩色数を返す．
int
coloring(const UdGraph& graph,
	 const string& algorithm,
	 vector<int>& color_map)
{
  if ( algorithm == "dsatur" ) {
    return dsatur(graph, color_map);
  }
  else if ( algorithm == "iscov" ) {
    nsUdGraph::IsCov iscsolver(graph);
    int c = iscsolver.covering(500, color_map);
    nsUdGraph::Dsatur dsatsolver(graph, color_map);
    return dsatsolver.coloring(color_map);
  }
  else if ( algorithm == "isx" ) {
    nsUdGraph::Isx isxsolver(graph);
    int c = isxsolver.coloring(500, color_map);
    cout << "isx end: c = " << c << endl;
    nsUdGraph::Dsatur dsatsolver(graph, color_map);
    //return dsatsolver.coloring(color_map);
    c = dsatsolver.coloring(color_map);
    cout << "dsatur end: c = " << c << endl;
    return c;
  }
  else if ( algorithm == "isx2" ) {
    nsUdGraph::Isx2 isxsolver(graph);
    int c = isxsolver.coloring(500, color_map);
    cout << "isx2 end: c = " << c << endl;
    nsUdGraph::Dsatur dsatsolver(graph, color_map);
    //return dsatsolver.coloring(color_map);
    c = dsatsolver.coloring(color_map);
    cout << "dsatur end: c = " << c << endl;
    return c;
  }
  else if ( algorithm == "tabucol" ) {
    return tabucol(graph, color_map);
  }
  else {
    // デフォルトフォールバック
    return dsatur(graph, color_map);
  }
}

// @relates UdGraph
// @brief 彩色問題を解く
// @param[in] graph 対象のグラフ
// @param[out] color_map ノードに対する彩色結果(=int)を収める配列
// @return 彩色数を返す．
//
// デフォルトのヒューリスティックを用いる．
int
coloring(const UdGraph& graph,
	 vector<int>& color_map)
{
  return coloring(graph, string(), color_map);
}

END_NAMESPACE_YM
