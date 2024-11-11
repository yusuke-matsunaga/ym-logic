#ifndef DDNODE_H
#define DDNODE_H

/// @file DdNode.h
/// @brief DdNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class DdNode DdNode.h "ym/DdNode.h"
/// @brief BDD/ZDD のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DdNode
{
  friend class DdNodeMgr;
  friend class DdNodeTable;
public:

  /// @brief コンストラクタ
  DdNode(
    SizeType index,
    DdEdge edge0,
    DdEdge edge1
  ) : mIndex{index},
      mEdge0{edge0},
      mEdge1{edge1}
  {
  }

  /// @brief デストラクタ
  ~DdNode() = default;


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
  DdEdge
  edge0() const
  {
    return mEdge0;
  }

  /// @brief 1枝を返す．
  DdEdge
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

  /// @brief ハッシュ関数
  SizeType
  hash() const
  {
    SizeType v{0};
    v += mIndex;
    v += mEdge0.hash() * 13;
    v += mEdge1.hash() * 17;
    return v;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  SizeType mIndex;

  // 0枝
  DdEdge mEdge0;

  // 1枝
  DdEdge mEdge1;

  // 参照回数
  mutable
  SizeType mRefCount{0};

  // ノードテーブル用のリンク
  mutable
  DdNode* mLink{nullptr};

};

/// @brief 等価比較演算
inline
bool
operator==(
  const DdNode& key1,
  const DdNode& key2
)
{
  return key1.index() == key2.index() &&
    key1.edge0() == key2.edge0() &&
    key1.edge1() == key2.edge1();
}

/// @brief 非等価比較演算
inline
bool
operator!=(
  const DdNode& key1,
  const DdNode& key2
)
{
  return !operator==(key1, key2);
}

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

/// @brief ハッシュ用の関数オブジェクト
template<>
struct hash<nsYm::nsDd::DdNode>
{
  SizeType
  operator()(
    const nsYm::nsDd::DdNode& key
  ) const
  {
    return key.hash();
  }
};

END_NAMESPACE_STD

#endif // DDNODE_H
