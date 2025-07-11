
/// @file BoolDivisionTest.cc
/// @brief BoolDivisin のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "BoolDivision.h"


BEGIN_NAMESPACE_YM_SOP

class BoolDivisionTest:
  public ::testing::Test
{
public:

  Literal lit0{0, false};
  Literal lit1{1, false};
  Literal lit2{2, false};
  Literal lit3{3, false};
  Literal lit4{4, false};
  Literal lit5{5, false};
  Literal lit6{6, false};
  Literal lit7{7, false};
  Literal lit8{8, false};
  Literal lit9{9, false};

};

TEST_F(BoolDivisionTest, test1)
{
  auto cover1 = SopCover{4, { {lit0, lit2},
			      {lit0, lit3},
			      {lit1, lit2},
			      {lit1, lit3} }};
  auto cover2 = SopCover{4, { {lit2}, {lit3} }};

  auto p = BoolDivision::divide(cover1, cover2);

  auto q = p.first;
  auto r = p.second;

  std::cout << "q: ";
  q.print(std::cout);
  std::cout << std::endl;

  std::cout << "r: ";
  r.print(std::cout);
  std::cout << std::endl;
}

TEST_F(BoolDivisionTest, test2)
{
  auto cover1 = SopCover{3, { {lit0, ~lit1},
			      {lit0, ~lit2},
			      {lit1, ~lit0},
			      {lit1, ~lit2},
			      {lit2, ~lit0},
			      {lit2, ~lit1} }};
  auto cover2 = SopCover{3, { {lit0}, {lit1}, {lit2} }};

  auto p = BoolDivision::divide(cover1, cover2);

  auto q = p.first;
  auto r = p.second;

  std::cout << "q: ";
  q.print(std::cout);
  std::cout << std::endl;

  std::cout << "r: ";
  r.print(std::cout);
  std::cout << std::endl;
}

END_NAMESPACE_YM_SOP
