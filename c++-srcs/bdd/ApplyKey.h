#ifndef APPLYKEY_H
#define APPLYKEY_H

/// @file ApplyOp.h
/// @brief ApplyOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

// ハッシュ表のキー
struct ApplyKey
{
  BddEdge mLeft;   // オペランド1
  BddEdge mRight;  // オペランド2
};

inline
bool
operator==(
  const ApplyKey& key1,
  const ApplyKey& key2
)
{
  return key1.mLeft == key2.mLeft && key1.mRight == key2.mRight;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_STD

template<>
struct hash<nsYm::nsBdd::ApplyKey>
{
  SizeType
  operator()(
    const nsYm::nsBdd::ApplyKey& key
  ) const
  {
    return key.mLeft.hash() + key.mRight.hash() * 13;
  }
};

END_NAMESPACE_STD

#endif // APPLYKEY_H
