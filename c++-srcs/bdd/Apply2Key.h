#ifndef APPLY2KEY_H
#define APPLY2KEY_H

/// @file Apply2Key.h
/// @brief Apply2Key のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

/// @brief 二項演算用のハッシュ表のキー
struct Apply2Key
{
  BddEdge mLeft;   ///< オペランド1
  BddEdge mRight;  ///< オペランド2
};

/// @brief 等価比較演算
inline
bool
operator==(
  const Apply2Key& key1,
  const Apply2Key& key2
)
{
  return key1.mLeft == key2.mLeft && key1.mRight == key2.mRight;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_STD

/// @brief ハッシュ用の関数オブジェクト
template<>
struct hash<nsYm::nsBdd::Apply2Key>
{
  SizeType
  operator()(
    const nsYm::nsBdd::Apply2Key& key
  ) const
  {
    return key.mLeft.hash() + key.mRight.hash() * 13;
  }
};

END_NAMESPACE_STD

#endif // APPLY2KEY_H
