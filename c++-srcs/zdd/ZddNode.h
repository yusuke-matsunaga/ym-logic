#ifndef ZDDNODE_H
#define ZDDNODE_H

/// @file ZddNode.h
/// @brief ZddNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ZddEdge.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddNode ZddNode.h "ym/ZddNode.h"
/// @brief ZDD のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class ZddNode
{
  friend class ZddMgrImpl;

private:

  /// @brief コンストラクタ
  ZddNode(
    SizeType index,
    ZddEdge edge0,
    ZddEdge edge1
  ) : mIndex{index},
      mEdge0{edge0},
      mEdge1{edge1}
  {
  }

  /// @brief デストラクタ
  ~ZddNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスを返す．
  SizeType
  index() const
  {
    return mIndex;
  }

  /// @brief 0枝を返す．
  ZddEdge
  edge0() const
  {
    return mEdge0;
  }

  /// @brief 1枝を返す．
  ZddEdge
  edge1() const
  {
    return mEdge1;
  }

  /// @brief 参照回数を返す．
  SizeType
  ref_count() const
  {
    return mRefCount;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  SizeType mRefCount{0};

  // インデックス
  SizeType mIndex;

  // 0枝
  ZddEdge mEdge0;

  // 1枝
  ZddEdge mEdge1;

  // ノードテーブル用のリンク
  ZddNode* mLink{nullptr};

};

/// @brief 先頭のノードを分解する．
inline
SizeType
decomp(
  ZddEdge left,
  ZddEdge right,
  ZddEdge& left0,
  ZddEdge& left1,
  ZddEdge& right0,
  ZddEdge& right1
)
{
  SizeType l_index;
  ZddNode* l_node;
  if ( left.is_const() ) {
    l_index = BAD_VARID;
  }
  else {
    l_node = left.node();
    l_index = l_node->index();
  }
  SizeType r_index;
  ZddNode* r_node;
  if ( right.is_const() ) {
    r_index = BAD_VARID;
  }
  else {
    r_node = right.node();
    r_index = r_node->index();
  }
  auto top = std::min(l_index, r_index);
  if ( l_index == top ) {
    left0 = l_node->edge0();
    left1 = l_node->edge1();
  }
  else {
    left0 = left;
    left1 = ZddEdge::zero();
  }
  if ( r_index == top ) {
    right0 = r_node->edge0();
    right1 = r_node->edge1();
  }
  else {
    right0 = right;
    right1 = ZddEdge::zero();
  }
  return top;
}

END_NAMESPACE_YM_ZDD

#endif // ZDDNODE_H
