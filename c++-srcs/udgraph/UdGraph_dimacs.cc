
/// @file UdGraph_dimacs.cc
/// @brief UdGraph_dimacs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "ym/MsgMgr.h"
#include "ym/split.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス UdGraph の DIMACS 形式のファイルの読み書きに関する関数
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

void
syntax_error(
  int line
)
{
  ostringstream err;
  err << "Line " << line << ": Syntax error";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  MsgType::Error,
		  "DIMACS002",
		  err.str());
  throw UdgError{err.str()};
}

END_NONAMESPACE

UdGraph
UdGraph::read_dimacs(
  const string& filename
)
{
  ifstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS005",
		    err.str());
    throw UdgError{err.str()};
  }
  return read_dimacs(s);
}

UdGraph
UdGraph::read_dimacs(
  istream& s
)
{
  bool first = true;
  int line = 1;
  string buff;
  SizeType node_num = 0;
  SizeType edge_num = 0;
  vector<pair<SizeType, SizeType> > tmp_edge_list;
  SizeType max_node_id = 0;
  // ファイルをスキャンする．
  // - 'p' 行から node_num, edge_num を得る．
  // - 'e' 行の内容を tmp_edge_list に入れる．
  // - 'e' 行に現れるノード番号の最大値を max_node_id に入れる．
  while ( getline(s, buff) ) {
    if ( buff[0] == 'c' ) {
      // コメント行は読み飛ばす
      continue;
    }

    auto str_list = split(buff);
    if ( str_list.empty() ) {
      syntax_error(line);
    }

    if ( str_list[0] == "p" ) {
      if ( !first ) {
	ostringstream err;
	err << "Line " << line
	    << ": 'p' line is allowed only once";
	MsgMgr::put_msg(__FILE__, __LINE__,
			MsgType::Error,
			"DIMACS001",
			err.str());
	throw UdgError{err.str()};
      }

      if ( str_list.size() != 4 || str_list[1] != "edge" ) {
	syntax_error(line);
      }
      node_num = atoi(str_list[2].c_str());
      edge_num = atoi(str_list[3].c_str());
    }
    else if ( str_list[0] == "e" ) {
      if ( str_list.size() != 3 ) {
	syntax_error(line);
      }
      SizeType id1 = atoi(str_list[1].c_str());
      SizeType id2 = atoi(str_list[2].c_str());
      tmp_edge_list.push_back(make_pair(id1, id2));
      if ( max_node_id < id1 ) {
	max_node_id = id1;
      }
      if ( max_node_id < id2 ) {
	max_node_id = id2;
      }
    }
    else {
      syntax_error(line);
    }
    ++ line;
  }

  if ( node_num < max_node_id ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS003",
		    "# of nodes corrected");
    node_num = max_node_id;
  }
  if ( edge_num != tmp_edge_list.size() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS004",
		    "# of edges corrected");
    // 実は edge_num は使わない．
  }

  UdGraph graph{node_num};
  for ( auto edge_pair: tmp_edge_list ) {
    SizeType id1 = edge_pair.first - 1;
    SizeType id2 = edge_pair.second - 1;
    graph.connect(id1, id2);
  }

  return graph;
}

void
UdGraph::write_dimacs(
  const string& filename
) const
{
  ofstream s{filename};
  if ( !s ) {
    ostringstream err;
    err << filename << ": Could not create file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS006",
		    err.str());
    throw UdgError{err.str()};
  }
  write_dimacs(s);
}

void
UdGraph::write_dimacs(
  ostream& s
) const
{
  s << "p edge " << node_num() << " " << edge_list().size() << endl;
  for ( auto edge: edge_list() ) {
    s << "e " << edge.id1() + 1 << " " << edge.id2() + 1 << endl;
  }
}

END_NAMESPACE_YM_UDGRAPH
