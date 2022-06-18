#ifndef MISNODE_H
#define MISNODE_H

/// @file MisNode.h
/// @brief MisNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MisNode MisNode.h "MisNode.h"
/// @brief LbMIS で用いられるクラス
//////////////////////////////////////////////////////////////////////
class MisNode
{
  friend class MisNodeHeap;

public:

  /// @brief コンストラクタ
  MisNode() = default;

  /// @brief デストラクタ
  ~MisNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  void
  set_row_pos(
    SizeType row_pos ///< [in] 行番号
  )
  {
    mRowPos = row_pos;
  }

  /// @brief 隣接ノードの情報を設定する．
  void
  set_adj_link(
    vector<MisNode*>&& adj_link
  )
  {
    mAdjLink.swap(adj_link);
    mNum = mAdjLink.size();
  }

  /// @brief 行番号を返す．
  SizeType
  row_pos() const
  {
    return mRowPos;
  }

  /// @brief 削除済みフラグを返す．
  bool
  deleted() const
  {
    // 実はヒープインデックスが無効か
    // どうかで判断する．
    return mHeapIdx == 0;
  }

  /// @brief 隣接するノード数を返す．
  SizeType
  adj_size() const
  {
    return mAdjLink.size();
  }

  /// @brief 隣接するノードを返す．
  MisNode*
  adj_node(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < adj_size() )
  ) const
  {
    return mAdjLink[pos];
  }

  /// @brief 有効な隣接ノード数を返す．
  SizeType
  adj_num() const
  {
    return mNum;
  }

  /// @brief adj_num を１減らす
  void
  dec_adj_num()
  {
    -- mNum;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  SizeType mRowPos{0};

  // 隣接するノードのポインタ配列
  vector<MisNode*> mAdjLink;

  // mAdjLink 中の有効な要素数
  SizeType mNum{0};

  // ヒープ上のインデックス
  SizeType mHeapIdx{0};

};

END_NAMESPACE_YM_MINCOV

#endif // MISNODE_H
