#ifndef SOPTEST_H
#define SOPTEST_H

/// @file SopTest.h
/// @brief SopTest 用のテストフィクスチャ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

class SopTest :
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

  Literal lit0{var0, false};
  Literal lit1{var1, false};
  Literal lit2{var2, false};
  Literal lit3{var3, false};
  Literal lit4{var4, false};
  Literal lit5{var5, false};
  Literal lit6{var6, false};
  Literal lit7{var7, false};
  Literal lit8{var8, false};
  Literal lit9{var9, false};

  Literal lit100{100, false};
};

END_NAMESPACE_YM

#endif // SOPTEST_H
