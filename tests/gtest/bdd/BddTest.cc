
/// @file BddTest.cc
/// @brief Bdd/BddMgr のユニットテスト用テストフィクスチャ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"
#include "BddTest.h"


BEGIN_NAMESPACE_YM

void
BddTest::SetUp()
{
}

void
BddTest::TearDown()
{
}

void
BddTest::check(
  const Bdd& bdd,
  const char* exp_str
)
{
  SizeType n = strlen(exp_str);
  SizeType ni = 0;
  while ( (1 << ni) < n ) {
    ++ ni;
  }
  ASSERT_EQ( n, (1 << ni) );
  vector<bool> inputs(ni);
  for ( SizeType p = 0; p < n; ++ p ) {
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( p & (1 << (ni - i - 1)) ) {
	inputs[i] = true;
      }
      else {
	inputs[i] = false;
      }
    }
    bool val = bdd.eval(inputs);
    bool exp_val;
    if ( exp_str[n - p - 1] == '0' ) {
      exp_val = false;
    }
    else {
      exp_val = true;
    }
    ostringstream buf;
    buf << "(";
    for ( SizeType i = 0; i < ni; ++ i ) {
      buf << " " << inputs[i];
    }
    buf << ")";
    EXPECT_EQ( exp_val, val ) << buf.str();
  }

  auto str = bdd.to_truth(ni);
  EXPECT_EQ( exp_str, str );
}

void
BddTest::test_check_sym(
  const Bdd& bdd,
  VarId var1,
  VarId var2
)
{
  auto lit1 = mMgr.literal(var1);
  auto lit2 = mMgr.literal(var2);

  auto cube00 = ~lit1 & ~lit2;
  auto bdd00 = bdd / cube00;
  auto bdd01 = bdd / (~lit1 & lit2);
  auto bdd10 = bdd / (lit1 & ~lit2);
  auto bdd11 = bdd / (lit1 & lit2);

  ostringstream buf;
  buf << ", where var1 = " << var1 << ", var2 = " << var2;
  bool exp1 = bdd01 == bdd10;
  EXPECT_EQ( exp1, bdd.check_sym(var1, var2, false) ) << buf.str();

  bool exp2 = bdd00 == bdd11;
  EXPECT_EQ( exp2, bdd.check_sym(var1, var2, true) ) << buf.str();
}

void
BddTest::test_check_sup(
  const Bdd& bdd,
  VarId var
)
{
  auto lit1 = mMgr.literal(var);
  auto bdd0 = bdd / ~lit1;
  auto bdd1 = bdd / lit1;

  ostringstream buf;
  buf << ", where var = " << var;
  bool exp = bdd0 != bdd1;
  EXPECT_EQ( exp, bdd.check_sup(var) ) << buf.str();
}

/// @brief サポートのテストを行う．
void
BddTest::test_support(
  const Bdd& bdd,
  const BddVarSet& sup,
  SizeType max_v
)
{
  auto var_list = sup.to_varlist();
  vector<bool> sup_mark(max_v, false);
  for ( auto var: var_list ) {
    sup_mark[var.val()] = true;
  }

  for ( SizeType i = 0; i < max_v; ++ i ) {
    VarId var{i};
    ostringstream buf;
    buf << ", where var = " << var;
    EXPECT_EQ( sup_mark[i], bdd.check_sup(var) ) << buf.str();
  }
}

// @brief onepathのテストを行う．
void
BddTest::test_onepath(
  const Bdd& bdd,
  const Bdd& path
)
{
  if ( bdd.is_zero() ) {
    EXPECT_TRUE( path.is_zero() );
    return;
  }

  EXPECT_TRUE( path.is_cube() );

  auto bdd1 = bdd / path;
  EXPECT_TRUE( bdd1.is_one() );
}

// @brief zeropathのテストを行う．
void
BddTest::test_zeropath(
  const Bdd& bdd,
  const Bdd& path
)
{
  if ( bdd.is_one() ) {
    EXPECT_TRUE( path.is_zero() );
    return;
  }

  EXPECT_TRUE( path.is_cube() );

  auto bdd1 = bdd / path;
  EXPECT_TRUE( bdd1.is_zero() );
}

END_NAMESPACE_YM
