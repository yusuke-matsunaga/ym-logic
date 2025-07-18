
/// @file KernelTest.cc
/// @brief KernelTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "KernelGen.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_SOP

class KernelTest:
public ::testing::Test
{
public:

  SizeType nv{10};
  SizeType var0{0};
  SizeType var1{1};
  SizeType var2{2};
  SizeType var3{3};
  SizeType var4{4};
  SizeType var5{5};
  SizeType var6{6};
  SizeType var7{7};
  SizeType var8{8};
  SizeType var9{9};

  Literal v0{var0, false};
  Literal v1{var1, false};
  Literal v2{var2, false};
  Literal v3{var3, false};
  Literal v4{var4, false};
  Literal v5{var5, false};
  Literal v6{var6, false};
  Literal v7{var7, false};
  Literal v8{var8, false};
  Literal v9{var9, false};

  void
  check(
    const SopCover& cover,
    const std::vector<std::string>& exp_list
  )
  {
    KernelGen kg;
    auto kernel_list = kg.all_kernels(cover);

    auto n = kernel_list.size();
    EXPECT_EQ( exp_list.size(), n );
    for ( SizeType i: Range(n) ) {
      auto& ki = kernel_list[i];
      auto& kernel = ki.first;
      auto& cokernels = ki.second;
      std::ostringstream tmp;
      tmp << kernel << "| ";
      const char* delim = "";
      for ( auto& cube: cokernels ) {
	tmp << delim << cube;
	delim = ", ";
      }
      auto tmp_str = tmp.str();
      EXPECT_EQ( exp_list[i], tmp_str );
    }
  }

};

TEST_F(KernelTest, test1)
{
  auto cover1 = SopCover{nv, { {v0, v2},
			       {v1, v2} }};

  std::vector<std::string> exp_list{
    "v0 + v1| v2"
  };

  check(cover1, exp_list);
}

TEST_F(KernelTest, test2)
{
  auto cover1 = SopCover{nv, { {v0, v2},
			       {v1, v2},
			       {v0, v3},
			       {v1, v3} }};

  std::vector<std::string> exp_list{
    "v2 + v3| v0, v1",
    "v0 + v1| v2, v3",
    "v0 v2 + v0 v3 + v1 v2 + v1 v3| {}"
  };

  check(cover1, exp_list);
}

TEST_F(KernelTest, test3)
{
  // adf + aef + bdf + bef + cdf + cef + bfg + h
  auto cover1 = SopCover{nv, { { v0, v3, v5 },
			       { v0, v4, v5 },
			       { v1, v3, v5 },
			       { v1, v4, v5 },
			       { v2, v3, v5 },
			       { v2, v4, v5 },
			       { v1, v5, v6 },
			       { v7 } } };

  std::vector<std::string> exp_list{
    "v3 + v4| v0 v5, v2 v5",
    "v3 + v4 + v6| v1 v5",
    "v0 + v1 + v2| v3 v5, v4 v5",
    "v0 v3 + v0 v4 + v1 v3 + v1 v4 + v1 v6 + v2 v3 + v2 v4| v5",
    "v0 v3 v5 + v0 v4 v5 + v1 v3 v5 + v1 v4 v5 + v1 v5 v6 + v2 v3 v5 + v2 v4 v5 + v7| {}"
  };

  check(cover1, exp_list);
}

TEST_F(KernelTest, test4)
{
  // af + bf + ag + cg + ade + bde + cde
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
  std::vector<std::string> exp_list{
    "v3 v4 + v6| v2",
    "v3 v4 + v5| v1",
    "v3 v4 + v5 + v6| v0",
    "v0 + v2| v6",
    "v0 + v1| v5",
    "v0 + v1 + v2| v3 v4",
    "v0 v3 v4 + v0 v5 + v0 v6 + v1 v3 v4 + v1 v5 + v2 v3 v4 + v2 v6| {}"
  };

  check(F, exp_list);
}

END_NAMESPACE_YM_SOP
