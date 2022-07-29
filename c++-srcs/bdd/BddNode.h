#ifndef BDDNODE_H
#define BDDNODE_H

/// @file BddNode.h
/// @brief BddNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddNode BddNode.h "ym/BddNode.h"
/// @brief BDD のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BddNode
{
  friend class BddMgrImpl;

private:

  /// @brief コンストラクタ
  BddNode(
    SizeType index,
    BddEdge edge0,
    BddEdge edge1
  ) : mIndex{index},
      mEdge0{edge0},
      mEdge1{edge1}
  {
  }

  /// @brief デストラクタ
  ~BddNode() = default;


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
  BddEdge
  edge0() const
  {
    return mEdge0;
  }

  /// @brief 1枝を返す．
  BddEdge
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
  BddEdge mEdge0;

  // 1枝
  BddEdge mEdge1;

  // ノードテーブル用のリンク
  BddNode* mLink{nullptr};

};

/// @brief 先頭のノードを分解する．
inline
SizeType
decomp(
  BddEdge left,
  BddEdge right,
  BddEdge& left0,
  BddEdge& left1,
  BddEdge& right0,
  BddEdge& right1
)
{
  auto l_node = left.node();
  auto l_inv = left.inv();
  auto l_index = l_node->index();
  auto r_node = right.node();
  auto r_inv = right.inv();
  auto r_index = r_node->index();
  auto top = std::min(l_index, r_index);
  if ( l_index == top ) {
    left0 = l_node->edge0() ^ l_inv;
    left1 = l_node->edge1() ^ l_inv;
  }
  else {
    left0 = left;
    left1 = left;
  }
  if ( r_index == top ) {
    right0 = r_node->edge0() ^ r_inv;
    right1 = r_node->edge1() ^ r_inv;
  }
  else {
    right0 = right;
    right1 = right;
  }
  return top;
}

END_NAMESPACE_YM_BDD

#endif // BDDNODE_H
