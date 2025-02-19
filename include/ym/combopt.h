#ifndef YM_COMBOPT_H
#define YM_COMBOPT_H

/// @file ym/combopt.h
/// @brief 組み合わせ最適化問題用の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup MincovGroup Mincov Solver
///
/// 最小被覆問題を解くためのライブラリ

#include "ym_config.h"


/// @brief mincov 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MINCOV \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMincov)

/// @brief mincov 用の名前空間の終了
#define END_NAMESPACE_YM_MINCOV \
END_NAMESPACE(nsMincov) \
END_NAMESPACE_YM

/// @brief udgraph 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_UDGRAPH \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsUdGraph)

/// @brief udgraph 用の名前空間の終了
#define END_NAMESPACE_YM_UDGRAPH \
END_NAMESPACE(nsUdGraph) \
END_NAMESPACE_YM

/// @brief max_clique 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MAXCLIQUE \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMaxClique)

/// @brief max_clique 用の名前空間の終了
#define END_NAMESPACE_YM_MAXCLIQUE \
END_NAMESPACE(nsMaxClique) \
END_NAMESPACE_YM

/// @brief coloring 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_COLORING \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsColoring)

/// @brief coloring 用の名前空間の終了
#define END_NAMESPACE_YM_COLORING \
END_NAMESPACE(nsColoring) \
END_NAMESPACE_YM

/// @namespace nsYm::nsMincov
/// @brief MINCOV ソルバ関係のクラスが属する名前空間

BEGIN_NAMESPACE_YM_MINCOV

class MinCov;

END_NAMESPACE_YM_MINCOV

BEGIN_NAMESPACE_YM_UDGRAPH

class UdGraph;
class UdEdge;

END_NAMESPACE_YM_UDGRAPH

BEGIN_NAMESPACE_YM

using nsMincov::MinCov;
using nsUdGraph::UdGraph;
using nsUdGraph::UdEdge;

END_NAMESPACE_YM

#endif // YM_COMBOPT_H
