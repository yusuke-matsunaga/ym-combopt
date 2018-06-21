#ifndef YM_UDGRAPH_NSDEF_H
#define YM_UDGRAPH_NSDEF_H

/// @file ym/udgraph_nsdef.h
/// @brief UdGraph 関係の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

//////////////////////////////////////////////////////////////////////
// 名前空間の定義用マクロ
// namespace でネストするのがいやなので define マクロでごまかす．
//////////////////////////////////////////////////////////////////////

#define BEGIN_NAMESPACE_YM_UDGRAPH \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsUdGraph)

#define END_NAMESPACE_YM_UDGRAPH \
END_NAMESPACE(nsUdGraph) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_UDGRAPH

class UdGraph;
class UdEdge;

END_NAMESPACE_YM_UDGRAPH

BEGIN_NAMESPACE_YM

using nsUdGraph::UdGraph;
using nsUdGraph::UdEdge;

END_NAMESPACE_YM

#endif // YM_UDGRAPH_NSDEF_H
