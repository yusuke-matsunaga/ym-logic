
/// @file Bdd_truth.cc
/// @brief Bdd::to_truth() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

string
truth_step(
  DdEdge edge,
  SizeType index,
  SizeType input_num
)
{
  if ( input_num < index ) {
    throw std::invalid_argument("Bdd::to_truth(): input_num is too small");
  }

  SizeType gap;
  string ans;
  if ( edge.is_zero() ) {
    gap = input_num - index;
    ans = "0";
  }
  else if ( edge.is_one() ) {
    gap = input_num - index;
    ans = "1";
  }
  else {
    auto node = edge.node();
    auto inv = edge.inv();
    auto edge0 = node->edge0() ^ inv;
    auto edge1 = node->edge1() ^ inv;
    auto ans0 = truth_step(edge0, node->index() + 1, input_num);
    auto ans1 = truth_step(edge1, node->index() + 1, input_num);
    gap = node->index() - index;
    ans = ans1 + ans0;
  }
  for ( SizeType i = 0; i < gap; ++ i ) {
    ans += ans;
  }
  return ans;
}

END_NONAMESPACE

// @brief 内容を真理値表の文字列に変換する．
string
Bdd::to_truth(
  SizeType input_num
) const
{
  if ( mMgr == nullptr ) {
    // 不正値の場合には空文字列を返す．
    return string{};
  }
  return truth_step(DdEdge{mRoot}, 0, input_num);
}

END_NAMESPACE_YM_DD
