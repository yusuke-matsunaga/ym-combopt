#ifndef YM_MINCOV_NSDEF_H
#define YM_MINCOV_NSDEF_H

/// @file mincov_nsdef.h
/// @brief Mincov Solver 用の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup MincovGroup Mincov Solver
///
/// 最小被覆問題を解くためのライブラリ

#include "ym/combopt.h"


/// @namespace nsYm::nsMincov
/// @brief MINCOV ソルバ関係のクラスが属する名前空間

BEGIN_NAMESPACE_YM_MINCOV

class LbCalc;
class Selector;
class McRowList;
class McColList;
class McColComp;
class McMatrix;
class McHeadList;

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_NSDEF_H
