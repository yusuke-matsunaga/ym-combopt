#ifndef MCLQNODE_H
#define MCLQNODE_H

/// @file MclqNode.h
/// @brief MclqNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"


BEGIN_NAMESPACE_YM_MAXCLIQUE

//////////////////////////////////////////////////////////////////////
/// @class MclqNode MclqNode.h "MclqNode.h"
/// @brief MclqHeap で用いられるクラス
//////////////////////////////////////////////////////////////////////
class MclqNode
{
public:

  /// @brief コンストラクタ
  MclqNode() = default;

  /// @brief デストラクタ
  ~MclqNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  void
  set(
    SizeType id ///< [in] ノード番号
  )
  {
    mId = id;
  }

  /// @brief 隣接ノードの情報を設定する．
  void
  set_adj_link(
    const vector<MclqNode*>& adj_link
  )
  {
    mAdjLink = adj_link;
    mNum = mAdjLink.size();
  }

  /// @brief ノード番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 隣接するノード数を返す．
  SizeType
  adj_size() const
  {
    return mAdjLink.size();
  }

  /// @brief 隣接するノードのリストを返す．
  const vector<MclqNode*>&
  adj_node_list() const
  {
    return mAdjLink;
  }

  /// @brief 有効な隣接ノード数を返す．
  SizeType
  adj_num() const
  {
    return mNum;
  }

  /// @brief adj_num を1減らす
  void
  dec_adj_num()
  {
    -- mNum;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  SizeType mId;

  // 隣接するノードのポインタ配列
  vector<MclqNode*> mAdjLink;

  // mAdjLink 中の有効な要素数
  SizeType mNum{0};

};

END_NAMESPACE_YM_MAXCLIQUE

#endif // MCLQNODE_H
