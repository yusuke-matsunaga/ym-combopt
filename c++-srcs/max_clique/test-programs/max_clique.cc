
/// @file max_clique.cc
/// @brief max_clique の実装ファイル
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
max_clique(
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
    std::unordered_map<string, JsonValue> opt_dict;
    opt_dict.emplace("algorithm", JsonValue{algorithm});
    auto option = JsonValue{opt_dict};
    timer.start();
    auto node_set = graph.max_clique(option);
    timer.stop();
    cout << "Max_Clique(" << algorithm << ") end." << endl
	 << "  Clique Size: " << node_set.size() << ". "
	 << timer.get_time() / 1000.0 << "(s)" << endl;
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
  return nsYm::max_clique(argc, argv);
}
