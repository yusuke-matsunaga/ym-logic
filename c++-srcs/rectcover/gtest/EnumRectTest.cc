
/// @file EnumRectTest.cc
/// @brief EnumRectTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/KIX.h"
#include "CkMatrix.h"
#include "EnumRect.h"


BEGIN_NAMESPACE_YM_RC

TEST(KIXTest, test1)
{
  std::vector<KIX::CoverInfo> cover_list;
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
    auto inputs_F = std::vector<SizeType>{0, 1, 2, 3, 4, 5, 6};
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
    auto inputs_G = std::vector<SizeType>{0, 1, 2, 4, 5};
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
    auto inputs_H = std::vector<SizeType>{0, 2, 3, 4};
    cover_list.push_back(KIX::CoverInfo{H, inputs_H});
  }

  for ( auto& cover_input: cover_list ) {
    std::cout << cover_input.cover << std::endl;
    for ( auto input: cover_input.inputs ) {
      std::cout << " " << input;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  // co-kernel/kernel cube matrix を作る．
  auto matrix = CkMatrix(cover_list);

  auto rect_list = matrix.enum_max_rects();

  if ( rect_list.empty() ) {
    std::cout << "rect_list is empty()" << std::endl;
  }
  else {
    for ( auto rect: rect_list ) {
      std::cout << rect << std::endl;
    }
  }
}

END_NAMESPACE_YM_RC
