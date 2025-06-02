
/// @file PatMgrTest.cc
/// @brief PatMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "PatMgr.h"
#include "PatNode.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

PatMgr::Tv4
calc_tv_dfs(
  const PatNode* node,
  std::unordered_map<SizeType, PatMgr::Tv4>& tv_dict
)
{
  if ( tv_dict.count(node->id()) > 0 ) {
    return tv_dict.at(node->id());
  }
  auto node0 = node->child0();
  auto inv0 = node->inv0();
  auto tv0 = calc_tv_dfs(node0, tv_dict);
  if ( inv0 ) {
    tv0 = ~tv0;
  }
  auto node1 = node->child1();
  auto inv1 = node->inv1();
  auto tv1 = calc_tv_dfs(node1, tv_dict);
  if ( inv1 ) {
    tv1 = ~tv1;
  }
  auto tv = tv0 & tv1;
  tv_dict.emplace(node->id(), tv);
  return tv;
}

PatMgr::Tv4
calc_tv(
  const PatGraph& pat,
  const Npn4& rep_npn
)
{
  auto npn = pat.npn() * rep_npn;
  static PatMgr::Tv4 tv_pat[] = {
    0xAAAA,
    0xCCCC,
    0xF0F0,
    0xFF00
  };
  std::unordered_map<SizeType, PatMgr::Tv4> tv_dict;
  tv_dict.emplace(0, 0x0000);
  for ( SizeType i = 0; i < 4; ++ i ) {
    auto iinv = npn.iinv(i);
    auto xipos = npn.iperm(i);
    auto tv = tv_pat[xipos];
    if ( iinv ) {
      tv = ~tv;
    }
    tv_dict.emplace(i + 1, tv);
  }
  auto tv = calc_tv_dfs(pat.root(), tv_dict);
  if ( npn.oinv() ) {
    tv = ~tv;
  }
  return tv;
}

END_NONAMESPACE

TEST(PatMgrTest, init)
{
  PatMgr mgr;

  for ( SizeType tv = 0; tv < 0x10000; ++ tv ) {
    Npn4 rep_npn;
    auto& pat_list = mgr.get_pat(tv, rep_npn);
    if ( pat_list.empty() ) {
      continue;
    }
    for ( auto& graph: pat_list ) {
      auto tv1 = calc_tv(graph, rep_npn);
      EXPECT_EQ( tv, tv1 );
    }
  }
}

END_NAMESPACE_YM_AIG
