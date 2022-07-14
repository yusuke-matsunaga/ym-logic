#ifndef BDDNODE_H
#define BDDNODE_H

/// @file BddNode.h
/// @brief BddNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BddNode BddNode.h "ym/BddNode.h"
/// @brief BDD のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BddNode
{
  friend class BddMgrImpl;
  friend class BddNodeTable;

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
  ~BddNode();


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

  /// @brief 極性補正を行った0枝を返す．
  BddEdge
  edge0(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return mEdge0 * inv;
  }

  /// @brief 極性補正を行った1枝を返す．
  BddEdge
  edge1(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return mEdge1 * inv;
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

END_NAMESPACE_YM

#endif // BDDNODE_H
