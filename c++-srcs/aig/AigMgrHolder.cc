
/// @file AigMgrHolder.cc
/// @brief AigMgrHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgrHolder.h"
#include "ym/AigHandle.h"
#include "aig/AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrHolder
//////////////////////////////////////////////////////////////////////

// @brief 値を指定したコンストラクタ
AigMgrHolder::AigMgrHolder(
  AigMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief デストラクタ
AigMgrHolder::~AigMgrHolder()
{
}

// @brief AigHandle を AigEdge に変換する．
AigEdge
AigMgrHolder::handle_to_edge(
  AigHandle handle
) const
{
  check_mgr(handle);
  return handle._edge();
}

// AigHandle のリストを AigEdge のリストに変換する．
vector<AigEdge>
AigMgrHolder::hlist_to_elist(
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
AigMgrImpl*
AigMgrHolder::hlist_to_elist(
  const vector<AigHandle>& handle_list,
  vector<AigEdge>& edge_list
)
{
  edge_list.clear();
  auto n = handle_list.size();
  if ( n == 0 ) {
    return nullptr;
  }
  edge_list.reserve(n);
  auto holder = handle_list.front().mgr_holder();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = handle_list[i];
    auto edge = holder.handle_to_edge(h);
    edge_list.push_back(edge);
  }
  return holder.get();
}

// @brief AigEdge を AigHandle に変換する．
AigHandle
AigMgrHolder::edge_to_handle(
  AigEdge edge
) const
{
  return AigHandle(*this, edge);
}

// @brief AigEdge のリストを AigHandle のリストに変換する．
vector<AigHandle>
AigMgrHolder::elist_to_hlist(
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

END_NAMESPACE_YM_AIG
