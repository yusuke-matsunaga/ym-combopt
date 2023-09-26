#ifndef PYMINCOV_H
#define PYMINCOV_H

/// @file PyMinCov.h
/// @brief PyMinCov のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/MinCov.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyMinCov PyMinCov.h "PyMinCov.h"
/// @brief Python 用の MinCov 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyMinCov
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief MinCov を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const MinCov& val ///< [in] 値
  );

  /// @brief PyObject が MinCov タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief MinCov を表す PyObject から MinCov を取り出す．
  /// @return MinCov を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  MinCov
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief MinCov を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYMINCOV_H
