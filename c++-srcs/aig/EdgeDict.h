#ifndef EDGEDICT_H
#define EDGEDICT_H

/// @file EdgeDict.h
/// @brief EdgeDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class EdgeDict EdgeDict.h "EdgeDict.h"
/// @brief ノード番号をキーにして AigEdge を格納する辞書
///
/// 実体は std::unordered_map<SizeType, AigEdge> だが
/// AigEdge をキーのように扱うことができる．
/// 反転した枝の結果も反転する．
//////////////////////////////////////////////////////////////////////
class EdgeDict
{
public:

  /// @brief コンストラクタ
  EdgeDict() = default;

  /// @brief デストラクタ
  ~EdgeDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録する．
  void
  put(
    const AigNode* node, ///< [in] キーのノード
    AigEdge edge         ///< [in] 結果の枝
  )
  {
    mDict.emplace(node->id(), edge);
  }

  /// @brief 登録されているか調べる．
  bool
  check(
    AigEdge key ///< [in] キーの枝
  ) const
  {
    auto node = key.node();
    return mDict.count(node->id()) > 0;
  }

  /// @brief 結果を得る．
  AigEdge
  get(
    AigEdge key ///< [in] キーの枝
  ) const
  {
    auto node = key.node();
    auto inv = key.inv();
    auto edge = mDict.at(node->id());
    if ( inv ) {
      edge = ~edge;
    }
    return edge;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 辞書の本体
  std::unordered_map<SizeType, AigEdge> mDict;

};

END_NAMESPACE_YM_AIG

#endif // EDGEDICT_H
