#ifndef PYM_YMCOMBOPT_H
#define PYM_YMCOMBOPT_H

/// @file ymcombopt.h
/// @brief ymcombopt の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymcombopt モジュールの初期化関数
extern "C" PyObject* PyInit_ymcombopt();

END_NAMESPACE_YM

#endif // PYM_YMCOMBOPT_H
