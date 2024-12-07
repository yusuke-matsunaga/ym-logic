
/// @file AigMgr.cc
/// @brief AigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgr::AigMgr(
) : mMgr{new AigMgrImpl}
{
}

// @brief 内容を指定したコンストラクタ
AigMgr::AigMgr(
  const AigMgrPtr& ptr
) : mMgr{ptr}
{
}

// @brief デストラクタ
AigMgr::~AigMgr()
{
}

// @brief ノード数を返す．
SizeType
AigMgr::node_num() const
{
  return mMgr->node_num();
}

// @brief 入力ノード数を返す．
SizeType
AigMgr::input_num() const
{
  return mMgr->input_num();
}

// @brief 入力のハンドルを返す．
AigHandle
AigMgr::input(
  SizeType input_id
)
{
  auto edge = mMgr->input(input_id);
  return mMgr.edge_to_handle(edge);
}

// @brief ANDノード数を返す．
SizeType
AigMgr::and_num() const
{
  return mMgr->and_num();
}

// @brief 論理シミュレーションを行う．
vector<AigBitVect>
AigMgr::eval(
  const vector<AigBitVect>& input_vals,
  const vector<AigHandle>& output_list
) const
{
  auto oedge_list = mMgr.hlist_to_elist(output_list);
  return mMgr->eval(input_vals, oedge_list);
}


// @brief 外部入力ノードを作る．
AigHandle
AigMgr::make_input()
{
  return mMgr.edge_to_handle(mMgr->make_input());
}

// @brief 複数の入力の AND を表すハンドルを返す．
AigHandle
AigMgr::and_op(
  const vector<AigHandle>& fanin_list
)
{
  auto edge_list = mMgr.hlist_to_elist(fanin_list);
  return mMgr.edge_to_handle(mMgr->and_op(edge_list));
}

// @brief 複数の入力の OR を表すハンドルを返す．
AigHandle
AigMgr::or_op(
  const vector<AigHandle>& fanin_list
)
{
  auto edge_list = mMgr.hlist_to_elist(fanin_list);
  return mMgr.edge_to_handle(mMgr->or_op(edge_list));
}

// @brief 複数の入力の XOR を表すハンドルを返す．
AigHandle
AigMgr::xor_op(
  const vector<AigHandle>& fanin_list
)
{
  auto edge_list = mMgr.hlist_to_elist(fanin_list);
  return mMgr.edge_to_handle(mMgr->xor_op(edge_list));
}

// @brief Expr から変換する．
AigHandle
AigMgr::from_expr(
  const Expr& expr
)
{
  auto edge = mMgr->from_expr(expr);
  return mMgr.edge_to_handle(edge);
}

// @brief 複数の Expr から変換する．
vector<AigHandle>
AigMgr::from_expr_list(
  const vector<Expr>& expr_list
)
{
  auto edge_list = mMgr->from_expr_list(expr_list);
  return mMgr.elist_to_hlist(edge_list);
}

// @brief コファクター演算
vector<AigHandle>
AigMgr::cofactor(
  const vector<AigHandle>& cube,
  const vector<AigHandle>& handle_list
)
{
  auto cedge_list = mMgr.hlist_to_elist(cube);
  auto edge_list = mMgr.hlist_to_elist(handle_list);
  auto ans_list = mMgr->cofactor(cedge_list, edge_list);
  return mMgr.elist_to_hlist(ans_list);
}

#if 0
// @brief AigHandle を AigEdge に変換する．
AigEdge
AigMgr::handle_to_edge(
  AigHandle handle
) const
{
  _check_mgr(handle);
  return handle._edge();
}

// AigHandle のリストを AigEdge のリストに変換する．
vector<AigEdge>
AigMgr::hlist_to_elist(
  const vector<AigHandle>& handle_list
) const
{
  auto n = handle_list.size();
  vector<AigEdge> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = handle_list[i];
    edge_list[i] = handle_to_edge(h);
  }
  return edge_list;
}

// @brief AigEdge のリストを AigHandle のリストに変換する．
vector<AigHandle>
AigMgr::elist_to_hlist(
  const vector<AigEdge>& edge_list ///< [in] 枝のリスト
) const
{
  auto n = edge_list.size();
  vector<AigHandle> handle_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto edge = edge_list[i];
    handle_list[i] = AigHandle{mMgr, edge};
  }
  return handle_list;
}
#endif

END_NAMESPACE_YM_AIG
