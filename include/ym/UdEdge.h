#ifndef YM_UDEDGE_H
#define YM_UDEDGE_H

/// @file ym/UdEdge.h
/// @brief UdEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#error "obsolete"

#include "ym/udgraph_nsdef.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class UdEdge UdEdge.h "UdEdge.h"
/// @brief UdGraph の枝を表すクラス
///
/// 向きがないので id1 <= id2 と仮定する．
//////////////////////////////////////////////////////////////////////
class UdEdge
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 内容は未定
  UdEdge() = default;

  /// @brief 内容を指定したコンストラクタ
  ///
  /// id1 > id2 の場合は取り替える．
  UdEdge(
    SizeType id1, ///< [in] ノード1の番号
    SizeType id2  ///< [in] ノード2の番号
  )
  {
    if ( id1 <= id2 ) {
      mId1 = id1;
      mId2 = id2;
    }
    else {
      mId1 = id2;
      mId2 = id1;
    }
  }

  /// @brief デストラクタ
  ~UdEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 小さい方のノード番号を得る．
  SizeType
  id1() const
  {
    return mId1;
  }

  /// @brief 大きい方のノード番号を得る．
  SizeType
  id2() const
  {
    return mId2;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号1
  SizeType mId1{-1};

  // ノード番号2
  SizeType mId2{-1};

};

END_NAMESPACE_YM_UDGRAPH

#endif // YM_UDEDGE_H
