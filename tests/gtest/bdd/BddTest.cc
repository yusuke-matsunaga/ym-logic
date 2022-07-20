
/// @file BddTest.cc
/// @brief Bdd のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

class BddTest :
public ::testing::Test
{
public:

  /// @brief BDDの内容を調べる．
  void
  check(
    const Bdd& bdd,     ///< [in] 対象のBDD
    const char* exp_str ///< [in] 真理値表の期待値
  );

  /// @brief リテラル関数を返す．
  Bdd
  literal(
    SizeType index ///< [in] インデックス
  )
  {
    return mMgr.literal(VarId{index});
  }


protected:

  void
  SetUp() override;

  void
  TearDown() override;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgr mMgr;

};

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


TEST_F(BddTest, invalid1)
{
  // 不正値になっているはず．
  Bdd bdd;

  EXPECT_TRUE( bdd.is_invalid() );
  EXPECT_FALSE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST_F(BddTest, invalid2)
{
  // 不正値になっているはず．
  Bdd bdd = Bdd::invalid();

  EXPECT_TRUE( bdd.is_invalid() );
  EXPECT_FALSE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
}

TEST_F(BddTest, zero)
{
  Bdd bdd = mMgr.zero();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_TRUE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "0");
}

TEST_F(BddTest, one)
{
  Bdd bdd = mMgr.one();

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_TRUE( bdd.is_one() );

  check(bdd, "1");
}

TEST_F(BddTest, lit1)
{
  VarId var{0};
  Bdd bdd = mMgr.literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, lit2)
{
  VarId var{0};
  Bdd bdd = mMgr.literal(var, false);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, lit3)
{
  VarId var{0};
  Bdd bdd = mMgr.literal(var, true);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "01");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST_F(BddTest, lit4)
{
  VarId var{0};
  Bdd bdd = mMgr.posi_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, lit5)
{
  VarId var{0};
  Bdd bdd = mMgr.nega_literal(var);

  EXPECT_FALSE( bdd.is_invalid() );
  EXPECT_TRUE( bdd.is_valid() );
  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );

  check(bdd, "01");

  Bdd f0;
  Bdd f1;
  VarId var1 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var1, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST_F(BddTest, and1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 & var2;

  check(bdd, "1000");
}

TEST_F(BddTest, or1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 | var2;

  check(bdd, "1110");
}

TEST_F(BddTest, xor1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd bdd = var1 ^ var2;

  check(bdd, "0110");
}

TEST_F(BddTest, func1)
{
  Bdd var1 = literal(0);
  Bdd var2 = literal(1);
  Bdd var3 = literal(2);
  Bdd bdd = (var1 & ~var2) | var3;

  check(bdd, "10111010");
}

TEST_F(BddTest, from_truth1)
{
  const char* exp_str = "10010110";
  Bdd bdd = mMgr.from_truth(exp_str);
  check(bdd, exp_str);
}

TEST_F(BddTest, from_truth2)
{
  const char* exp_str = "10010101";
  Bdd bdd = mMgr.from_truth(exp_str);
  check(bdd, exp_str);
}

END_NAMESPACE_YM
