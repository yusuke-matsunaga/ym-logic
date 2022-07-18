#ifndef ITEOP_H
#define ITEOP_H

/// @file IteOp.h
/// @brief IteOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"
#include "BddEdge.h"
#include "ApplyKey.h"


BEGIN_NAMESPACE_YM_BDD

// ハッシュ表のキー
struct IteKey
{
  BddEdge mEdge0;
  BddEdge mEdge1;
  BddEdge mEdge2;
};

inline
bool
operator==(
  const IteKey& key1,
  const IteKey& key2
)
{
  return key1.mEdge0 == key2.mEdge0 && key1.mEdge1 == key2.mEdge1 && key1.mEdge2 == key2.mEdge2;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_STD

template<>
struct hash<nsYm::nsBdd::IteKey>
{
  SizeType
  operator()(
    const nsYm::nsBdd::IteKey& key
  ) const
  {
    return key.mEdge0.hash() + key.mEdge1.hash() * 13 + key.mEdge2.hash() * 17;
  }
};

END_NAMESPACE_STD

BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class IteOp IteOp.h "IteOp.h"
/// @brief ITE演算をおこなうクラス
//////////////////////////////////////////////////////////////////////
class IteOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  IteOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~IteOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ITE 演算を行う．
  BddEdge
  ite_step(
    BddEdge e0,
    BddEdge e1,
    BddEdge e2
  );

  /// @brief AND演算を行う．
  BddEdge
  and_step(
    BddEdge left,
    BddEdge right
  );

  /// @brief OR演算を行う．
  BddEdge
  or_step(
    BddEdge left,
    BddEdge right
  )
  {
    // De Morgan の法則
    return ~and_step(~left, ~right);
  }

  /// @brief XOR演算を行う．
  BddEdge
  xor_step(
    BddEdge left,
    BddEdge right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<IteKey, BddEdge> mIteTable;

  // AND用のテーブル
  unordered_map<ApplyKey, BddEdge> mAndTable;

  // XOR用のテーブル
  unordered_map<ApplyKey, BddEdge> mXorTable;

};

END_NAMESPACE_YM_BDD

#endif // ITEOP_H
