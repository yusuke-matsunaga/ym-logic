
/// @file BddTruthOp.cc
/// @brief BddTruthOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "dd/BddMgrImpl.h"
#include "BddTruthOp.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

SizeType
log2(
  SizeType x
)
{
  SizeType y = 0;
  while ( (1 << y) < x ) {
    ++ y;
  }
  return y;
}

END_NONAMESPACE

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgrImpl::from_truth(
  const string& str,
  const vector<BddVar>& var_list
)
{
  auto n = str.size();
  auto ni = log2(n);
  if ( (1 << ni) != n ) {
    throw std::invalid_argument{"invalid string for truth format"};
  }
  vector<BddVar> tmp_var_list(ni);
  if ( var_list.empty() ) {
    for ( SizeType i = 0; i < ni; ++ i ) {
      tmp_var_list[i] = variable(i);
    }
  }
  else if ( var_list.size() != ni ) {
    throw std::invalid_argument{"var_list.size() mismatch"};
  }
  tmp_var_list = var_list;

  // str が適切な文字列か確認する．
  for ( char c: str ) {
    if ( c != '0' && c != '1' ) {
      throw std::invalid_argument("only '0' or '1' are expected");
    }
  }

  BddTruthOp op{*this, level_list(tmp_var_list)};
  auto e = op.op_step(str, 0);
  return _bdd(e);
}


//////////////////////////////////////////////////////////////////////
// クラス BddTruthOp
//////////////////////////////////////////////////////////////////////

// @brief 真理値表の文字列からBDDを作る．
DdEdge
BddTruthOp::op_step(
  const string& str,
  SizeType level
)
{
  if ( str == "0" ) {
    return DdEdge::zero();
  }
  if ( str == "1" ) {
    return DdEdge::one();
  }
  if ( mTable.count(str) > 0 ) {
    // 同じ文字列の結果が記録されていたらそれを用いる．
    return mTable.at(str);
  }

  SizeType n = str.size();
  SizeType h = n / 2;
  string str0 = str.substr(h);
  string str1 = str.substr(0, h);
  auto e0 = op_step(str0, level + 1);
  auto e1 = op_step(str1, level + 1);
  auto ans = new_node(level, e0, e1);
  mTable.emplace(str, ans);
  return ans;
}

END_NAMESPACE_YM_DD
