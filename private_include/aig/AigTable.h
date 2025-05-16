#ifndef AIGTABLE_H
#define AIGTABLE_H

/// @file AigTable.h
/// @brief AigTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigNodeHash AigTable.h "AigTable.h"
/// @brief AigNode のハッシュ関数
//////////////////////////////////////////////////////////////////////
class AigNodeHash
{
public:

  /// @brief AigNode のハッシュ関数
  SizeType
  operator()(
    const AigNode* node
  ) const
  {
    auto e0 = node->fanin0();
    auto e1 = node->fanin1();
    return e0.index() + e1.index() * 13;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class AigNodeEq AigTable.h "AigTable.h"
/// @brief AigNode の等価比較関数
//////////////////////////////////////////////////////////////////////
class AigNodeEq
{
public:

  /// @brief AigNode の等価比較関数
  SizeType
  operator()(
    const AigNode* a,
    const AigNode* b
  ) const
  {
    auto a_0 = a->fanin0();
    auto a_1 = a->fanin1();
    auto b_0 = b->fanin0();
    auto b_1 = b->fanin1();
    return a_0 == b_0 && a_1 == b_1;
  }

};

/// @brief AigNode の構造ハッシュテーブル
using AigTable = std::unordered_set<AigNode*, AigNodeHash, AigNodeEq>;

END_NAMESPACE_YM_AIG

#endif // AIGTABLE_H
