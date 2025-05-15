
/// @file PatMgrTest.cc
/// @brief PatMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "PatMgr.h"
#include "PatNode.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(PatMgrTest, init)
{
  PatMgr mgr;

  for ( SizeType tv = 0; tv < 0x10000; ++ tv ) {
    auto& pat_list = mgr.pat_list(tv);
    if ( pat_list.empty() ) {
      continue;
    }
    cout << "F" << hex << tv << dec << endl;
    for ( auto node: pat_list ) {
      cout << "  Node#" << node->id() << endl;
      cout << "    ";
      if ( node->is_xor() ) {
	cout << "XOR";
      }
      else {
	cout << "AND";
      }
      cout << endl;
      cout << "    TV:    " << hex << node->tv() << dec << endl
	   << "    Size:  " << node->size() << endl
	   << "    Level: " << node->level() << endl;
    }
  }
}

END_NAMESPACE_YM_LOGIC
