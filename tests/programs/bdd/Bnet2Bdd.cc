
/// @file Bnet2Bdd.cc
/// @brief Bnet2Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Bnet2Bdd.h"
#include "ym/BnNode.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
Bnet2Bdd::Bnet2Bdd(
  BddMgr& mgr,
  const BnNetwork& network
) : mMgr{mgr},
    mNetwork{network}
{
}

// @brief BnNetwork の global function を作る．
vector<Bdd>
Bnet2Bdd::make_global_func()
{
  SizeType ni = mNetwork.input_num();
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto bdd = mMgr.make_posi_literal(VarId{i});
    mBddMap.emplace(mNetwork.input_id(i), bdd);
  }
  for ( SizeType id: mNetwork.logic_id_list() ) {
    auto bdd = make_node_func(id);
    mBddMap.emplace(id, bdd);
  }
  SizeType no = mNetwork.output_num();
  vector<Bdd> output_func(no);
  for ( SizeType i = 0; i < no; ++ i ) {
    auto id = mNetwork.output_src_id(i);
    auto bdd = mBddMap.at(id);
    output_func[i] = bdd;
  }
  return output_func;
}

// @brief BnNode に対応するBDDを作る．
Bdd
Bnet2Bdd::make_node_func(
  SizeType node_id
)
{
  const auto& node = mNetwork.node(node_id);
  SizeType ni = node.fanin_num();
  vector<Bdd> fanin_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    fanin_list[i] = mBddMap.at(node.fanin_id(i));
  }
  switch ( node.type() ) {
  case BnNodeType::C0:
    return mMgr.make_zero();
  case BnNodeType::C1:
    return mMgr.make_one();
  case BnNodeType::Buff:
    return fanin_list[0];
  case BnNodeType::Not:
    return ~fanin_list[0];
  case BnNodeType::And:
    return make_and(fanin_list);
  case BnNodeType::Nand:
    return ~make_and(fanin_list);
  case BnNodeType::Or:
    return make_or(fanin_list);
  case BnNodeType::Nor:
    return ~make_or(fanin_list);
  case BnNodeType::Xor:
    return make_xor(fanin_list);
  case BnNodeType::Xnor:
    return ~make_xor(fanin_list);
  case BnNodeType::Expr:
    return make_expr(mNetwork.expr(node.expr_id()), fanin_list);
  case BnNodeType::TvFunc:
    return make_func(mNetwork.func(node.func_id()), fanin_list);
  default:
    ASSERT_NOT_REACHED;
  }
  return Bdd::make_invalid();
}

// @brief 論理積を計算する．
Bdd
Bnet2Bdd::make_and(
  const vector<Bdd>& fanin_list
)
{
  auto bdd = mMgr.make_one();
  for ( auto bdd1: fanin_list ) {
    bdd &= bdd1;
  }
  return bdd;
}

// @brief 論理和を計算する．
Bdd
Bnet2Bdd::make_or(
  const vector<Bdd>& fanin_list
)
{
  auto bdd = mMgr.make_zero();
  for ( auto bdd1: fanin_list ) {
    bdd |= bdd1;
  }
  return bdd;
}

// @brief 排他的論理和を計算する．
Bdd
Bnet2Bdd::make_xor(
  const vector<Bdd>& fanin_list
)
{
  auto bdd = mMgr.make_zero();
  for ( auto bdd1: fanin_list ) {
    bdd ^= bdd1;
  }
  return bdd;
}

// @brief 論理式を計算する．
Bdd
Bnet2Bdd::make_expr(
  const Expr& expr,
  const vector<Bdd>& fanin_list
)
{
  if ( expr.is_zero() ) {
    return mMgr.make_zero();
  }
  if ( expr.is_one() ) {
    return mMgr.make_one();
  }
  if ( expr.is_posi_literal() ) {
    return fanin_list[expr.varid().val()];
  }
  if ( expr.is_nega_literal() ) {
    return ~fanin_list[expr.varid().val()];
  }
  SizeType nc = expr.child_num();
  vector<Bdd> child_funcs(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    child_funcs[i] = make_expr(expr.child(i), fanin_list);
  }
  if ( expr.is_and() ) {
    return make_and(child_funcs);
  }
  if ( expr.is_or() ) {
    return make_or(child_funcs);
  }
  if ( expr.is_xor() ) {
    return make_xor(child_funcs);
  }
  ASSERT_NOT_REACHED;
  return Bdd::make_invalid();
}

// @brief 真理値表を計算する．
Bdd
Bnet2Bdd::make_func(
  const TvFunc& func,
  const vector<Bdd>& fanin_list
)
{
  // 未完
  return Bdd::make_invalid();
}

END_NAMESPACE_YM
