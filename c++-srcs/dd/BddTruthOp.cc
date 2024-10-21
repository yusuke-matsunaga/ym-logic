
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

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgrImpl::from_truth(
  const string& str
)
{
  // str が適切な文字列か確認する．
  for ( char c: str ) {
    if ( c != '0' && c != '1' ) {
      throw std::invalid_argument("only '0' or '1' are expected");
    }
  }
  for ( SizeType n = str.size(); n > 1; n >>= 1 ) {
    if ( (n % 2) != 0 ) {
      throw std::invalid_argument("the length is expected to be the power of 2");
    }
  }

  BddTruthOp op{*this};
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
  SizeType index
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
  auto e0 = op_step(str0, index + 1);
  auto e1 = op_step(str1, index + 1);
  auto ans = new_node(index, e0, e1);
  mTable.emplace(str, ans);
  return ans;
}

END_NAMESPACE_YM_DD
