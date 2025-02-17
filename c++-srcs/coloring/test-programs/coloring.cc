
/// @file coloring.cc
/// @brief coloring の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "ym/Timer.h"
#include <libgen.h>


BEGIN_NAMESPACE_YM

void
usage(
  char* arg0
)
{
  cerr << "Usage: " << basename(arg0) << " <dimacs-file>" << endl;
}


int
coloring(
  int argc,
  char** argv
)
{
  string filename;
  string algorithm;

  switch ( argc ) {
  case 2:
    filename = argv[1];
    algorithm = string{};
    break;

  case 3:
    filename = argv[2];
    algorithm = argv[1];
    break;

  default:
    usage(argv[0]);
    return 1;
  }

  try {
    Timer timer;
    timer.start();
    auto graph = UdGraph::read_dimacs(filename);
    timer.stop();
    cout << "Reading " << filename << " end." << endl
	 << " " << timer.get_time() / 1000.0 << "(s)" << endl;

    timer.reset();
    timer.start();
    vector<SizeType> color_map;
    auto nc = graph.coloring(color_map, algorithm);
    timer.stop();
    cout << "Coloring(" << algorithm << ") end." << endl
	 << " " << nc << " colors. " << timer.get_time() / 1000.0 << "(s)" << endl;
  }
  catch ( std::invalid_argument& error ) {
    cerr << error.what() << endl;
    return 1;
  }
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return nsYm::coloring(argc, argv);
}
