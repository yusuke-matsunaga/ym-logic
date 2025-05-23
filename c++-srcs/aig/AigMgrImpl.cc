
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


BEGIN_NAMESPACE_YM_AIG

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
  const Expr& expr
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
    while ( input_num() <= varid ) {
      make_input();
    }
    auto node = mInputArray[varid];
    return AigEdge{node, lit.is_negative()};
  }
  auto n = expr.operand_num();
  vector<AigEdge> opr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    opr_list[i] = from_expr(expr.operand(i));
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

// @brief 複数の Expr から変換する．
vector<AigEdge>
AigMgrImpl::from_expr_list(
  const vector<Expr>& expr_list
)
{
  vector<AigEdge> ans_list;
  ans_list.reserve(expr_list.size());
  for ( auto& expr: expr_list ) {
    auto h = from_expr(expr);
    ans_list.push_back(h);
  }
  return ans_list;
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

END_NAMESPACE_YM_AIG
