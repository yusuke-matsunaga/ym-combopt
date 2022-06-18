
/// @file coloring.cc
/// @brief coloring の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "Dsatur.h"
#include "IsCov.h"
#include "Isx.h"
#include "Isx2.h"
#include "TabuCol.h"


BEGIN_NAMESPACE_YM

// dsatur で彩色問題を解く．
SizeType
dsatur(
  const UdGraph& graph,
  vector<SizeType>& color_map
)
{
  nsUdGraph::Dsatur dsatsolver(graph);
  return dsatsolver.coloring(color_map);
}

// tabucol で彩色問題を解く．
SizeType
tabucol(
  const UdGraph& graph,
  vector<SizeType>& color_map
)
{
  SizeType k0 = dsatur(graph, color_map);
  SizeType limit = 100000;
  SizeType L = 9;
  double alpha = 0.6;
  SizeType k1 = k0;
  for ( SizeType k = k0; k > 0; -- k ) {
    nsUdGraph::TabuCol tabucol{graph, k};
    vector<SizeType> color_map1;
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

SizeType
UdGraph::coloring(
  vector<SizeType>& color_map,
  const string& algorithm
) const
{
  if ( algorithm == "dsatur" ) {
    return dsatur(*this, color_map);
  }
  else if ( algorithm == "iscov" ) {
    nsUdGraph::IsCov iscsolver(*this);
    SizeType c = iscsolver.covering(500, color_map);
    nsUdGraph::Dsatur dsatsolver(*this, color_map);
    return dsatsolver.coloring(color_map);
  }
  else if ( algorithm == "isx" ) {
    nsUdGraph::Isx isxsolver(*this);
    SizeType c = isxsolver.coloring(500, color_map);
    //cout << "isx end: c = " << c << endl;
    nsUdGraph::Dsatur dsatsolver(*this, color_map);
    //return dsatsolver.coloring(color_map);
    c = dsatsolver.coloring(color_map);
    //cout << "dsatur end: c = " << c << endl;
    return c;
  }
  else if ( algorithm == "isx2" ) {
    nsUdGraph::Isx2 isxsolver(*this);
    SizeType c = isxsolver.coloring(500, color_map);
    //cout << "isx2 end: c = " << c << endl;
    nsUdGraph::Dsatur dsatsolver(*this, color_map);
    //return dsatsolver.coloring(color_map);
    c = dsatsolver.coloring(color_map);
    //cout << "dsatur end: c = " << c << endl;
    return c;
  }
  else if ( algorithm == "tabucol" ) {
    return tabucol(*this, color_map);
  }
  else {
    // デフォルトフォールバック
    return dsatur(*this, color_map);
  }
}

END_NAMESPACE_YM
