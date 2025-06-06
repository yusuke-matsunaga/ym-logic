
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

// @brief ANDノードの入力側からのトポロジカル順のリストを得る．
std::vector<AigNode*>
AigMgrImpl::and_list() const
{
  std::vector<AigNode*> node_list;
  node_list.reserve(and_num());
  for ( auto& node: mNodeArray ) {
    if ( node->is_input() || node->ref_count() == 0 ) {
      continue;
    }
    node_list.push_back(node.get());
  }
  return node_list;
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
  // ノードの評価を行う．
  // AIG はノードの作り方から mNodeArray が
  // かならずトロポジカルソートされている性質がある．
  for ( auto& node: mNodeArray ) {
    AigBitVect bv;
    if ( node->is_input() ) {
      bv = input_vals[node->input_id()];
    }
    else {
      auto h0 = node->fanin0();
      auto h1 = node->fanin1();
      auto bv0 = val_array.get_val(h0);
      auto bv1 = val_array.get_val(h1);
      bv = bv0 & bv1;
    }
    val_array.put_val(node.get(), bv);
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

// @brief ノードの置き換えを行う．
void
AigMgrImpl::replace(
  AigNode* old_node,
  AigEdge new_edge
)
{
#if DEBUG_AIGMGRIMPL
  {
    DOUT << "replace(Node#" << old_node->id()
	 << ", " << new_edge << ")" << endl;
    for ( auto& fo_info: old_node->mFanoutList ) {
      if ( fo_info.type == 2 ) {
	DOUT << "  -> AigHandle(" << fo_info.handle->_edge() << ")" << endl;
      }
      else {
	DOUT << "  -> AigNode(Node#" << fo_info.node->id()
	     << "), " << static_cast<int>(fo_info.type) << endl;
      }
    }
  }
#endif

  for ( auto& fo_info: old_node->mFanoutList ) {
    if ( fo_info.type == 2 ) {
      auto handle = fo_info.handle;
      auto new_edge1 = new_edge;
      if ( handle->_edge().inv() ) {
	new_edge1 = ~new_edge1;
      }
      handle->mEdge = new_edge1.mPackedData;
      new_edge1.add_fanout(handle);
      _propagate_change(handle);
    }
    else {
      auto fo_node = fo_info.node;
      auto pos = fo_info.type;
      auto old_edge = fo_node->fanin(pos);
      auto new_edge1 = new_edge;
      if ( old_edge.inv() ) {
	new_edge1 = ~new_edge1;
      }
      fo_node->mFanins[pos] = new_edge1.mPackedData;
      new_edge1.add_fanout(fo_node, pos);
      _propagate_change(fo_node);
    }
  }
  old_node->mFanoutList.clear();
  _free_node(old_node);
  _sanity_check();
}

// @brief 参照回数が0のノードを取り除く
void
AigMgrImpl::sweep()
{
  auto node_list = and_list();
  std::unordered_map<SizeType, AigEdge> remap_dict;
  for ( auto node: node_list ) {
    bool changed = false;
    auto fanin0 = node->fanin0();
    if ( remap_dict.count(fanin0.node()->id()) > 0 ) {
      changed = true;
      fanin0 = remap_dict.at(fanin0.node()->id());
      if ( node->fanin0_inv() ) {
	fanin0 = ~fanin0;
      }
    }
    auto fanin1 = node->fanin1();
    if ( remap_dict.count(fanin1.node()->id()) > 0 ) {
      changed = true;
      fanin1 = remap_dict.at(fanin1.node()->id());
      if ( node->fanin1_inv() ) {
	fanin1 = ~fanin1;
      }
    }
    if ( !changed ) {
      // 変わらず．
      continue;
    }
    if ( fanin0.is_zero() || fanin1.is_zero() ) {
      // node は 0 になる．
      remap_dict.emplace(node->id(), AigEdge::zero());
      _free_node(node);
    }
    if ( fanin0.is_one() && fanin1.is_one() ) {
      // node は 1 になる．
      remap_dict.emplace(node->id(), AigEdge::one());
      _free_node(node);
    }
    if ( fanin0.is_one() ) {
      // node は fanin1 に置き換えられる．
      remap_dict.emplace(node->id(), fanin1);
      _free_node(node);
    }
    if ( fanin1.is_one() ) {
      // node は fanin0 に置き換えられる．
      remap_dict.emplace(node->id(), fanin0);
      _free_node(node);
    }
    if ( fanin0 == fanin1 ) {
      // node は fanin0 に置き換えられる．
      remap_dict.emplace(node->id(), fanin0);
      _free_node(node);
    }
    if ( fanin0 == ~fanin1 ) {
      // node は 0 に置き換えられる．
      remap_dict.emplace(node->id(), AigEdge::zero());
      _free_node(node);
    }
    // 新しいノードを作る．
    auto new_edge = and_op(fanin0, fanin1);
    remap_dict.emplace(node->id(), new_edge);
    _free_node(node);
  }

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
      if ( wpos != rpos ) {
	std::swap(*wpos, *rpos);
      }
      node_ptr->mId = id;
      ++ id;
      ++ wpos;
    }
  }
  if ( wpos != epos ) {
    // ここで参照回数0のノードが開放される．
    mNodeArray.erase(wpos, epos);
  }
}

// @brief ノードを削除する．
void
AigMgrImpl::_free_node(
  AigNode* node
)
{
  std::deque<AigNode*> queue;

  queue.push_back(node);
  while ( !queue.empty() ) {
    auto node = queue.front();
    queue.pop_front();
    if ( node->is_input() ) {
      // 入力ノードは削除しない．
      continue;
    }
#if DEBUG_AIGMGRIMPL
    {
      DOUT << "  _free_node(Node#" << node->id() << ")" << endl;
    }
#endif
    -- mNodeNum;
    mAndTable.erase(node);

    auto node0 = node->fanin0_node();
    if ( node0 != nullptr ) {
      if ( node0->delete_fanout(node, 0) ) {
	queue.push_back(node0);
      }
      node->mFanins[0] = 0;
    }

    auto node1 = node->fanin1_node();
    if ( node1 != nullptr ) {
      if ( node1->delete_fanout(node, 1) ) {
	queue.push_back(node1);
      }
      node->mFanins[1] = 0;
    }
    _propagate_delete(node);
  }
}

// @brief 内部情報が正しいかチェックする．
void
AigMgrImpl::_sanity_check() const
{
  // Phase-0
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

  // Phase-1
  for ( auto& node: mNodeArray ) {
    for ( auto& fo_info: node->mFanoutList ) {
      if ( fo_info.type == 2 ) {
	auto handle = fo_info.handle;
	if ( handle->_edge().node() != node.get() ) {
	  print(cout);
	  {
	    cout << endl
		 << "Node#" << node->id()
		 << " -> AigHandle" << endl;
	  }
	  throw std::logic_error{"something wrong(1)"};
	}
      }
      else {
	auto fo_node = fo_info.node;
	if ( fo_node->fanin_node(fo_info.type) != node.get() ) {
	  print(cout);
	  {
	    cout << endl
		 << "Node#" << node->id()
		 << " -> Node#" << fo_node->id()
		 << "@" << static_cast<int>(fo_info.type)
		 << ": " << fo_node->fanin_node(fo_info.type)->id() << endl;
	  }
	  throw std::logic_error{"something wrong(2)"};
	}
      }
    }
  }
  // Phase-2
  for ( auto& node: mNodeArray ) {
    if ( !node->is_and() ) {
      continue;
    }
    auto node0 = node->fanin0_node();
    if ( node0 != nullptr ) {
      bool found = false;
      for ( auto& fo_info: node0->mFanoutList ) {
	if ( fo_info.type == 0 && fo_info.node == node.get() ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	print(cout);
	cout << endl
	     << "checking Node#" << node->id()
	     << "@0 = Node#" << node0->id() << endl;
	for ( auto& fo_info: node0->mFanoutList ) {
	  if ( fo_info.type == 2 ) {
	    cout << "  AigHandle" << endl;
	  }
	  else {
	    cout << "  AigNode(Node#" << fo_info.node->id()
		 << ")@" << static_cast<int>(fo_info.type) << endl;
	  }
	}
	throw std::logic_error{"something wrong(3)"};
      }
    }
    auto node1 = node->fanin1_node();
    if ( node1 != nullptr ) {
      auto found = false;
      for ( auto& fo_info: node1->mFanoutList ) {
	if ( fo_info.type == 1 && fo_info.node == node.get() ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	print(cout);
	cout << endl
	     << "checking Node#" << node->id()
	     << "@1 = Node#" << node1->id() << endl;
	for ( auto& fo_info: node1->mFanoutList ) {
	  if ( fo_info.type == 2 ) {
	    cout << "  AigHandle" << endl;
	  }
	  else {
	    cout << "  AigNode(Node#" << fo_info.node->id()
		 << "@" << static_cast<int>(fo_info.type) << endl;
	  }
	}
	throw std::logic_error{"something wrong(4)"};
      }
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
    s << "Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else { // node->is_and() ) {
      s << "And("
	<< node->fanin0()
	<< ", "
	<< node->fanin1()
	<< ")";
    }
    s << endl;
    for ( auto& fo_info: node->mFanoutList ) {
      s << "  -> ";
      if ( fo_info.type == 2 ) {
	s << "AigHandle("
	  << hex << fo_info.handle << dec
	  << ")";
      }
      else {
	s << "Node#" << fo_info.node->id()
	  << ", " << static_cast<int>(fo_info.type);
      }
      s << endl;
    }
    s << endl;
  }
}

END_NAMESPACE_YM_AIG
