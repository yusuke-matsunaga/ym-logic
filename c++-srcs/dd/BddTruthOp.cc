
/// @file BddTruthOp.cc
/// @brief BddTruthOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "BddMgrImpl.h"
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
BddMgr::from_truth(
  const std::string& str,
  const std::vector<BddVar>& var_list
)
{
  auto n = str.size();
  auto ni = log2(n);
  if ( (1 << ni) != n ) {
    throw std::invalid_argument{"invalid string for truth format"};
  }
  std::vector<BddVar> tmp_var_list(ni);
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

  auto level_list = BddVar::conv_to_levellist(tmp_var_list);
  BddTruthOp op(get(), level_list);
  auto edge = op.op_step(str, 0);
  return _bdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス BddTruthOp
//////////////////////////////////////////////////////////////////////

// @brief 真理値表の文字列からBDDを作る．
DdEdge
BddTruthOp::op_step(
  const std::string& str,
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
  auto str0 = str.substr(h);
  auto str1 = str.substr(0, h);
  auto e0 = op_step(str0, level + 1);
  auto e1 = op_step(str1, level + 1);
  auto ans = new_node(level, e0, e1);
  mTable.emplace(str, ans);
  return ans;
}

END_NAMESPACE_YM_DD
