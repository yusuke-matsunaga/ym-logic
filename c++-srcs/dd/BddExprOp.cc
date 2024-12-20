
/// @file BddExprOp.cc
/// @brief BddExprOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "BddExprOp.h"
#include "ym/BddVar.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_DD

Bdd
BddMgrPtr::from_expr(
  const Expr& expr,
  const vector<BddVar>& var_list
)
{
  std::unordered_set<BddVar> var_set;
  for ( auto var: var_list ) {
    var_set.emplace(var);
  }
  // var_list に足りない変数を追加する．
  vector<BddVar> tmp_var_list = var_list;
  auto nv = expr.input_size();
  SizeType pos = 0;
  while ( tmp_var_list.size() < nv ) {
    for ( ; ; ) {
      auto var = variable(pos);
      ++ pos;
      if ( var_set.count(var) == 0 ) {
	tmp_var_list.push_back(var);
	break;
      }
    }
  }

  BddExprOp op{get(), level_list(tmp_var_list)};
  auto edge = op.op_step(expr);
  return _bdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス BddExprOp
//////////////////////////////////////////////////////////////////////

// @brief Expr を BDD に変換する．
DdEdge
BddExprOp::op_step(
  const Expr& expr
)
{
  if ( expr.is_zero() ) {
    return DdEdge::zero();
  }
  if ( expr.is_one() ) {
    return DdEdge::one();
  }
  if ( expr.is_literal() ) {
    auto lit = expr.literal();
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    auto level = mLevelList[varid];
    auto ans = new_node(level, DdEdge::zero(), DdEdge::one());
    if ( inv ) {
      ans = ~ans;
    }
    return ans;
  }
  if ( expr.is_and() ) {
    auto ans_edge = DdEdge::one();
    for ( auto expr1: expr.operand_list() ) {
      auto edge1 = op_step(expr1);
      ans_edge = and_step(ans_edge, edge1);
    }
    return ans_edge;
  }
  if ( expr.is_or() ) {
    auto ans_edge = DdEdge::zero();
    for ( auto expr1: expr.operand_list() ) {
      auto edge1 = op_step(expr1);
      ans_edge = or_step(ans_edge, edge1);
    }
    return ans_edge;
  }
  if ( expr.is_xor() ) {
    auto ans_edge = DdEdge::zero();
    for ( auto expr1: expr.operand_list() ) {
      auto edge1 = op_step(expr1);
      ans_edge = xor_step(ans_edge, edge1);
    }
    return ans_edge;
  }
  ASSERT_NOT_REACHED;
  return DdEdge::zero();
}

END_NAMESPACE_YM_DD
