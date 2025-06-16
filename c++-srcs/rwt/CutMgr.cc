
/// @file CutMgr.cc
/// @brief CutMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CutMgr.h"
#include "aig/AigNode.h"

#define DEBUG_CUTMGR 1
#define VERIFY_CUTMGR 1


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス CutMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CutMgr::CutMgr(
  AigMgrImpl* mgr,
  SizeType cut_size
) : EventMgr(mgr),
    mCutSize{cut_size}
{
}

// @brief デストラクタ
CutMgr::~CutMgr()
{
  for ( auto p: mCutDict ) {
    auto& cut_list = p.second;
    _remove_cuts(cut_list);
  }
}

BEGIN_NONAMESPACE

std::vector<AigNode*>
make_footprint(
  const std::vector<Cut*>& cut_list
)
{
  std::vector<AigNode*> fp_list;
  std::unordered_set<SizeType> fp_hash;
  for ( auto cut: cut_list ) {
    for ( auto node: cut->leaf_list() ) {
      if ( fp_hash.count(node->id()) == 0 ) {
	fp_hash.emplace(node->id());
	fp_list.push_back(node);
      }
    }
    for ( auto node: cut->node_list() ) {
      if ( fp_hash.count(node->id()) == 0 ) {
	fp_hash.emplace(node->id());
	fp_list.push_back(node);
      }
    }
  }
  return fp_list;
}

END_NONAMESPACE

/// @brief enum_cuts() の下請け関数
const std::vector<Cut*>&
CutMgr::_enum_cuts(
  AigNode* node
)
{
  if ( mCutDict.count(node->id()) == 0 ) {
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
      auto fp_mark = _merge_footprint(node0, node1);
      for ( auto cut0: cuts0 ) {
	for ( auto cut1: cuts1 ) {
	  auto cut = _merge_cuts(node, cut0, cut1, fp_mark);
	  if ( cut != nullptr ) {
	    cut_list.push_back(cut);
	  }
	}
      }
    }
    mCutDict.emplace(node->id(), cut_list);
    auto fp_list = make_footprint(cut_list);
    mFootPrintDict.emplace(node->id(), fp_list);
  }
  return mCutDict.at(node->id());
}

BEGIN_NONAMESPACE

bool
check_dfs(
  AigNode* node,
  const std::unordered_set<SizeType>& fp_mark,
  const std::unordered_set<SizeType>& leaf_mark,
  std::unordered_set<SizeType>& mark
)
{
  if ( fp_mark.count(node->id()) == 0 ) {
    return true;
  }
  if ( mark.count(node->id()) > 0 ) {
    return true;
  }
  mark.emplace(node->id());
  if ( node->is_and() ) {
    auto node0 = node->fanin0_node();
    if ( leaf_mark.count(node0->id()) ) {
      return false;
    }
    if ( !check_dfs(node0, fp_mark, leaf_mark, mark) ) {
      return false;
    }
    auto node1 = node->fanin1_node();
    if ( leaf_mark.count(node1->id()) ) {
      return false;
    }
    if ( !check_dfs(node1, fp_mark, leaf_mark, mark) ) {
      return false;
    }
  }
  return true;
}

bool
check_leaf_list(
  const std::vector<AigNode*>& leaf_list,
  const std::unordered_set<SizeType>& fp_mark
)
{
  std::unordered_set<SizeType> leaf_mark;
  for ( auto node: leaf_list ) {
    leaf_mark.emplace(node->id());
  }
  std::unordered_set<SizeType> mark;
  for ( auto node: leaf_list ) {
    if ( !check_dfs(node, fp_mark, leaf_mark, mark) ) {
      return false;
    }
  }
  return true;
}

END_NONAMESPACE

// @brief 2つのカットをマージする．
Cut*
CutMgr::_merge_cuts(
  AigNode* root,
  const Cut* cut0,
  const Cut* cut1,
  const std::unordered_set<SizeType>& fp_mark
)
{
  // 結果の葉のリスト
  std::vector<AigNode*> leaf_list;
  // leaf_list 用のハッシュ
  std::unordered_set<SizeType> leaf_hash;
  // 結果のノードリスト
  std::vector<AigNode*> node_list;

  // cut0 のノードリストに対応したハッシュを作る．
  // 同時に node_list に追加する．
  std::unordered_set<SizeType> set0;
  for ( auto node: cut0->node_list() ) {
    set0.emplace(node->id());
    node_list.push_back(node);
  }

  // cut1 のノードリストに対応したハッシュを作る．
  // 同時に node_list に追加する．
  std::unordered_set<SizeType> set1;
  for ( auto node: cut1->node_list() ) {
    set1.emplace(node->id());
    if ( set0.count(node->id()) == 0 ) {
      node_list.push_back(node);
    }
  }

  // cut0 の葉のノードが set1 に含まれていたら不正
  // 適正なら leaf_list に追加する．
  for ( auto node: cut0->leaf_list() ) {
    if ( set1.count(node->id()) > 0 ) {
      return nullptr;
    }
    leaf_list.push_back(node);
    leaf_hash.emplace(node->id());
  }

  // cut1 の葉のノードが set0 に含まれていたら不正
  // 適正なら leaf_list に追加する．
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

  // leaf_list が正しいかチェックする．
  if ( !check_leaf_list(leaf_list, fp_mark) ) {
    return nullptr;
  }
  return new Cut(leaf_list, node_list);
}

// @brief フットプリントをマージする．
std::unordered_set<SizeType>
CutMgr::_merge_footprint(
  AigNode* node0,
  AigNode* node1
)
{
  std::unordered_set<SizeType> mark;
  for ( auto node: mFootPrintDict.at(node0->id()) ) {
    mark.emplace(node->id());
  }
  for ( auto node: mFootPrintDict.at(node1->id()) ) {
    if ( mark.count(node->id()) == 0 ) {
      mark.emplace(node->id());
    }
  }
  return mark;
}

// @brief ノードのファンインが変化したときに呼ばれる関数
void
CutMgr::change_proc(
  AigNode* node
)
{
  if ( mCutDict.count(node->id()) > 0 ) {
    auto& cut_list = mCutDict.at(node->id());
    _remove_cuts(cut_list);
    mCutDict.erase(node->id());
  }
}

// @brief ノードが削除されるときに呼ばれる関数
void
CutMgr::delete_proc(
  AigNode* node
)
{
}

// @brief カットの情報が正しいかチェックする．
void
CutMgr::_sanity_check() const
{
  for ( auto& p: mCutDict ) {
    auto& cut_list = p.second;
    for ( auto cut: cut_list ) {
      if ( cut->root()->ref_count() == 0 ) {
	continue;
      }
      // カットが正しいかチェックする．
      std::unordered_set<SizeType> mark;
      for ( auto node: cut->leaf_list() ) {
	mark.emplace(node->id());
      }
      for ( auto node: cut->node_list() ) {
	if ( node->ref_count() == 0 ) {
	  cout << "Node#" << node->id() << " is deleted" << endl;
	  cut->print(cout);
	  abort();
	}
	auto node0 = node->fanin0_node();
	if ( mark.count(node0->id()) == 0 ) {
	  cout << "Node#" << node0->id() << " is not contained" << endl;
	  cut->print(cout);
	  abort();
	}
	auto node1 = node->fanin1_node();
	if ( mark.count(node1->id()) == 0 ) {
	  cout << "Node#" << node1->id() << " is not contained" << endl;
	  cut->print(cout);
	  abort();
	}
	mark.emplace(node->id());
      }
    }
  }
}

END_NAMESPACE_YM_AIG
