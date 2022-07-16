
/// @file TruthOp.cc
/// @brief TruthOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "TruthOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 真理値表の文字列からBDDを作る．
BddEdge
TruthOp::op_step(
  const string& str,
  SizeType index
)
{
  if ( str == "0" ) {
    return BddEdge::make_zero();
  }
  if ( str == "1" ) {
    return BddEdge::make_one();
  }
  if ( mTable.count(str) > 0 ) {
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

END_NAMESPACE_YM_BDD
