#ifndef STRUCTTABLE_H
#define STRUCTTABLE_H

/// @file StructTable.h
/// @brief StructTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FraigNode.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class StructHash StructTable.h "StructTable.h"
/// @brief FraigNode の構造ハッシュ関数
//////////////////////////////////////////////////////////////////////
class StructHash
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief FraigNode のハッシュ関数
  SizeType
  operator()(
    const FraigNode* node ///< [in] 対象のノード
  ) const
  {
    auto h1 = node->fanin0_sig();
    auto h2 = node->fanin1_sig();
    return h1 + h2 * 13;
  }
};


//////////////////////////////////////////////////////////////////////
/// @class StructEq StructTable.h "StructTable.h"
/// @brief FraigNode の構造ハッシュ用等価比較関数
//////////////////////////////////////////////////////////////////////
class StructEq
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief FraigNode の構造比較関数
  bool
  operator()(
    const FraigNode* node1, ///< [in] ノード1
    const FraigNode* node2  ///< [in] ノード2
  ) const
  {
    auto h1_0 = node1->fanin0_sig();
    auto h1_1 = node1->fanin1_sig();
    auto h2_0 = node2->fanin0_sig();
    auto h2_1 = node2->fanin1_sig();
    return h1_0 == h2_0 && h1_1 == h2_1;
  }
};

/// @brief FraigNode の構造ハッシュテーブル
using StructTable = std::unordered_set<FraigNode*, StructHash, StructEq>;

END_NAMESPACE_YM_FRAIG

#endif // STRUCTTABLE_H
