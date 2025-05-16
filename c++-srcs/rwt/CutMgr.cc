
/// @file CutMgr.cc
/// @brief CutMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CutMgr.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

// @brief コンストラクタ
CutMgr::CutMgr(
  SizeType cut_size
) : mCutSize{cut_size}
{
}

// @brief デストラクタ
CutMgr::~CutMgr()
{
  for ( auto p: mCutHash ) {
    auto& cut_list = p.second;
    remove_cuts(cut_list);
  }
}

/// @brief enum_cuts() の下請け関数
const std::vector<Cut*>&
CutMgr::_enum_cuts(
  const AigNode* node ///< [in] 根のノード
)
{
  if ( mCutHash.count(node->id()) == 0 ) {
    std::vector<Cut*> cut_list;
    { // 自身を葉とするカットを作る．
      auto cut = new Cut({node}, {});
      cut_list.push_back(cut);
    }
    if ( node->is_and() ) {
      auto node0 = node->fanin0_node();
      auto node1 = node->fanin1_node();
      auto& cuts0 = _enum_cuts(node0);
      auto& cuts1 = _enum_cuts(node1);
      cut_list.reserve(cuts0.size() * cuts1.size() + 1);
      for ( auto cut0: cuts0 ) {
	for ( auto cut1: cuts1 ) {
	  auto cut = _merge_cuts(node, cut0, cut1);
	  if ( cut != nullptr ) {
	    cut_list.push_back(cut);
	  }
	}
      }
    }
    mCutHash.emplace(node->id(), cut_list);
  }
  return mCutHash.at(node->id());
}

// @brief 2つのカットをマージする．
Cut*
CutMgr::_merge_cuts(
  const AigNode* root,
  const Cut* cut0,
  const Cut* cut1
)
{
  // 結果の葉のリスト
  std::vector<const AigNode*> leaf_list;
  // leaf_list 用のハッシュ
  std::unordered_set<SizeType> leaf_hash;
  // 結果のノードリスト
  std::vector<const AigNode*> node_list;
  node_list.push_back(root);
  // cut0 のノードリストに対応したハッシュ
  std::unordered_set<SizeType> set0;
  for ( auto node: cut0->node_list() ) {
    set0.emplace(node->id());
    node_list.push_back(node);
  }
  // cut1 のノードリストに対応したハッシュ
  std::unordered_set<SizeType> set1;
  for ( auto node: cut1->node_list() ) {
    set1.emplace(node->id());
    if ( set0.count(node->id()) == 0 ) {
      node_list.push_back(node);
    }
  }
  // cut0 の葉のノードが set1 に含まれていたら不正
  for ( auto node: cut0->leaf_list() ) {
    if ( set1.count(node->id()) > 0 ) {
      return nullptr;
    }
    leaf_list.push_back(node);
    leaf_hash.emplace(node->id());
  }
  // cut1 の葉のノードが set0 に含まれていたら不正
  for ( auto node: cut1->leaf_list() ) {
    if ( set0.count(node->id()) > 0 ) {
      return nullptr;
    }
    if ( leaf_hash.count(node->id()) == 0 ) {
      leaf_list.push_back(node);
    }
  }
  if ( leaf_list.size() > mCutSize ) {
    // サイズが大きければ不正
    return nullptr;
  }
  return new Cut(leaf_list, node_list);
}

END_NAMESPACE_YM_AIG
