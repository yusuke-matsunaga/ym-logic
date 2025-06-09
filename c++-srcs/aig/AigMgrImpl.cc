
/// @file AigMgrImpl.cc
/// @brief AigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "aig/AigMgrImpl.h"
#include "ym/AigHandle.h"
#include "ValArray.h"
#include "EdgeDict.h"

#define DEBUG_AIGMGRIMPL 1
#define DOUT std::cout


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス EventMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EventMgr::EventMgr(
  AigMgrImpl* mgr
) : mMgr{mgr}
{
  mgr->add_event_mgr(this);
}

// @brief デストラクタ
EventMgr::~EventMgr()
{
  mMgr->delete_event_mgr(this);
}

// @brief ハンドルの内容が変化したときに呼ばれる関数
void
EventMgr::change_proc(
  AigHandle* handle
)
{
}

// @brief ノードのファンインが変化したときに呼ばれる関数
void
EventMgr::change_proc(
  AigNode* node
)
{
}

// @brief ノードが削除されるときに呼ばれる関数
void
EventMgr::delete_proc(
  AigNode* node ///< [in] 対象のノード
)
{
}


//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgrImpl::AigMgrImpl()
{
}

// @brief デストラクタ
AigMgrImpl::~AigMgrImpl()
{
}

// @brief 論理シミュレーションを行う．
AigBitVect
AigMgrImpl::eval(
  const vector<AigBitVect>& input_vals,
  AigEdge output
) const
{
  // ノード番号をキーにして値を入れておく配列
  ValArray val_array(node_num());

  eval_sub(input_vals, val_array);

  return val_array.get_val(output);
}

// @brief 論理シミュレーションを行う．
vector<AigBitVect>
AigMgrImpl::eval(
  const vector<AigBitVect>& input_vals,
  const vector<AigEdge>& output_list
) const
{
  // ノード番号をキーにして値を入れておく配列
  ValArray val_array(node_num());

  eval_sub(input_vals, val_array);

  vector<AigBitVect> output_vals;
  auto no = output_list.size();
  output_vals.reserve(no);
  for ( auto edge: output_list ) {
    auto bv = val_array.get_val(edge);
    output_vals.push_back(bv);
  }
  return output_vals;
}

// @brief eval() の下請け関数
void
AigMgrImpl::eval_sub(
  const vector<AigBitVect>& input_vals,
  ValArray& val_array
) const
{
  for ( auto node: mInputArray ) {
    auto bv = input_vals[node->input_id()];
    val_array.put_val(node, bv);
  }
  // ノードの評価を行う．
  auto& node_list = and_list();
  for ( auto node: node_list ) {
    auto h0 = node->fanin0();
    auto h1 = node->fanin1();
    auto bv0 = val_array.get_val(h0);
    auto bv1 = val_array.get_val(h1);
    auto bv = bv0 & bv1;
    val_array.put_val(node, bv);
  }
}

BEGIN_NONAMESPACE

void
dfs(
  AigNode* node,
  std::unordered_set<SizeType>& mark
)
{
  if ( mark.count(node->id()) > 0 ) {
    return;
  }
  mark.emplace(node->id());
  if ( node->is_input() ) {
    return;
  }
  dfs(node->fanin0_node(), mark);
  dfs(node->fanin1_node(), mark);
}

END_NONAMESPACE

// @brief ノード数を返す．
SizeType
AigMgrImpl::size(
  const vector<AigEdge>& edge_list
) const
{
  std::unordered_set<SizeType> mark;
  for ( auto& edge: edge_list ) {
    if ( !edge.is_const() ) {
      dfs(edge.node(), mark);
    }
  }
  return mark.size();
}

// @brief and_op() の下請け関数
AigEdge
AigMgrImpl::and_sub(
  const vector<AigEdge>& fanin_list,
  SizeType begin,
  SizeType end
)
{
  if ( begin >= end ) {
    throw std::logic_error{"begin >= end"};
  }
  auto n = end - begin;
  if ( n == 1 ) {
    return fanin_list[begin];
  }
  if ( n == 2 ) {
    auto e0 = fanin_list[begin + 0];
    auto e1 = fanin_list[begin + 1];
    return and_op(e0, e1);
  }
  auto nh = (begin + end) / 2;
  auto e0 = and_sub(fanin_list, begin, nh);
  auto e1 = and_sub(fanin_list, nh, end);
  return and_op(e0, e1);
}

// @brief or_op() の下請け関数
AigEdge
AigMgrImpl::or_sub(
  const vector<AigEdge>& fanin_list,
  SizeType begin,
  SizeType end
)
{
  if ( begin >= end ) {
    throw std::logic_error{"begin >= end"};
  }
  auto n = end - begin;
  if ( n == 1 ) {
    return fanin_list[begin];
  }
  if ( n == 2 ) {
    auto e0 = fanin_list[begin + 0];
    auto e1 = fanin_list[begin + 1];
    return or_op(e0, e1);
  }
  auto nh = (begin + end) / 2;
  auto e0 = or_sub(fanin_list, begin, nh);
  auto e1 = or_sub(fanin_list, nh, end);
  return or_op(e0, e1);
}

// @brief xor_op() の下請け関数
AigEdge
AigMgrImpl::xor_sub(
  const vector<AigEdge>& fanin_list,
  SizeType begin,
  SizeType end
)
{
  if ( begin >= end ) {
    throw std::logic_error{"begin >= end"};
  }
  auto n = end - begin;
  if ( n == 1 ) {
    return fanin_list[begin];
  }
  if ( n == 2 ) {
    auto e0 = fanin_list[begin + 0];
    auto e1 = fanin_list[begin + 1];
    return xor_op(e0, e1);
  }
  auto nh = (begin + end) / 2;
  auto e0 = xor_sub(fanin_list, begin, nh);
  auto e1 = xor_sub(fanin_list, nh, end);
  return xor_op(e0, e1);
}

// @brief Expr から変換する．
AigEdge
AigMgrImpl::from_expr(
  const Expr& expr,
  const std::vector<AigEdge>& input_list
)
{
  if ( expr.is_zero() ) {
    return AigEdge::zero();
  }
  if ( expr.is_one() ) {
    return AigEdge::one();
  }
  if ( expr.is_literal() ) {
    auto lit = expr.literal();
    auto varid = lit.varid();
    if ( input_list.size() <= varid ) {
      throw std::invalid_argument{"input_list is too small"};
    }
    auto e = input_list[varid];
    if ( lit.is_positive() ) {
      return e;
    }
    else {
      return ~e;
    }
  }
  auto n = expr.operand_num();
  vector<AigEdge> opr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    opr_list[i] = from_expr(expr.operand(i), input_list);
  }
  if ( expr.is_and() ) {
    return and_op(opr_list);
  }
  if ( expr.is_or() ) {
    return or_op(opr_list);
  }
  if ( expr.is_xor() ) {
    return xor_op(opr_list);
  }
  ASSERT_NOT_REACHED;
  return AigEdge::zero();
}

// @brief SopCover から変換する．
AigEdge
AigMgrImpl::from_cover(
  const SopCover& cover,
  const std::vector<AigEdge>& input_list
)
{
  auto nc = cover.cube_num();
  if ( nc == 0 ) {
    // 空のカバーは 0
    return AigEdge::zero();
  }
  std::vector<AigEdge> edge_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    auto cube = cover.get_cube(i);
    auto e1 = from_cube(cube, input_list);
    edge_list[i] = e1;
  }
  return or_op(edge_list);
}

// @brief SopCube から変換する．
AigEdge
AigMgrImpl::from_cube(
  const SopCube& cube,
  const std::vector<AigEdge>& input_list
)
{
  auto lit_list = cube.literal_list();
  if ( lit_list.empty() ) {
    // 空のキューブは 1
    return AigEdge::one();
  }
  auto n = lit_list.size();
  std::vector<AigEdge> edge_list;
  edge_list.reserve(n);
  for ( auto lit: lit_list ) {
    auto e1 = input_list[lit.varid()];
    if ( lit.is_negative() ) {
      e1 = ~e1;
    }
    edge_list.push_back(e1);
  }
  return and_op(edge_list);
}

BEGIN_NONAMESPACE

// @brief cube の内容を辞書に登録する．
void
cofactor_init(
  const vector<AigEdge>& cedge_list,
  EdgeDict& res_dict
)
{
  for ( auto edge: cedge_list ) {
    if ( edge.is_const() ) {
      // 定数が来ることはないはず．
      // 今はスキップする．
      continue;
    }
    auto node = edge.node();
    auto inv = edge.inv();
    auto new_edge = edge.inv() ? AigEdge::zero() : AigEdge::one();
    res_dict.put(node, new_edge);
  }
}

END_NONAMESPACE

// @brief コファクター演算
AigEdge
AigMgrImpl::cofactor(
  const vector<AigEdge>& cedge_list,
  AigEdge edge
)
{
  // 元のノード番号をキーにして新しい枝を格納する辞書
  EdgeDict res_dict;

  // cedge_list の内容を res_dict に入れておく．
  cofactor_init(cedge_list, res_dict);

  // edge の TFI をボトムアップに作る．
  auto new_edge = cofactor_sub(edge, res_dict);
  return new_edge;
}

// @brief コファクター演算
vector<AigEdge>
AigMgrImpl::cofactor(
  const vector<AigEdge>& cedge_list,
  const vector<AigEdge>& edge_list
)
{
  // 元のノード番号をキーにして新しい枝を格納する辞書
  EdgeDict res_dict;

  // cedge_list の内容を res_dict に入れておく．
  cofactor_init(cedge_list, res_dict);

  // edge_list の TFI をボトムアップに作る．
  vector<AigEdge> new_edge_list;
  new_edge_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto new_edge = cofactor_sub(edge, res_dict);
    new_edge_list.push_back(new_edge);
  }
  return new_edge_list;
}

// @brief cofactor() の下請け関数
AigEdge
AigMgrImpl::cofactor_sub(
  AigEdge edge,
  EdgeDict& res_dict
)
{
  if ( res_dict.check(edge) ) {
    return res_dict.get(edge);
  }
  auto node = edge.node();
  auto inv = edge.inv();
  AigEdge new_edge;
  if ( node->is_and() ) {
    auto e0 = node->fanin0();
    auto new_e0 = cofactor_sub(e0, res_dict);
    auto e1 = node->fanin1();
    auto new_e1 = cofactor_sub(e1, res_dict);
    new_edge = and_op(new_e0, new_e1);
  }
  else {
    new_edge = AigEdge{node, false};
  }
  res_dict.put(node, new_edge);
  if ( inv ) {
    new_edge = ~new_edge;
  }
  return new_edge;
}

// @brief ノードのファンインを変更する．
void
AigMgrImpl::_change_fanin(
  AigNode* node,
  AigEdge fanin0,
  AigEdge fanin1
)
{
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "_change_fanin(Node#" << node->id()
	 << ", " << fanin0
	 << ", " << fanin1
	 << ")" << endl;
  }
#endif
  mAndTable.erase(node);
  auto node0 = node->fanin0_node();
  auto new_node0 = fanin0.node();
  if ( node0 != new_node0 ) {
    _inc_node_ref(new_node0);
    _dec_node_ref(node0);
    node->mFanins[0] = fanin0.mPackedData;
  }
  auto node1 = node->fanin1_node();
  auto new_node1 = fanin1.node();
  if ( node1 != new_node1 ) {
    _inc_node_ref(new_node1);
    _dec_node_ref(node1);
    node->mFanins[1] = fanin1.mPackedData;
  }
  mAndDirty = true;
  mAndTable.insert(node);
  _propagate_change(node);
}

// @brief 参照回数が0のノードを取り除く
void
AigMgrImpl::sweep()
{
  // mNodeArray は unique_ptr<> の配列なのでちょっと面倒
  // とりあえず，削除されるノードを末尾に寄せて最後に
  // erase() する．
  auto rpos = mNodeArray.begin();
  auto epos = mNodeArray.end();
  auto wpos = rpos;
  SizeType id = 0;
  for ( ; rpos != epos; ++ rpos ) {
    auto& node_ptr = *rpos;
    if ( node_ptr->is_input() || node_ptr->ref_count() > 0 ) {
      node_ptr->mId = id;
      if ( wpos != rpos ) {
	std::swap(*wpos, node_ptr);
      }
      ++ id;
      ++ wpos;
    }
    else {
      auto node = node_ptr.get();
      mAndTable.erase(node);
#if DEBUG_AIGMGRIMPL
      {
	DOUT << "  Node#" << node->id() << " is deleted" << endl;
      }
#endif
    }
  }
  if ( wpos != epos ) {
    // ここで参照回数0のノードが開放される．
    mNodeArray.erase(wpos, epos);
  }
}

// @brief ANDノードを作る．
AigNode*
AigMgrImpl::_new_and(
  AigEdge fanin0,
  AigEdge fanin1
)
{
  auto id = mNodeArray.size();
  auto node = new AigNode{id, fanin0, fanin1};
  mNodeArray.push_back(std::unique_ptr<AigNode>{node});
  mAndList.push_back(node);
  mAndTable.insert(node);
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "  _new_and(Node#"
	 << node->id()
	 << ": AND(" << fanin0
	 << ", " << fanin1 << "))" << endl;
  }
#endif
  return node;
}

BEGIN_NONAMESPACE

void
and_dfs(
  AigNode* node,
  std::unordered_set<SizeType>& mark,
  std::vector<AigNode*>& and_list
)
{
  if ( node->is_input() || node->ref_count() == 0 ) {
    return;
  }
  if ( mark.count(node->id()) > 0 ) {
    return;
  }
  auto node0 = node->fanin0_node();
  and_dfs(node0, mark, and_list);
  auto node1 = node->fanin1_node();
  and_dfs(node1, mark, and_list);
  mark.emplace(node->id());
}

END_NONAMESPACE

// @brief mAndList を再構築する．
void
AigMgrImpl::_make_and_list() const
{
  mAndList.clear();
  mAndList.reserve(mNodeArray.size() - mInputArray.size());
  std::unordered_set<SizeType> mark;
  for ( auto& node: mNodeArray ) {
    and_dfs(node.get(), mark, mAndList);
  }
  mAndDirty = false;
}

// @brief ノードの参照回数を増やす．
void
AigMgrImpl::_inc_node_ref(
  AigNode* node
)
{
  if ( node->is_input() ) {
    // 入力ノードの参照回数は変更しない．
    return;
  }
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "  _inc_node_ref(Node#" << node->id() << ")" << endl;
  }
#endif
  if ( node->_inc_ref() ) {
    _inc_node_ref(node->fanin0_node());
    _inc_node_ref(node->fanin1_node());
  }
}

// @brief ノードの参照回数を減らす．
void
AigMgrImpl::_dec_node_ref(
  AigNode* node
)
{
  if ( node->is_input() ) {
    // 入力ノードの参照回数は変更しない．
    return;
  }
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "  _dec_node_ref(Node#" << node->id() << ")" << endl;
  }
#endif
  if ( node->_dec_ref() ) {
    _dec_node_ref(node->fanin0_node());
    _dec_node_ref(node->fanin1_node());
  }
}

// @brief 内部情報が正しいかチェックする．
void
AigMgrImpl::_sanity_check() const
{
  for ( auto& node: mNodeArray ) {
    if ( node->ref_count() == 0 ) {
      continue;
    }
    if ( node->is_input() ) {
      continue;
    }
    if ( node->fanin0_node() == nullptr ) {
      print(cout);
      std::ostringstream buf;
      buf << "Node#" << node->id() << "->fanin0_node() == nullptr";
      cout << buf.str() << endl;
      throw std::logic_error{buf.str()};
    }
    if ( node->fanin1_node() == nullptr ) {
      print(cout);
      std::ostringstream buf;
      buf << "Node#" << node->id() << "->fanin1_node() == nullptr";
      cout << buf.str() << endl;
      throw std::logic_error{buf.str()};
    }
  }
}

// @brief 内容を出力する．
void
AigMgrImpl::print(
  std::ostream& s
) const
{
  for ( auto& node: mNodeArray ) {
    node->print(s);
  }
  for ( auto handle: mHandleHash ) {
    auto edge = handle->_edge();
    s << "Handle("
      << hex << handle << dec
      << "): " << edge << endl;
  }
  s << "# of inputs: "  << mInputArray.size() << endl
    << "# of ANDs:   "  << and_num() << endl
    << "# of handles: " << mHandleHash.size() << endl;
}

END_NAMESPACE_YM_AIG
