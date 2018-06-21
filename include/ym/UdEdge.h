#ifndef YM_UDEDGE_H
#define YM_UDEDGE_H

/// @file ym/UdEdge.h
/// @brief UdEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


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
  UdEdge();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] id1, id2 枝の両端のノード番号
  ///
  /// id1 > id2 の場合は取り替える．
  UdEdge(int id1,
	 int id2);

  /// @brief デストラクタ
  ~UdEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 小さい方のノード番号を得る．
  int
  id1() const;

  /// @brief 大きい方のノード番号を得る．
  int
  id2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号1
  int mId1;

  // ノード番号2
  int mId2;

};


//////////////////////////////////////////////////////////////////////
// UdEdge のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
//
// 内容は未定
inline
UdEdge::UdEdge()
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] id1, id2 枝の両端のノード番号
//
// id1 > id2 の場合は取り替える．
inline
UdEdge::UdEdge(int id1,
	       int id2)
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

// @brief デストラクタ
inline
UdEdge::~UdEdge()
{
}

// @brief 小さい方のノード番号を得る．
inline
int
UdEdge::id1() const
{
  return mId1;
}

// @brief 大きい方のノード番号を得る．
inline
int
UdEdge::id2() const
{
  return mId2;
}

END_NAMESPACE_YM_UDGRAPH

#endif // YM_UDEDGE_H
