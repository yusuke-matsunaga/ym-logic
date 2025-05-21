
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

TEST(PatMgrTest, init)
{
  PatMgr mgr;

  for ( SizeType tv = 0; tv < 0x10000; ++ tv ) {
    Npn4 rep_npn;
    auto& pat_list = mgr.get_pat(tv, rep_npn);
    if ( pat_list.empty() ) {
      continue;
    }
    cout << hex << setw(4) << setfill('0') << tv
	 << dec << setfill(' ') << endl;
    for ( auto& graph: pat_list ) {
      auto root = graph.root;
      auto npn = graph.npn * rep_npn;
      cout << "  Node#" << root->id() << ": " << npn << endl;
      auto tv1 = npn.xform(root->tv());
      EXPECT_EQ( tv, tv1 );
    }
  }
}

END_NAMESPACE_YM_AIG
