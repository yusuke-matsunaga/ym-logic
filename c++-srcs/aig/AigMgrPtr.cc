
/// @file AigMgrPtr.cc
/// @brief AigMgrPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgrPtr.h"
#include "ym/AigHandle.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief 値を指定したコンストラクタ
AigMgrPtr::AigMgrPtr(
  AigMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief コピーコンストラクタ
AigMgrPtr::AigMgrPtr(
  const AigMgrPtr& src
) : mPtr{src.mPtr}
{
}

// @brief デストラクタ
AigMgrPtr::~AigMgrPtr()
{
}

// @brief AigHandle を AigEdge に変換する．
AigEdge
AigMgrPtr::handle_to_edge(
  AigHandle handle
) const
{
  _check_mgr(handle);
  return handle._edge();
}

// AigHandle のリストを AigEdge のリストに変換する．
vector<AigEdge>
AigMgrPtr::hlist_to_elist(
  const vector<AigHandle>& handle_list
) const
{
  auto n = handle_list.size();
  vector<AigEdge> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = handle_list[i];
    auto edge = handle_to_edge(h);
    edge_list[i] = edge;
  }
  return edge_list;
}

// AigHandle のリストを AigEdge のリストに変換する．
AigMgrPtr
AigMgrPtr::hlist_to_elist(
  const vector<AigHandle>& handle_list,
  vector<AigEdge>& edge_list
)
{
  edge_list.clear();
  auto n = handle_list.size();
  if ( n == 0 ) {
    return AigMgrPtr{};
  }
  edge_list.reserve(n);
  auto mgr = handle_list.front().mgr_ptr();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = handle_list[i];
    auto edge = mgr.handle_to_edge(h);
    edge_list.push_back(edge);
  }
  return mgr;
}

// @brief AigEdge を AigHandle に変換する．
AigHandle
AigMgrPtr::edge_to_handle(
  AigEdge edge
) const
{
  return AigHandle{*this, edge};
}

// @brief AigEdge のリストを AigHandle のリストに変換する．
vector<AigHandle>
AigMgrPtr::elist_to_hlist(
  const vector<AigEdge>& edge_list ///< [in] 枝のリスト
) const
{
  auto n = edge_list.size();
  vector<AigHandle> handle_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto edge = edge_list[i];
    handle_list[i] = edge_to_handle(edge);
  }
  return handle_list;
}

// @brief ハンドルが同じマネージャに属しているかチェックする．
void
AigMgrPtr::_check_mgr(
  const AigHandle& handle
) const
{
  if ( !handle.is_const() && handle.mMgr != *this ) {
    throw std::invalid_argument{"AigMgr mismatch"};
  }
}

END_NAMESPACE_YM_AIG
