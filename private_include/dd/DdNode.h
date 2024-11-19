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
///
/// 意味的には以下の情報を持つ．
/// - レベル
/// - 0-枝
/// - 1-枝
///
/// さらに以下の情報も管理用に持つ．
/// - 参照回数
/// - ハッシュ用にリンクポインタ
//////////////////////////////////////////////////////////////////////
class DdNode
{
  friend class DdNodeMgr;
  friend class DdNodeTable;
public:

  /// @brief コンストラクタ
  DdNode(
    SizeType level,
    DdEdge edge0,
    DdEdge edge1
  ) : mLevel{level},
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

  /// @brief レベルを返す．
  SizeType
  level() const
  {
    return mLevel;
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

  /// @brief レベルを変更する．
  void
  chg_level(
    SizeType new_level
  )
  {
    mLevel = new_level;
  }

  /// @brief 枝を変更する．
  void
  chg_edges(
    DdEdge new_edge0,
    DdEdge new_edge1
  )
  {
    mEdge0 = new_edge0;
    mEdge1 = new_edge1;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // レベル
  SizeType mLevel;

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
  return key1.level() == key2.level() &&
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

#endif // DDNODE_H
