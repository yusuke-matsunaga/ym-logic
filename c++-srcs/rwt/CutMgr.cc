
/// @file CutMgr.cc
/// @brief CutMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CutMgr.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス CutMgr
//////////////////////////////////////////////////////////////////////

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
  AigNode* node
)
{
  if ( mCutHash.count(node->id()) == 0 ) {
    std::vector<Cut*> cut_list;
    { // 自身を葉とするカットを作る．
      auto cut = new Cut(node);
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
  AigNode* root,
  const Cut* cut0,
  const Cut* cut1
)
{
  // 結果の葉のリスト
  std::vector<AigNode*> leaf_list;
  // leaf_list 用のハッシュ
  std::unordered_set<SizeType> leaf_hash;
  // 結果のノードリスト
  std::vector<AigNode*> node_list;
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
  node_list.push_back(root);
  {
    // カットが正しいかチェックする．
    std::unordered_set<SizeType> mark;
    for ( auto node: leaf_list ) {
      mark.emplace(node->id());
    }
    for ( auto node: node_list ) {
      auto node0 = node->fanin0_node();
      if ( mark.count(node0->id()) == 0 ) {
	cout << "Node#" << node0->id() << " is not contained" << endl;
	cout << "root: " << root->id() << endl;
	cout << "cut0:" << endl;
	cut0->print(cout);
	cout << "cut1:" << endl;
	cut1->print(cout);
	abort();
      }
      auto node1 = node->fanin1_node();
      if ( mark.count(node1->id()) == 0 ) {
	cout << "Node#" << node1->id() << " is not contained" << endl;
	cout << "root: " << root->id() << endl;
	cout << "cut0:" << endl;
	cut0->print(cout);
	cout << "cut1:" << endl;
	cut1->print(cout);
	abort();
      }
      mark.emplace(node->id());
    }
  }
  return new Cut(root, leaf_list, node_list);
}

// @brief ノードの削除に伴ってカットを削除する．
void
CutMgr::erase_cuts(
  AigNode* node ///< [in] 対象のノード
)
{
  std::deque<AigNode*> queue;
  queue.push_back(node);
  while ( !queue.empty() ) {
    auto node = queue.front();
    queue.pop_front();
    if ( mCutHash.count(node->id()) > 0 ) {
      auto& cut_list = mCutHash.at(node->id());
      remove_cuts(cut_list);
      mCutHash.erase(node->id());
      for ( auto& fo_info: node->fanout_list() ) {
	if ( fo_info.type == 2 ) {
	  continue;
	}
	auto fo_node = fo_info.node;
	queue.push_back(fo_node);
      }
    }
  }
}

END_NAMESPACE_YM_AIG
