
/// @file Bdd_truth.cc
/// @brief Bdd::to_truth() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "dd/BddMgrImpl.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

string
truth_step(
  DdEdge edge,
  SizeType pos,
  const vector<SizeType>& index_list
)
{
  if ( pos >= index_list.size() ) {
    if ( edge.is_zero() ) {
      return "0";
    }
    if ( edge.is_one() ) {
      return "1";
    }
    throw std::invalid_argument("Bdd::to_truth(var_list): invalid var_list");
  }
  auto index = index_list[pos];
  // edge を index で分解する．
  // -> edge0 と edge1
  DdEdge edge0;
  DdEdge edge1;
  auto ans0 = truth_step(edge0, pos + 1, index_list);
  auto ans1 = truth_step(edge1, pos + 1, index_list);
  auto ans = ans1 + ans0;
  return ans;
}

END_NONAMESPACE

// @brief 内容を真理値表の文字列に変換する．
string
Bdd::to_truth(
  const vector<BddVar> var_list
) const
{
  if ( is_invalid() ) {
    // 不正値の場合には空文字列を返す．
    return string{};
  }
  auto index_list = mMgr->index_list(var_list);
  return truth_step(DdEdge{mRoot}, 0, index_list);
}

END_NAMESPACE_YM_DD
