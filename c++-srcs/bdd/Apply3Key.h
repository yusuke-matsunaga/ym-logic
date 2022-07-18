#ifndef APPLY3KEY_H
#define APPLY3KEY_H

/// @file Apply3Key.h
/// @brief Apply3Key のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

/// @brief 三項演算用のハッシュ表のキー
struct Apply3Key
{
  BddEdge mEdge0;
  BddEdge mEdge1;
  BddEdge mEdge2;
};

/// @brief 等価比較演算
inline
bool
operator==(
  const Apply3Key& key1,
  const Apply3Key& key2
)
{
  return key1.mEdge0 == key2.mEdge0 && key1.mEdge1 == key2.mEdge1 && key1.mEdge2 == key2.mEdge2;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_STD

/// @brief ハッシュ用の関数オブジェクト
template<>
struct hash<nsYm::nsBdd::Apply3Key>
{
  SizeType
  operator()(
    const nsYm::nsBdd::Apply3Key& key
  ) const
  {
    return key.mEdge0.hash() + key.mEdge1.hash() * 13 + key.mEdge2.hash() * 17;
  }
};

END_NAMESPACE_STD

#endif // APPLY3KEY_H
