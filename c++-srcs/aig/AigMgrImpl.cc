
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
#include "CopyOp.h"
#include "EvalOp.h"

#define DEBUG_AIGMGRIMPL 0
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

// @brief 複製を作る．
AigEdge
AigMgrImpl::copy(
  AigEdge edge
)
{
  CopyOp op(this);
  auto new_edge = op.copy(edge);
  return new_edge;
}

// @brief 複製を作る．
std::vector<AigEdge>
AigMgrImpl::copy(
  const std::vector<AigEdge>& edge_list
)
{
  CopyOp op(this);
  std::vector<AigEdge> ans_list;
  ans_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto new_edge = op.copy(edge);
    ans_list.push_back(new_edge);
  }
  return ans_list;
}

// @brief 論理シミュレーションを行う．
AigBitVect
AigMgrImpl::eval(
  const std::unordered_map<SizeType, AigBitVect>& ival_dict,
  AigEdge output
) const
{
  EvalOp eval(ival_dict);
  auto oval = eval(output);
  return oval;
}

// @brief 論理シミュレーションを行う．
vector<AigBitVect>
AigMgrImpl::eval(
  const std::unordered_map<SizeType, AigBitVect>& ival_dict,
  const vector<AigEdge>& output_list
) const
{
  EvalOp eval(ival_dict);

  vector<AigBitVect> output_vals;
  auto no = output_list.size();
  output_vals.reserve(no);
  for ( auto output: output_list ) {
    auto oval = eval(output);
    output_vals.push_back(oval);
  }
  return output_vals;
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
    inc_node_ref(new_node0);
    dec_node_ref(node0);
    node->mFanins[0] = fanin0.mPackedData;
  }
  auto node1 = node->fanin1_node();
  auto new_node1 = fanin1.node();
  if ( node1 != new_node1 ) {
    inc_node_ref(new_node1);
    dec_node_ref(node1);
    node->mFanins[1] = fanin1.mPackedData;
  }
  mAndDirty = true;
  mAndTable.insert(node);
  _propagate_change(node);
}

// @brief ノードを無効化する．
void
AigMgrImpl::_deactivate(
  AigNode* node
)
{
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "_deactivate(Node#" << node->id() << ")" << endl;
  }
#endif
  mAndTable.erase(node);
  node->_deactivate();
  if ( node->mFanins[0] != 0 ) {
    dec_node_ref(node->fanin0_node());
  }
  if ( node->mFanins[1] != 0 ) {
    dec_node_ref(node->fanin1_node());
  }
  mAndDirty = true;
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
      mAndDirty = true;
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
  mAndDirty = true;
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
  if ( node->is_input() ||
       node->ref_count() == 0 ) {
    return;
  }
  if ( mark.count(node->id()) > 0 ) {
    return;
  }
  auto node0 = node->fanin0_node();
  and_dfs(node0, mark, and_list);
  auto node1 = node->fanin1_node();
  and_dfs(node1, mark, and_list);
  and_list.push_back(node);
  mark.emplace(node->id());
}

END_NONAMESPACE

// @brief mAndList を再構築する．
void
AigMgrImpl::_make_and_list() const
{
  mAndList.clear();
  mAndList.reserve(mNodeArray.size() - input_num());
  std::unordered_set<SizeType> mark;
  for ( auto& node: mNodeArray ) {
    and_dfs(node.get(), mark, mAndList);
  }
  mAndDirty = false;
}

// @brief ノードの参照回数を増やす．
void
AigMgrImpl::inc_node_ref(
  AigNode* node
)
{
  if ( node->is_input() ) {
    // 入力ノードの参照回数は変更しない．
    return;
  }
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "  inc_node_ref(Node#" << node->id() << ")" << endl;
  }
#endif
  if ( node->_inc_ref() ) {
    inc_node_ref(node->fanin0_node());
    inc_node_ref(node->fanin1_node());
  }
}

// @brief ノードの参照回数を減らす．
void
AigMgrImpl::dec_node_ref(
  AigNode* node
)
{
  if ( node->is_input() ) {
    // 入力ノードの参照回数は変更しない．
    return;
  }
  if ( node->ref_count() == 0 ) {
    return;
  }
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "  dec_node_ref(Node#" << node->id() << ")" << endl;
  }
#endif
  if ( node->_dec_ref() ) {
    _deactivate(node);
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
  s << "# of inputs: "  << input_num() << endl
    << "# of ANDs:   "  << and_num() << endl
    << "# of handles: " << mHandleHash.size() << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス CopyOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
AigEdge
CopyOp::copy(
  AigEdge edge
)
{
  if ( edge.is_zero() ) {
    return AigEdge::zero();
  }
  if ( edge.is_one() ) {
    return AigEdge::one();
  }
  auto node = edge.node();
  AigNode* new_node = nullptr;
  if ( mNodeDict.count(node->id()) == 0 ) {
    AigEdge new_edge;
    if ( node->is_input() ) {
      new_edge = mMgr->input(node->input_id());
    }
    else {
      auto new_fanin0 = copy(node->fanin0());
      auto new_fanin1 = copy(node->fanin1());
      new_edge = mMgr->and_op(new_fanin0, new_fanin1);
    }
    if ( new_edge.inv() ) {
      throw std::logic_error{"something wrong"};
    }
    new_node = new_edge.node();
    mNodeDict.emplace(node->id(), new_node);
  }
  else {
    new_node = mNodeDict.at(node->id());
  }
  return AigEdge(new_node, edge.inv());
}


//////////////////////////////////////////////////////////////////////
// クラス EvalOp
//////////////////////////////////////////////////////////////////////

// @brief 値を求める．
AigBitVect
EvalOp::eval(
  AigEdge edge
)
{
  if ( edge.is_zero() ) {
    return 0U;
  }
  else if ( edge.is_one() ) {
    return ~0U;
  }
  auto node = edge.node();
  auto val = 0U;
  if ( mValDict.count(node->id()) > 0 ) {
    val = mValDict.at(node->id());
  }
  else {
    if ( node->is_input() ) {
      if ( mIvalDict.count(node->input_id()) == 0 ) {
	std::ostringstream buf;
	buf << "Input#" << node->input_id() << "'s value is not registered";
	throw std::logic_error{buf.str()};
      }
      val = mIvalDict.at(node->input_id());
    }
    else {
      auto fanin0 = node->fanin0();
      auto fanin1 = node->fanin1();
      auto val0 = eval(fanin0);
      auto val1 = eval(fanin1);
      val = val0 & val1;
    }
    mValDict.emplace(node->id(), val);
  }
  if ( edge.inv() ) {
    val = ~val;
  }
  return val;
}

END_NAMESPACE_YM_AIG
