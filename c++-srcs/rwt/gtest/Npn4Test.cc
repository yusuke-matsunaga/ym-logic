
/// @file Npn4Test.cc
/// @brief Npn4Test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "Npn4.h"


BEGIN_NAMESPACE_YM_AIG

class Npn4Test:
public ::testing::Test
{
public:
  // 順列のリスト
  std::vector<std::vector<SizeType>> iperm_list = {
    {0, 1, 2, 3},
    {0, 1, 3, 2},
    {0, 2, 1, 3},
    {0, 2, 3, 1},
    {0, 3, 1, 2},
    {0, 3, 2, 1},
    {1, 0, 2, 3},
    {1, 0, 3, 2},
    {1, 2, 0, 3},
    {1, 2, 3, 0},
    {1, 3, 0, 2},
    {1, 3, 2, 0},
    {2, 0, 1, 3},
    {2, 0, 3, 1},
    {2, 1, 0, 3},
    {2, 1, 3, 0},
    {2, 3, 0, 1},
    {2, 3, 1, 0},
    {3, 0, 1, 2},
    {3, 0, 2, 1},
    {3, 1, 0, 2},
    {3, 1, 2, 0},
    {3, 2, 0, 1},
    {3, 2, 1, 0}
  };

};

TEST_F(Npn4Test, empty_constr)
{
  auto npn = Npn4();

  EXPECT_FALSE( npn.oinv() );
  EXPECT_FALSE( npn.iinv(0) );
  EXPECT_FALSE( npn.iinv(1) );
  EXPECT_FALSE( npn.iinv(2) );
  EXPECT_FALSE( npn.iinv(3) );
  EXPECT_EQ( 0, npn.iperm(0) );
  EXPECT_EQ( 1, npn.iperm(1) );
  EXPECT_EQ( 2, npn.iperm(2) );
  EXPECT_EQ( 3, npn.iperm(3) );
}

TEST_F(Npn4Test, constr1)
{
  for ( SizeType b = 0; b < 32; ++ b ) {
    auto oinv = static_cast<bool>((b >> 4) & 1);
    std::vector<bool> iinv(4);
    for ( SizeType i = 0; i < 4; ++ i ) {
      iinv[i] = static_cast<bool>((b >> i) & 1);
    }
    for ( auto& iperm: iperm_list ) {
      auto npn = Npn4(oinv, iinv, iperm);

      EXPECT_EQ( oinv, npn.oinv() );
      EXPECT_EQ( iinv[0], npn.iinv(0) );
      EXPECT_EQ( iinv[1], npn.iinv(1) );
      EXPECT_EQ( iinv[2], npn.iinv(2) );
      EXPECT_EQ( iinv[3], npn.iinv(3) );
      EXPECT_EQ( iperm[0], npn.iperm(0) );
      EXPECT_EQ( iperm[1], npn.iperm(1) );
      EXPECT_EQ( iperm[2], npn.iperm(2) );
      EXPECT_EQ( iperm[3], npn.iperm(3) );

      auto npn2 = Npn4(oinv, iinv, iperm);
      EXPECT_EQ( npn2, npn );
    }
  }
}

TEST_F(Npn4Test, identity)
{
  auto npn = Npn4::identity();

  EXPECT_FALSE( npn.oinv() );
  EXPECT_FALSE( npn.iinv(0) );
  EXPECT_FALSE( npn.iinv(1) );
  EXPECT_FALSE( npn.iinv(2) );
  EXPECT_FALSE( npn.iinv(3) );
  EXPECT_EQ( 0, npn.iperm(0) );
  EXPECT_EQ( 1, npn.iperm(1) );
  EXPECT_EQ( 2, npn.iperm(2) );
  EXPECT_EQ( 3, npn.iperm(3) );

  auto npn2 = Npn4();
  EXPECT_EQ( npn2, npn );
}

Npn4::Tv4
xform(
  const Npn4& npn,
  Npn4::Tv4 tv
)
{
  Npn4::Tv4 xtv = 0;
  for ( int b = 0; b < 16; ++ b ) {
    auto obit = static_cast<bool>((tv >> b) & 1);
    if ( obit == npn.oinv() ) {
      continue;
    }
    int xb = 0;
    for ( SizeType i = 0; i < 4; ++ i ) {
      auto ibit = static_cast<bool>((b >> i) & 1);
      if ( ibit != npn.iinv(i) ) {
	xb |= (1 << npn.iperm(i));
      }
    }
    xtv |= (1 << xb);
  }
  return xtv;
}

TEST_F(Npn4Test, xform)
{
  for ( SizeType b = 0; b < 32; ++ b ) {
    auto oinv = static_cast<bool>((b >> 4) & 1);
    std::vector<bool> iinv(4);
    for ( SizeType i = 0; i < 4; ++ i ) {
      iinv[i] = static_cast<bool>((b >> i) & 1);
    }
    for ( auto& iperm: iperm_list ) {
      auto npn = Npn4(oinv, iinv, iperm);

      for ( SizeType tv = 0; tv < 0x10000; ++ tv ) {
	auto xtv = npn.xform(tv);
	auto xtv2 = xform(npn, tv);
	std::ostringstream buf;
	buf << "npn = " << npn
	    << ", tv = "
	    << std::hex << std::setw(4) << std::setfill('0') << tv
	    << std::dec << std::setfill(' ');
	EXPECT_EQ( xtv2, xtv ) << buf.str();
      }
    }
  }
}

TEST_F(Npn4Test, inv)
{
  for ( SizeType b = 0; b < 32; ++ b ) {
    auto oinv = static_cast<bool>((b >> 4) & 1);
    std::vector<bool> iinv(4);
    for ( SizeType i = 0; i < 4; ++ i ) {
      iinv[i] = static_cast<bool>((b >> i) & 1);
    }
    for ( auto& iperm: iperm_list ) {
      auto npn = Npn4(oinv, iinv, iperm);
      auto inv_npn = ~npn;

      EXPECT_EQ( npn.oinv(), inv_npn.oinv() );
      for ( SizeType i = 0; i < 4; ++ i ) {
	auto i2 = npn.iperm(i);
	EXPECT_EQ( npn.iinv(i), inv_npn.iinv(i2) );
	EXPECT_EQ( i, inv_npn.iperm(i2) );
      }
      auto npn2 = npn * inv_npn;
      std::ostringstream buf1;
      buf1 << npn << " * " << inv_npn << " = " << npn2;
      EXPECT_EQ( Npn4::identity(), npn2 ) << buf1.str();

      auto npn3 = inv_npn * npn;
      std::ostringstream buf2;
      buf2 << inv_npn << " * " << npn << " = " << npn3;
      EXPECT_EQ( Npn4::identity(), npn3 ) << buf2.str();
    }
  }
}

BEGIN_NONAMESPACE

Npn4
compose(
  const Npn4& npn1,
  const Npn4& npn2
)
{
  auto oinv = npn1.oinv() != npn2.oinv();
  std::vector<bool> iinv(4);
  std::vector<SizeType> iperm(4);
  for ( SizeType i = 0; i < 4; ++ i ) {
    auto i2 = npn1.iperm(i);
    iinv[i] = npn1.iinv(i) != npn2.iinv(i2);
    iperm[i] = npn2.iperm(i2);
  }
  return Npn4(oinv, iinv, iperm);
}

END_NONAMESPACE

TEST_F(Npn4Test, compose)
{
  for ( SizeType b = 0; b < 32; ++ b ) {
    auto oinv = static_cast<bool>((b >> 4) & 1);
    std::vector<bool> iinv(4);
    for ( SizeType i = 0; i < 4; ++ i ) {
      iinv[i] = static_cast<bool>((b >> i) & 1);
    }
    for ( auto& iperm: iperm_list ) {
      auto npn = Npn4(oinv, iinv, iperm);

      for ( SizeType b2 = 0; b2 < 32; ++ b2 ) {
	auto oinv2 = static_cast<bool>((b2 >> 4) & 1);
	std::vector<bool> iinv2(4);
	for ( SizeType i2 = 0; i2 < 4; ++ i2 ) {
	  iinv2[i2] = static_cast<bool>((b2 >> i2) & 1);
	}
	for ( auto& iperm2: iperm_list ) {
	  auto npn2 = Npn4(oinv2, iinv2, iperm2);
	  auto npn3 = npn * npn2;
	  auto npn4 = compose(npn, npn2);
	  std::ostringstream buf;
	  buf << npn << " * " << npn2 << " = " << npn3;
	  EXPECT_EQ( npn4, npn3 ) << buf.str();
	}
      }
    }
  }
}

TEST_F(Npn4Test, normalize)
{
  std::unordered_set<Npn4::Tv4> rep_funcs;
  for ( SizeType tv = 0; tv < 0x10000; ++ tv ) {
    Npn4 npn;
    auto ctv = Npn4::normalize(tv, npn);
    if ( rep_funcs.count(ctv) == 0 ) {
      rep_funcs.emplace(ctv);
    }
    auto tv1 = npn.xform(ctv);
    std::ostringstream buf;
    buf << hex << setw(4) << setfill('0')
	<< tv << "| "
	<< setw(4) << ctv
	<< dec << setfill(' ')
	<< ", " << npn
	<< " => "
	<< hex << setw(4) << setfill('0')
	<< tv1 << setfill(' ') << dec;
    EXPECT_EQ( tv1, tv ) << buf.str();
  }
  EXPECT_EQ( 222, rep_funcs.size() );
}

END_NAMESPACE_YM_AIG
