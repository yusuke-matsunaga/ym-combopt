
/// @file UdGraph_coloring.cc
/// @brief UdGraph::coloring の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "Dsatur.h"
#include "IsCov.h"
#include "Isx.h"
#include "Isx2.h"
#include "TabuCol.h"


BEGIN_NAMESPACE_YM_UDGRAPH

// dsatur で彩色問題を解く．
inline
SizeType
dsatur(
  const UdGraph& graph,
  vector<SizeType>& color_map
)
{
  nsColoring::Dsatur dsat{graph};
  return dsat.coloring(color_map);
}

// dsatur で彩色問題を解く．
inline
SizeType
dsatur(
  const UdGraph& graph,
  vector<SizeType>& color_map,
  const vector<SizeType>& initial_color_map
)
{
  nsColoring::Dsatur dsat{graph, initial_color_map};
  return dsat.coloring(color_map);
}

// tabucol で彩色問題を解く．
SizeType
tabucol(
  const UdGraph& graph,
  vector<SizeType>& color_map
)
{
  // 初期解を Dsatur で得る．
  // ただし色数しか使わない．
  SizeType k0 = dsatur(graph, color_map);
  SizeType limit = 100000;
  SizeType L = 9;
  double alpha = 0.6;
  SizeType k1 = k0;
  for ( SizeType k = k0; k > 0; -- k ) {
    nsColoring::TabuCol tabucol{graph, k};
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
  const JsonValue& option
) const
{
  string algorithm = "dsatur"; // デフォルトフォールバック
  if ( option.is_object() && option.has_key("algorithm") ) {
    auto alg_obj = option.get("algorithm");
    algorithm = alg_obj.get_string();
  }
  if ( algorithm == "dsatur" ) {
    return dsatur(*this, color_map);
  }
  if ( algorithm == "iscov" ) {
    nsColoring::IsCov iscsolver(*this);
    SizeType c = iscsolver.covering(500, color_map);
    return dsatur(*this, color_map, color_map);
  }
  if ( algorithm == "isx" ) {
    nsColoring::Isx isxsolver(*this);
    SizeType c = isxsolver.coloring(500, color_map);
    //cout << "isx end: c = " << c << endl;
    return dsatur(*this, color_map, color_map);
  }
  if ( algorithm == "isx2" ) {
    nsColoring::Isx2 isxsolver(*this);
    SizeType c = isxsolver.coloring(500, color_map);
    //cout << "isx2 end: c = " << c << endl;
    return dsatur(*this, color_map, color_map);
  }
  if ( algorithm == "tabucol" ) {
    return tabucol(*this, color_map);
  }

  // デフォルトフォールバック
  return dsatur(*this, color_map);
}

END_NAMESPACE_YM_UDGRAPH
