
/// @file replace.cc
/// @brief AigMgrImpl::replace() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"
#include "ReplaceMgr.h"
#include "ym/AigHandle.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス ReplaceMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReplaceMgr::ReplaceMgr(
  AigMgrImpl* mgr
) : mMgr{mgr}
{
  mMgr->_attach(this);
}

// @brief デストラクタ
ReplaceMgr::~ReplaceMgr()
{
  mMgr->_dettach(this);
}


//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief ReplaceMgr を登録する．
void
AigMgrImpl::_attach(
  ReplaceMgr* mgr
)
{
  if ( mRepMgr != nullptr ) {
    throw std::logic_error{"another ReplaceMgr has been already attached"};
  }
  mRepMgr = mgr;
  auto node_list = and_list();
  for ( auto node: node_list ) {
    auto node0 = node->fanin0_node();
    auto& fo0 = mRepMgr->fo_info(node0);
    fo0.fanin0_list.push_back(node);
    auto node1 = node->fanin1_node();
    auto& fo1 = mRepMgr->fo_info(node1);
    fo1.fanin1_list.push_back(node);
  }
  for ( auto handle: mHandleHash ) {
    auto edge = handle->_edge();
    if ( edge.is_and() ) {
      auto node = edge.node();
      auto& fo = mRepMgr->fo_info(node);
      fo.handle_list.push_back(handle);
    }
  }
}

// @brief ReplaceMgr の登録を解除する．
void
AigMgrImpl::_dettach(
  ReplaceMgr* mgr
)
{
  if ( mRepMgr != mgr ) {
    throw std::logic_error{"ReplaceMgr mismatch"};
  }
  mRepMgr = nullptr;
}

// @brief ノード(枝)の置き換えを行う．
void
AigMgrImpl::_replace(
  AigNode* old_node,
  AigEdge new_edge
)
{
  struct RepInfo {
    AigNode* old_node;
    AigEdge new_edge;
  };

  if ( mRepMgr == nullptr ) {
    throw std::logic_error{"ReplaceMgr is not attached"};
  }

  std::deque<RepInfo> queue;
  queue.push_back({old_node, new_edge});
  while ( !queue.empty() ) {
    auto& info = queue.front();
    auto old_node = info.old_node;
    auto new_edge = info.new_edge;
    queue.pop_front();

    auto& fo_info = mRepMgr->fo_info(old_node);
    for ( auto node: fo_info.fanin0_list ) {
      if ( node->ref_count() == 0 ) {
	continue;
      }
      if ( node->fanin0_node() != old_node ) {
	throw std::logic_error{"FoInfo is broken"};
      }
      auto edge0 = new_edge * node->fanin0_inv();
      auto edge1 = node->fanin1();
      AigEdge new_edge1;
      if ( _special_case(edge0, edge1, new_edge1) ) {
	queue.push_back({node, new_edge1});
      }
      else {
	_change_fanin(node, edge0, edge1);
	auto new_node = new_edge.node();
	auto& new_fo_info = mRepMgr->fo_info(new_node);
	new_fo_info.fanin0_list.push_back(node);
      }
    }
    for ( auto node: fo_info.fanin1_list ) {
      if ( node->ref_count() == 0 ) {
	continue;
      }
      if ( node->fanin1_node() != old_node ) {
	throw std::logic_error{"FoInfo is broken"};
      }
      auto edge0 = node->fanin0();
      auto edge1 = new_edge * node->fanin1_inv();
      AigEdge new_edge1;
      if ( _special_case(edge0, edge1, new_edge1) ) {
	queue.push_back({node, new_edge1});
      }
      else {
	_change_fanin(node, edge0, edge1);
	auto new_node = new_edge.node();
	auto& new_fo_info = mRepMgr->fo_info(new_node);
	new_fo_info.fanin1_list.push_back(node);
      }
    }
    for ( auto handle: fo_info.handle_list ) {
      if ( handle->_edge().node() != old_node ) {
	throw std::logic_error{"FoInfo is broken"};
      }
      auto edge = new_edge * handle->inv();
      handle->_set_edge(edge);
      if ( !new_edge.is_const() ) {
	auto new_node = new_edge.node();
	auto& new_fo_info = mRepMgr->fo_info(new_node);
	new_fo_info.handle_list.push_back(handle);
      }
    }
  }
}

END_NAMESPACE_YM_AIG
