
/// @file AigMgr.cc
/// @brief AigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "aig/AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgr::AigMgr(
) : AigMgrHolder(new AigMgrImpl)
{
}

// @brief 内容を指定したコンストラクタ
AigMgr::AigMgr(
  const AigMgrHolder& holder
) : AigMgrHolder(holder)
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
  return get()->node_num();
}

// @brief 入力ノード数を返す．
SizeType
AigMgr::input_num() const
{
  return get()->input_num();
}

// @brief コピーを作る．
AigHandle
AigMgr::copy(
  AigHandle handle
)
{
  auto edge = handle_to_edge(handle);
  AigMgr new_mgr;
  auto new_edge = new_mgr.get()->copy(edge);
  return new_mgr.edge_to_handle(new_edge);
}

// @brief 複数のハンドルのコピーを作る．
std::vector<AigHandle>
AigMgr::copy(
  const std::vector<AigHandle>& handle_list
)
{
  auto edge_list = hlist_to_elist(handle_list);
  AigMgr new_mgr;
  auto new_edge_list = new_mgr.get()->copy(edge_list);
  return new_mgr.elist_to_hlist(new_edge_list);
}

// @brief ANDノード数を返す．
SizeType
AigMgr::and_num() const
{
  return get()->and_num();
}

// @brief 論理シミュレーションを行う．
std::vector<AigBitVect>
AigMgr::eval(
  const std::unordered_map<SizeType, AigBitVect>& ival_dict,
  const vector<AigHandle>& output_list
) const
{
  auto oedge_list = hlist_to_elist(output_list);
  return get()->eval(ival_dict, oedge_list);
}

// @brief 外部入力ノードを返す．
AigHandle
AigMgr::input(
  SizeType input_id
)
{
  auto node = get()->input_node(input_id);
  auto edge = AigEdge(node, false);
  return edge_to_handle(edge);
}

// @brief 複数の入力の AND を表すハンドルを返す．
AigHandle
AigMgr::and_op(
  const vector<AigHandle>& fanin_list
)
{
  auto edge_list = hlist_to_elist(fanin_list);
  return edge_to_handle(get()->and_op(edge_list));
}

// @brief 複数の入力の OR を表すハンドルを返す．
AigHandle
AigMgr::or_op(
  const vector<AigHandle>& fanin_list
)
{
  auto edge_list = hlist_to_elist(fanin_list);
  return edge_to_handle(get()->or_op(edge_list));
}

// @brief 複数の入力の XOR を表すハンドルを返す．
AigHandle
AigMgr::xor_op(
  const vector<AigHandle>& fanin_list
)
{
  auto edge_list = hlist_to_elist(fanin_list);
  return edge_to_handle(get()->xor_op(edge_list));
}

// @brief PrimType の演算を行う．
AigHandle
AigMgr::primitive_op(
  PrimType type,
  const vector<AigHandle>& fanin_list
)
{
  switch ( type ) {
  case PrimType::None: break;
  case PrimType::C0:   return AigHandle::zero();
  case PrimType::C1:   return AigHandle::one();
  case PrimType::Buff: return fanin_list[0];
  case PrimType::Not:  return ~fanin_list[0];
  case PrimType::And:  return and_op(fanin_list);
  case PrimType::Nand: return ~and_op(fanin_list);
  case PrimType::Or:   return or_op(fanin_list);
  case PrimType::Nor:  return ~or_op(fanin_list);
  case PrimType::Xor:  return xor_op(fanin_list);
  case PrimType::Xnor: return ~xor_op(fanin_list);
  }
  std::ostringstream buf;
  buf << type << ": Wrong primitive type";
  throw std::invalid_argument{buf.str()};
  return AigHandle::zero();
}

// @brief Expr から変換する．
AigHandle
AigMgr::from_expr(
  const Expr& expr
)
{
  auto edge = get()->from_expr(expr);
  return edge_to_handle(edge);
}

// @brief Expr から変換する．
AigHandle
AigMgr::from_expr(
  const Expr& expr,
  const std::vector<AigHandle>& input_list
)
{
  std::vector<AigEdge> input_edge_list;
  hlist_to_elist(input_list, input_edge_list);
  auto edge = get()->from_expr(expr, input_edge_list);
  return edge_to_handle(edge);
}

// @brief 複数の Expr から変換する．
vector<AigHandle>
AigMgr::from_expr_list(
  const vector<Expr>& expr_list
)
{
  auto edge_list = get()->from_expr_list(expr_list);
  return elist_to_hlist(edge_list);
}

// @brief 複数の Expr から変換する．
vector<AigHandle>
AigMgr::from_expr_list(
  const vector<Expr>& expr_list,
  const std::vector<AigHandle>& input_list
)
{
  std::vector<AigEdge> input_edge_list;
  hlist_to_elist(input_list, input_edge_list);
  auto edge_list = get()->from_expr_list(expr_list, input_edge_list);
  return elist_to_hlist(edge_list);
}

// @brief SopCover から変換する．
AigHandle
AigMgr::from_cover(
  const SopCover& cover
)
{
  auto edge = get()->from_cover(cover);
  return edge_to_handle(edge);
}

// @brief SopCover から変換する．
AigHandle
AigMgr::from_cover(
  const SopCover& cover,
  const std::vector<AigHandle>& input_list
)
{
  std::vector<AigEdge> input_edge_list;
  hlist_to_elist(input_list, input_edge_list);
  auto edge = get()->from_cover(cover, input_edge_list);
  return edge_to_handle(edge);
}

// @brief SopCube から変換する．
AigHandle
AigMgr::from_cube(
  const SopCube& cube
)
{
  auto edge = get()->from_cube(cube);
  return edge_to_handle(edge);
}

// @brief SopCube から変換する．
AigHandle
AigMgr::from_cube(
  const SopCube& cube,
  const std::vector<AigHandle>& input_list
)
{
  std::vector<AigEdge> input_edge_list;
  hlist_to_elist(input_list, input_edge_list);
  auto edge = get()->from_cube(cube, input_edge_list);
  return edge_to_handle(edge);
}

// @brief コファクター演算
vector<AigHandle>
AigMgr::cofactor(
  const vector<AigHandle>& cube,
  const vector<AigHandle>& handle_list
)
{
  auto cedge_list = hlist_to_elist(cube);
  auto edge_list = hlist_to_elist(handle_list);
  auto ans_list = get()->cofactor(cedge_list, edge_list);
  return elist_to_hlist(ans_list);
}

// @brief local rewriting を行う．
void
AigMgr::rewrite()
{
  get()->rewrite();
}

// @brief 参照されていないノードを削除する．
void
AigMgr::sweep()
{
  get()->sweep();
}

// @brief 内容を出力する．
void
AigMgr::print(
  std::ostream& s
) const
{
  get()->print(s);
}

END_NAMESPACE_YM_AIG
