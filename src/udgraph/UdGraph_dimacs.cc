
/// @file UdGraph_dimacs.cc
/// @brief UdGraph_dimacs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/UdGraph.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス UdGraph の DIMACS 形式のファイルの読み書きに関する関数
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 文字列を空白で区切る
void
split(const string& src_str,
      vector<string>& str_list)
{
  string tmp_str(src_str);
  for ( ; ; ) {
    string::size_type p = tmp_str.find_first_of(' ');
    string tmp = tmp_str.substr(0, p);
    str_list.push_back(tmp);
    if ( p == string::npos ) {
      break;
    }
    tmp_str = tmp_str.substr(p + 1, string::npos);
  }
}

void
syntax_error(int line)
{
  ostringstream err;
  err << "Line " << line << ": Syntax error";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  MsgType::Error,
		  "DIMACS002",
		  err.str());
}

END_NONAMESPACE

// @relates UdGraph
// @brief DIMACS 形式のファイルを読み込む．
// @param[in] filename 入力元のファイル名
// @param[in] graph 読み込んだ内容を設定するグラフ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
read_dimacs(const string& filename,
	    UdGraph& graph)
{
  ifstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS005",
		    err.str());
    return false;
  }
  return read_dimacs(s, graph);
}

// @relates UdGraph
// @brief DIMACS 形式のファイルを読み込む．
// @param[in] s 入力元のストリーム
// @param[in] graph 読み込んだ内容を設定するグラフ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
read_dimacs(istream& s,
	    UdGraph& graph)
{
  bool first = true;
  int line = 1;
  string buff;
  int node_num = 0;
  int edge_num = 0;
  vector<pair<int, int> > tmp_edge_list;
  int max_node_id = 0;
  // ファイルをスキャンする．
  // - 'p' 行から node_num, edge_num を得る．
  // - 'e' 行の内容を tmp_edge_list に入れる．
  // - 'e' 行に現れるノード番号の最大値を max_node_id に入れる．
  while ( getline(s, buff) ) {
    if ( buff[0] == 'c' ) {
      // コメント行は読み飛ばす
      continue;
    }

    vector<string> str_list;
    split(buff, str_list);
    if ( str_list.empty() ) {
      syntax_error(line);
      return false;
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
	return false;
      }

      if ( str_list.size() != 4 || str_list[1] != "edge" ) {
	syntax_error(line);
	return false;
      }
      node_num = atoi(str_list[2].c_str());
      edge_num = atoi(str_list[3].c_str());
    }
    else if ( str_list[0] == "e" ) {
      if ( str_list.size() != 3 ) {
	syntax_error(line);
	return false;
      }
      int id1 = atoi(str_list[1].c_str());
      int id2 = atoi(str_list[2].c_str());
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
      return false;
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

  graph.resize(node_num);
  for ( auto edge_pair: tmp_edge_list ) {
    int id1 = edge_pair.first - 1;
    int id2 = edge_pair.second - 1;
    graph.connect(id1, id2);
  }

  return true;
}

// @relates UdGraph
// @brief 内容を DIMACS 形式で出力する．
// @param[in] filename 出力先のファイル名
// @param[in] graph 対象のグラフ
void
write_dimacs(const string& filename,
	     const UdGraph& graph)
{
  ofstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": Could not create file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS006",
		    err.str());
    return;
  }
  write_dimacs(s, graph);
}

// @relates UdGraph
// @brief 内容を DIMACS 形式で出力する．
// @param[in] s 出力先のストリーム
// @param[in] graph 対象のグラフ
void
write_dimacs(ostream& s,
	     const UdGraph& graph)
{
  s << "p edge " << graph.node_num() << " " << graph.edge_list().size() << endl;
  for ( auto edge: graph.edge_list() ) {
    s << "e " << edge.id1() + 1 << " " << edge.id2() + 1 << endl;
  }
}


END_NAMESPACE_YM
