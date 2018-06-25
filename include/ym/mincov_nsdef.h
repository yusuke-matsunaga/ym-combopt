#ifndef YM_MINCOV_NSDEF_H
#define YM_MINCOV_NSDEF_H

/// @file ym/mincov_nsdef.h
/// @brief Mincov Solver 用の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2018 Yusuke Matsunaga
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

/// @namespace nsYm::nsMincov
/// @brief MINCOV ソルバ関係のクラスが属する名前空間

BEGIN_NAMESPACE_YM_MINCOV

class MinCov;
class LbCalc;
class Selector;
class McCell;
class McHead;
class McHeadList;
class McRowList;
class McColList;
class McBlock;
class McMatrix;

END_NAMESPACE_YM_MINCOV

BEGIN_NAMESPACE_YM

using nsMincov::MinCov;

END_NAMESPACE_YM

#endif // YM_MINCOV_NSDEF_H
