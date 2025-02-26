
/// @file KIXTest.cc
/// @brief KIXTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/KIX.h"


BEGIN_NAMESPACE_YM_RC

TEST(KIXTest, test1)
{
  vector<KIX::CoverInfo> cover_list;
  {
    auto a = Literal(0);
    auto b = Literal(1);
    auto c = Literal(2);
    auto d = Literal(3);
    auto e = Literal(4);
    auto f = Literal(5);
    auto g = Literal(6);

    auto F = SopCover(7,
		      { {a, f},
			{b, f},
			{a, g},
			{c, g},
			{a, d, e},
			{b, d, e},
			{c, d, e} });
    auto inputs_F = vector<SizeType>{0, 1, 2, 3, 4, 5, 6};
    cover_list.push_back(KIX::CoverInfo{F, inputs_F});
  }
  {
    auto a = Literal(0);
    auto b = Literal(1);
    auto c = Literal(2);
    auto e = Literal(3);
    auto f = Literal(4);

    auto G = SopCover(5,
		      { {a, f},
			{b, f},
			{a, c, e},
			{b, c, e} });
    auto inputs_G = vector<SizeType>{0, 1, 2, 4, 5};
    cover_list.push_back(KIX::CoverInfo{G, inputs_G});
  }
  {
    auto a = Literal(0);
    auto c = Literal(1);
    auto d = Literal(2);
    auto e = Literal(3);

    auto H = SopCover(4,
		      { {a, d, e},
			{c, d, e} });
    auto inputs_H = vector<SizeType>{0, 2, 3, 4};
    cover_list.push_back(KIX::CoverInfo{H, inputs_H});
  }

  {
    SizeType nl = 0;
    for ( auto& cover_info: cover_list ) {
      nl += cover_info.cover.literal_num();
    }
    cout << "Initial Literal Num: " << nl << endl;
  }
  for ( auto& cover_input: cover_list ) {
    cout << cover_input.cover << endl;
    for ( auto input: cover_input.inputs ) {
      cout << " " << input;
    }
    cout << endl;
    cout << endl;
  }

  auto ans_cover_list = KIX::extract(cover_list);

  {
    SizeType nl = 0;
    for ( auto& cover_info: ans_cover_list ) {
      nl += cover_info.cover.literal_num();
    }
    cout << "Final Literal Num: " << nl << endl;
  }
  for ( auto& cover_input: ans_cover_list ) {
    cout << cover_input.cover << endl;
    for ( auto input: cover_input.inputs ) {
      cout << " " << input;
    }
    cout << endl;
    cout << endl;
  }
}

END_NAMESPACE_YM_RC
