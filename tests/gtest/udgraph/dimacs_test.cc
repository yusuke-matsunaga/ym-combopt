
/// @file dimacs_test.cc
/// @brief dimacs_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM

TEST(UdGraphTest, read_dimacs_error0)
{
  string filename = "bac_filename";
  bool caught = false;
  try {
    auto graph = UdGraph::read_dimacs(filename);
  }
  catch ( UdgError& error ) {
    caught = true;
  }
  ASSERT_TRUE( caught );
}

TEST(UdGraphTest, read_dimacs)
{
  string filename = string(TESTDATA_DIR) + string("/anna.col");
  ifstream s(filename);
  ASSERT_COND( static_cast<bool>(s) );

  try {
    UdGraph graph = UdGraph::read_dimacs(s);

    EXPECT_EQ( 138, graph.node_num() );
    EXPECT_EQ( 986, graph.edge_num() );

    ostringstream obuf;
    graph.write_dimacs(obuf);

    istringstream s1(obuf.str());
    UdGraph graph2 = UdGraph::read_dimacs(s1);

    ostringstream obuf2;
    graph2.write_dimacs(obuf2);

    EXPECT_EQ( obuf2.str(), obuf.str() );
  }
  catch ( UdgError& error ) {
    EXPECT_TRUE( false );
  }
}

END_NAMESPACE_YM
