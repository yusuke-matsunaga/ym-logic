#ifndef REPLACEDICT_H
#define REPLACEDICT_H

/// @file ReplaceDict.h
/// @brief ReplaceDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class ReplaceDict ReplaceDict.h "ReplaceDict.h"
/// @brief 置き換え結果を記録する辞書
///
/// 意味的には AigEdge -> AigEdge だが反転を考慮するために内部では
/// ノード番号 -> AigEdge の辞書を持つ．
//////////////////////////////////////////////////////////////////////
class ReplaceDict
{
public:

  /// @brief コンストラクタ
  ReplaceDict() = default;

  /// @brief デストラクタ
  ~ReplaceDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録する．
  void
  add(
    AigNode* old_node, ///< [in] 置き換え対象のノード
    AigEdge  new_edge  ///< [in] 置き換え結果の枝
  )
  {
    mDict.emplace(old_node->id(), new_edge);
  }

  /// @brief 置き換え結果を得る．
  ///
  /// 登録されていなければ元の枝をそのまま返す．
  AigEdge
  get(
    AigEdge edge ///< [in] 対象の枝
  ) const
  {
    auto node = edge.node();
    if ( mDict.count(node->id()) > 0 ) {
      auto new_edge = mDict.at(node->id());
      if ( edge.inv() ) {
	new_edge = ~new_edge;
      }
      return new_edge;
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

#endif // REPLACEDICT_H
