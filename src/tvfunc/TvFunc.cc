﻿
/// @file TvFunc.cc
/// @brief TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/TvFunc.h"
#include "ym/NpnMap.h"


// 1 ワード当たりの入力数
#define NIPW 6

//#define DEBUG

BEGIN_NONAMESPACE

// コファクターマスク
ymuint64 c_masks[] = {
  0xAAAAAAAAAAAAAAAA,
  0xCCCCCCCCCCCCCCCC,
  0xF0F0F0F0F0F0F0F0,
  0xFF00FF00FF00FF00,
  0xFFFF0000FFFF0000,
  0xFFFFFFFF00000000
};

// 対称性を調べるためのテーブルその1
// 同位相の時に用いる．
ymuint64 sym_masks2[] = {
  0x2222222222222222, // (1, 0)
  0x0A0A0A0A0A0A0A0A, // (2, 0)
  0x0C0C0C0C0C0C0C0C, // (2, 1)
  0x00AA00AA00AA00AA, // (3, 0)
  0x00CC00CC00CC00CC, // (3, 1)
  0x00F000F000F000F0, // (3, 2)
  0x0000AAAA0000AAAA, // (4, 0)
  0x0000CCCC0000CCCC, // (4, 1)
  0x0000F0F00000F0F0, // (4, 2)
  0x0000FF000000FF00, // (4, 3)
  0x00000000AAAAAAAA, // (5, 0)
  0x00000000CCCCCCCC, // (5, 1)
  0x00000000F0F0F0F0, // (5, 2)
  0x00000000FF00FF00, // (5, 3)
  0x00000000FFFF0000  // (5, 4)
};

// 対称性を調べるためのテーブルその2
// 逆位相の時に用いる．
ymuint64 sym_masks3[] = {
  0x1111111111111111, // (1, 0)
  0x0505050505050505, // (2, 0)
  0x0303030303030303, // (2, 1)
  0x0055005500550055, // (3, 0)
  0x0033003300330033, // (3, 1)
  0x000F000F000F000F, // (3, 2)
  0x0000555500005555, // (4, 0)
  0x0000333300003333, // (4, 1)
  0x00000F0F00000F0F, // (4, 2)
  0x000000FF000000FF, // (4, 3)
  0x0000000055555555, // (5, 0)
  0x0000000033333333, // (5, 1)
  0x000000000F0F0F0F, // (5, 2)
  0x0000000000FF00FF, // (5, 3)
  0x000000000000FFFF  // (5, 4)
};

ymuint s_plist[] = {
  0,
  1,  2,  4,  8, 16, 32,
  3,  5,  6,  9, 10, 12, 17, 18, 20, 24, 33, 34, 36, 40, 48,
  7, 11, 13, 14, 19, 21, 22, 25, 26, 28, 35, 37, 38, 41, 42, 44, 49, 50, 52, 56,
  15, 23, 27, 29, 30, 39, 43, 45, 46, 51, 53, 54, 57, 58, 60,
  31, 47, 55, 59, 61, 62,
  63
};

ymuint s_pidx[] = {
  0, 1, 7, 22, 42, 57, 63, 64
};


#include "w_table.h"

END_NONAMESPACE

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

// 入力数のみ指定したコンストラクタ
// 中身は恒偽関数
TvFunc::TvFunc(ymuint ni) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new ymuint64[mBlockNum])
{
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    mVector[i] = 0UL;
  }
}

// 恒真関数を作るコンストラクタ
// 2番目の引数はダミー
TvFunc::TvFunc(ymuint ni,
	       int dummy) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new ymuint64[mBlockNum])
{
  switch ( mInputNum ) {
  case 0:
    mVector[0] = 0x1;
    break;

  case 1:
    mVector[0] = 0x3;
    break;

  case 2:
    mVector[0] = 0xF;
    break;

  case 3:
    mVector[0] = 0xFF;
    break;

  case 4:
    mVector[0] = 0xFFFF;
    break;

  case 5:
    mVector[0] = 0xFFFFFFFF;
    break;

  case 6:
    mVector[0] = 0xFFFFFFFFFFFFFFFF;
    break;

  default:
    for (ymuint i = 0; i < mBlockNum; ++ i) {
      mVector[i] = ~(0UL);
    }
  }
}

// リテラル関数を作るコンストラクタ
TvFunc::TvFunc(ymuint ni,
	       VarId varid,
	       bool inv) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new ymuint64[mBlockNum])
{
  ASSERT_COND( varid.val() < ni );
  switch ( ni ) {
  case 1:
    if ( inv ) {
      mVector[0] = 0x1;
    }
    else {
      mVector[0] = 0x2;
    }
    break;

  case 2:
    switch ( varid.val() ) {
    case 0:
      if ( inv ) {
	mVector[0] = 0x5;
      }
      else {
	mVector[0] = 0xA;
      }
      break;

    case 1:
      if ( inv ) {
	mVector[0] = 0x3;
      }
      else {
	mVector[0] = 0xC;
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case 3:
    switch ( varid.val() ) {
    case 0:
      if ( inv ) {
	mVector[0] = 0x55;
      }
      else {
	mVector[0] = 0xAA;
      }
      break;

    case 1:
      if ( inv ) {
	mVector[0] = 0x33;
      }
      else {
	mVector[0] = 0xCC;
      }
      break;

    case 2:
      if ( inv ) {
	mVector[0] = 0x0F;
      }
      else {
	mVector[0] = 0xF0;
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case 4:
    switch ( varid.val() ) {
    case 0:
      if ( inv ) {
	mVector[0] = 0x5555;
      }
      else {
	mVector[0] = 0xAAAA;
      }
      break;

    case 1:
      if ( inv ) {
	mVector[0] = 0x3333;
      }
      else {
	mVector[0] = 0xCCCC;
      }
      break;

    case 2:
      if ( inv ) {
	mVector[0] = 0x0F0F;
      }
      else {
	mVector[0] = 0xF0F0;
      }
      break;

    case 3:
      if ( inv ) {
	mVector[0] = 0x00FF;
      }
      else {
	mVector[0] = 0xFF00;
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case 5:
    switch ( varid.val() ) {
    case 0:
      if ( inv ) {
	mVector[0] = 0x55555555;
      }
      else {
	mVector[0] = 0xAAAAAAAA;
      }
      break;

    case 1:
      if ( inv ) {
	mVector[0] = 0x33333333;
      }
      else {
	mVector[0] = 0xCCCCCCCC;
      }
      break;

    case 2:
      if ( inv ) {
	mVector[0] = 0x0F0F0F0F;
      }
      else {
	mVector[0] = 0xF0F0F0F0;
      }
      break;

    case 3:
      if ( inv ) {
	mVector[0] = 0x00FF00FF;
      }
      else {
	mVector[0] = 0xFF00FF00;
      }
      break;

    case 4:
      if ( inv ) {
	mVector[0] = 0x0000FFFF;
      }
      else {
	mVector[0] = 0xFFFF0000;
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case 6:
    switch ( varid.val() ) {
    case 0:
      if ( inv ) {
	mVector[0] = 0x5555555555555555;
      }
      else {
	mVector[0] = 0xAAAAAAAAAAAAAAAA;
      }
      break;

    case 1:
      if ( inv ) {
	mVector[0] = 0x3333333333333333;
      }
      else {
	mVector[0] = 0xCCCCCCCCCCCCCCCC;
      }
      break;

    case 2:
      if ( inv ) {
	mVector[0] = 0x0F0F0F0F0F0F0F0F;
      }
      else {
	mVector[0] = 0xF0F0F0F0F0F0F0F0;
      }
      break;

    case 3:
      if ( inv ) {
	mVector[0] = 0x00FF00FF00FF00FF;
      }
      else {
	mVector[0] = 0xFF00FF00FF00FF00;
      }
      break;

    case 4:
      if ( inv ) {
	mVector[0] = 0x0000FFFF0000FFFF;
      }
      else {
	mVector[0] = 0xFFFF0000FFFF0000;
      }
      break;

    case 5:
      if ( inv ) {
	mVector[0] = 0x00000000FFFFFFFF;
      }
      else {
	mVector[0] = 0xFFFFFFFF00000000;
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  default:
    if ( varid.val() < NIPW ) {
      ymuint64 pat = c_masks[varid.val()];
      if ( inv ) {
	pat = ~pat;
      }
      for (ymuint b = 0; b < mBlockNum; ++ b) {
	mVector[b] = pat;
      }
    }
    else {
      ymuint i5 = varid.val() - NIPW;
      ymuint check = 1U << i5;
      ymuint64 pat0;
      if ( inv ) {
	pat0 = 0UL;
      }
      else {
	pat0 = ~(0UL);
      }
      ymuint64 pat1 = ~pat0;
      for (ymuint b = 0; b < mBlockNum; ++ b) {
	if ( b & check ) {
	  mVector[b] = pat0;
	}
	else {
	  mVector[b] = pat1;
	}
      }
    }
    break;
  }
}

// 入力数と真理値を指定したコンストラクタ
TvFunc::TvFunc(ymuint ni,
	       const vector<int>& values) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new ymuint64[mBlockNum])
{
  ymuint ni_pow = 1U << ni;
  ASSERT_COND(values.size() == ni_pow );
  if ( ni <= NIPW ) {
    ymuint64 pat = 0UL;
    for (ymuint i = 0; i < ni_pow; ++ i) {
      if ( values[i] ) {
	pat |= 1UL << i;
      }
    }
    mVector[0] = pat;
  }
  else {
    ymuint nipb_pow = 1U << NIPW;
    ymuint base = 0;
    for (ymuint i = 0; i < mBlockNum; ++ i, base += nipb_pow) {
      ymuint64 pat = 0UL;
      for (ymuint j = 0; j < nipb_pow; ++ j) {
	if ( values[base + j] ) {
	  pat |= 1UL << j;
	}
      }
      mVector[i] = pat;
    }
  }
}

// コピーコンストラクタ
TvFunc::TvFunc(const TvFunc& src) :
  mInputNum(src.mInputNum),
  mBlockNum(src.mBlockNum),
  mVector(new ymuint64[mBlockNum])
{
  for (ymuint b = 0; b < mBlockNum; ++ b) {
    mVector[b] = src.mVector[b];
  }
}

// 代入演算子
const TvFunc&
TvFunc::operator=(const TvFunc& src)
{
  if ( mBlockNum != src.mBlockNum ) {
    delete [] mVector;
    mBlockNum = src.mBlockNum;
    mVector = new ymuint64[mBlockNum];
  }
  mInputNum = src.mInputNum;

  for (ymuint b = 0; b < mBlockNum; ++ b) {
    mVector[b] = src.mVector[b];
  }

  return *this;
}

// デストラクタ
TvFunc::~TvFunc()
{
  delete [] mVector;
}

// 恒偽関数を作る．
TvFunc
TvFunc::const_zero(ymuint ni)
{
  return TvFunc(ni);
}

// 恒真関数を作る．
TvFunc
TvFunc::const_one(ymuint ni)
{
  return TvFunc(ni, 0);
}

// 肯定のリテラル関数を作る．
TvFunc
TvFunc::posi_literal(ymuint ni,
		     VarId varid)
{
  return TvFunc(ni, varid, false);
}

// 否定のリテラル関数を作る．
TvFunc
TvFunc::nega_literal(ymuint ni,
		     VarId varid)
{
  return TvFunc(ni, varid, true);
}

// 自分自身を否定する．
const TvFunc&
TvFunc::negate()
{
  switch ( mInputNum ) {
  case 0:
    mVector[0] ^= 0x1;
    break;

  case 1:
    mVector[0] ^= 0x3;
    break;

  case 2:
    mVector[0] ^= 0xF;
    break;

  case 3:
    mVector[0] ^= 0xFF;
    break;

  case 4:
    mVector[0] ^= 0xFFFF;
    break;

  case 5:
    mVector[0] ^= 0xFFFFFFFF;
    break;

  case 6:
    mVector[0] ^= 0xFFFFFFFFFFFFFFFF;
    break;

  default:
    for (ymuint b = 0; b < mBlockNum; ++ b) {
      mVector[b] ^= ~(0UL);
    }
    break;
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
const TvFunc&
TvFunc::operator&=(const TvFunc& src1)
{
  for (ymuint b = 0; b < mBlockNum; ++ b) {
    mVector[b] &= src1.mVector[b];
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
const TvFunc&
TvFunc::operator|=(const TvFunc& src1)
{
  for (ymuint b = 0; b < mBlockNum; ++ b) {
    mVector[b] |= src1.mVector[b];
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
const TvFunc&
TvFunc::operator^=(const TvFunc& src1)
{
  for (ymuint b = 0; b < mBlockNum; ++ b) {
    mVector[b] ^= src1.mVector[b];
  }
  return *this;
}

// @brief コファクターを計算し自分に代入する．
// @param[in] varid 変数番号
// @param[in] pol 極性
// @return 自身への参照を返す．
const TvFunc&
TvFunc::set_cofactor(VarId varid,
		     bool inv)
{
  ymuint pos = varid.val();
  if ( pos < NIPW ) {
    ymuint64 mask = c_masks[pos];
    if ( inv ) {
      mask = ~mask;
    }
    int shift = 1 << pos;
    for (ymuint b = 0; b < mBlockNum; ++ b) {
      ymuint64 pat = mVector[b] & mask;
      if ( inv ) {
	pat |= (pat << shift);
      }
      else {
	pat |= (pat >> shift);
      }
      mVector[b] = pat;
    }
  }
  else {
    pos -= NIPW;
    ymuint bit = 1U << pos;
    for (ymuint i = 0; i < mBlockNum; ++ i) {
      if ( inv ) {
	if ( (i & bit) == bit ) {
	  mVector[i] = mVector[i ^ bit];
	}
      }
      else {
	if ( (i & bit) == 0U ) {
	  mVector[i] = mVector[i ^ bit];
	}
      }
    }
  }
  return *this;
}

BEGIN_NONAMESPACE

// word の中の 1 のビットを数える．
// 0入力用
inline
ymuint
count_onebits_0(ymuint64 word)
{
  return word & 0x1;
}

// word の中の 1 のビットを数える．
// 1入力用
inline
ymuint
count_onebits_1(ymuint64 word)
{
  const ymuint64 mask1 = 0x3;

  word = (word & mask1) + ((word >> 1) & mask1);
  return word;
}

// word の中の 1 のビットを数える．
// 2入力用
inline
ymuint
count_onebits_2(ymuint64 word)
{
  const ymuint64 mask1  = 0x5;
  const ymuint64 mask2  = 0x3;

  word = (word & mask1) + ((word >> 1) & mask1);
  word = (word & mask2) + ((word >> 2) & mask2);
  return word;
}

// word の中の 1 のビットを数える．
// 3入力用
inline
ymuint
count_onebits_3(ymuint64 word)
{
  const ymuint64 mask1  = 0x55;
  const ymuint64 mask2  = 0x33;
  const ymuint64 mask4  = 0x0f;

  word = (word & mask1) + ((word >> 1) & mask1);
  word = (word & mask2) + ((word >> 2) & mask2);
  word = (word & mask4) + ((word >> 4) & mask4);
  return word;
}

// word の中の 1 のビットを数える．
// 4入力用
inline
ymuint
count_onebits_4(ymuint64 word)
{
  const ymuint64 mask1  = 0x5555;
  const ymuint64 mask2  = 0x3333;
  const ymuint64 mask4  = 0x0f0f;
  const ymuint64 mask8  = 0x00ff;

  word = (word & mask1) + ((word >> 1) & mask1);
  word = (word & mask2) + ((word >> 2) & mask2);
  word = (word & mask4) + ((word >> 4) & mask4);
  word = (word & mask8) + ((word >> 8) & mask8);
  return word;
}

// word の中の 1 のビットを数える．
// 5入力用
inline
ymuint
count_onebits_5(ymuint64 word)
{
  const ymuint64 mask1   = 0x55555555;
  const ymuint64 mask2   = 0x33333333;
  const ymuint64 mask4   = 0x0f0f0f0f;
  const ymuint64 mask8   = 0x00ff00ff;
  const ymuint64 mask16  = 0x0000ffff;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  return word;
}

// word の中の 1 のビットを数える．
// 6入力用
inline
ymuint
count_onebits_6(ymuint64 word)
{
  const ymuint64 mask1  = 0x5555555555555555;
  const ymuint64 mask2  = 0x3333333333333333;
  const ymuint64 mask4  = 0x0f0f0f0f0f0f0f0f;
  const ymuint64 mask8  = 0x00ff00ff00ff00ff;
  const ymuint64 mask16 = 0x0000ffff0000ffff;
  const ymuint64 mask32 = 0x00000000ffffffff;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  word = (word & mask32) + ((word >> 32) & mask32);
  return word;
}

// word の中の 1 のビットを数える．
inline
ymuint
count_onebits(ymuint64 word)
{
  return count_onebits_6(word);
}

END_NONAMESPACE

// 0 の数を数える．
ymuint
TvFunc::count_zero() const
{
  switch ( input_num() ) {
  case 0: return (1 << 0) - count_onebits_0(mVector[0]);
  case 1: return (1 << 1) - count_onebits_1(mVector[0]);
  case 2: return (1 << 2) - count_onebits_2(mVector[0]);
  case 3: return (1 << 3) - count_onebits_3(mVector[0]);
  case 4: return (1 << 4) - count_onebits_4(mVector[0]);
  case 5: return (1 << 5) - count_onebits_5(mVector[0]);
  case 6: return (1 << 6) - count_onebits_6(mVector[0]);

  default:
    ;
  }

  ymuint ans = 0U;
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    ans += count_onebits(mVector[i]);
  }
  return (1U << input_num()) - ans;
}

// 1 の数を数える．
ymuint
TvFunc::count_one() const
{
  switch ( input_num() ) {
  case 0: return count_onebits_0(mVector[0]);
  case 1: return count_onebits_1(mVector[0]);
  case 2: return count_onebits_2(mVector[0]);
  case 3: return count_onebits_3(mVector[0]);
  case 4: return count_onebits_4(mVector[0]);
  case 5: return count_onebits_5(mVector[0]);
  case 6: return count_onebits_6(mVector[0]);

  default:
    ;
  }

  ymuint ans = 0UL;
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    ans += count_onebits(mVector[i]);
  }
  return ans;
}

// 0次の Walsh 係数を求める．
ymint
TvFunc::walsh_0() const
{
  switch ( input_num() ) {
  case 0: return (1 << 0) - count_onebits_0(mVector[0]) * 2;
  case 1: return (1 << 1) - count_onebits_1(mVector[0]) * 2;
  case 2: return (1 << 2) - count_onebits_2(mVector[0]) * 2;
  case 3: return (1 << 3) - count_onebits_3(mVector[0]) * 2;
  case 4: return (1 << 4) - count_onebits_4(mVector[0]) * 2;
  case 5: return (1 << 5) - count_onebits_5(mVector[0]) * 2;
  case 6: return (1 << 6) - count_onebits_6(mVector[0]) * 2;

  default:
    ;
  }

  ymint ans = 0;
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    ans += count_onebits(mVector[i]);
  }
  return (1 << input_num()) - ans * 2;
}

// 1次の Walsh 係数を求める．
ymint
TvFunc::walsh_1(VarId varid) const
{
  ymuint pos = varid.val();
  switch ( input_num() ) {
  case 0: ASSERT_NOT_REACHED;
  case 1: return (1 << 1) - count_onebits_1(mVector[0] ^ c_masks[pos]) * 2;
  case 2: return (1 << 2) - count_onebits_2(mVector[0] ^ c_masks[pos]) * 2;
  case 3: return (1 << 3) - count_onebits_3(mVector[0] ^ c_masks[pos]) * 2;
  case 4: return (1 << 4) - count_onebits_4(mVector[0] ^ c_masks[pos]) * 2;
  case 5: return (1 << 5) - count_onebits_5(mVector[0] ^ c_masks[pos]) * 2;
  case 6: return (1 << 6) - count_onebits_6(mVector[0] ^ c_masks[pos]) * 2;

  default:
    ;
  }

  // n > 5
  int c = 0;
  int n = 1 << input_num();
  if ( pos < NIPW ) {
    ymuint64 mask = c_masks[pos];
    for (ymuint i = 0; i < mBlockNum; ++ i) {
      c += count_onebits(mVector[i] ^ mask);
    }
  }
  else {
    ymuint i5 = pos - NIPW;
    // ymuint check = 1 << i5;
    // for (ymuint b = 0; b < mBlockNum; ++ b) {
    //   if ( b & check ) {
    //     c += count_onebits(~mVector[b]);
    //   } else {
    //     c += count_onebits(mVector[b]);
    //   }
    // }
    for (ymuint b = 0; b < mBlockNum; ++ b) {
      ymuint64 mask = 0UL - ((b >> i5) & 1UL);
      c += count_onebits(mVector[b] ^ mask);
    }
  }
  return n - c * 2;
}

// 2次の Walsh 係数を求める．
ymint
TvFunc::walsh_2(VarId var1,
		VarId var2) const
{
  ymuint i = var1.val();
  ymuint j = var2.val();

  if ( i == j ) {
    return 0;
  }

  switch ( input_num() ) {
  case 0:
  case 1:
    ASSERT_NOT_REACHED;
    break;

  case 2:
    return (1 << 2) - count_onebits_2(mVector[0] ^ c_masks[i] ^ c_masks[j]) * 2;

  case 3:
    return (1 << 3) - count_onebits_3(mVector[0] ^ c_masks[i] ^ c_masks[j]) * 2;

  case 4:
    return (1 << 4) - count_onebits_4(mVector[0] ^ c_masks[i] ^ c_masks[j]) * 2;

  case 5:
    return (1 << 5) - count_onebits_5(mVector[0] ^ c_masks[i] ^ c_masks[j]) * 2;

  case 6:
    return (1 << 6) - count_onebits_6(mVector[0] ^ c_masks[i] ^ c_masks[j]) * 2;

  default:
    ;
  }

  // i と j を正規化する．
  if ( i < j ) {
    ymuint tmp = i;
    i = j;
    j = tmp;
  }

  ymint c = 0;
  if ( i < NIPW ) {
    ymuint64 mask = c_masks[i] ^ c_masks[j];
    for (ymuint i = 0; i < mBlockNum; ++ i) {
      c += count_onebits(mVector[i] ^ mask);
    }
  }
  else if ( j < NIPW ) {
    // ymuint check = 1 << (i - 5);
    // ymuint mask1 = c_masks[j];
    // ymuint mask0 = ~mask1;
    // for (ymuint b = 0; b < mBlockNum; ++ b) {
    //   if ( b & check ) {
    //     c += count_onebits(mVector[b] ^ mask0);
    //   } else {
    //     c += count_onebits(mVector[b] ^ mask1);
    //   }
    // }
    ymuint i5 = i - NIPW;
    ymuint64 mask = c_masks[j];
    for (ymuint i = 0; i < mBlockNum; ++ i) {
      ymuint64 mask1 = 0UL - ((i >> i5) & 1UL);
      c += count_onebits(mVector[i] ^ mask ^ mask1);
    }
  }
  else {
    // ymuint check1 = 1 << (i - 5);
    // ymuint check2 = 1 << (j - 5);
    // ymuint check = check1 | check2;
    // for (ymuint b = 0; b < mBlockNum; ++ b) {
    //   ymuint tmp = b & check;
    //   if ( tmp == check1 || tmp == check2 ) {
    //     c += count_onebits(~mVector[b]);
    //   } else {
    //     c += count_onebits(mVector[b]);
    //   }
    // }
    ymuint i5 = i - NIPW;
    ymuint j5 = j - NIPW;
    for (ymuint i = 0; i < mBlockNum; ++ i) {
      ymuint64 mask = 0UL - (((i >> i5) ^ (i >> j5)) & 1UL);
      c += count_onebits(mVector[i] ^ mask);
    }
  }
  return (1 << input_num()) - c * 2;
}

BEGIN_NONAMESPACE

// 5入力の walsh_01 用サブルーティン
inline
ymint
walsh_01_5b(ymuint64* src_vec,
	    ymint vec[])
{
  const ymuint64 mask1   = 0x55555555;
  const ymuint64 mask2   = 0x33333333;
  const ymuint64 mask4   = 0x0f0f0f0f;
  const ymuint64 mask8   = 0x00ff00ff;
  const ymuint64 mask16  = 0x0000ffff;

  ymuint64 tmp;
  {
    tmp = src_vec[0];
    ymuint64 tmp0 = tmp & mask1;
    ymuint64 tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x55555555;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[0] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask2;
    ymuint64 tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x22222222;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[1] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask4;
    ymuint64 tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x04040404;
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[2] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask8;
    ymuint64 tmp1 = (tmp >> 8) & mask8;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x00080008;
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[3] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask16;
    ymuint64 tmp1 = (tmp >> 16) & mask16;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x00000010;
    vec[4] += tmp0;
  }

  return tmp;
}

// 6 入力の walsh_01 用サブルーティン
inline
ymint
walsh_01_6b(ymuint64* src_vec,
	    ymint vec[])
{
  const ymuint64 mask1   = 0x5555555555555555;
  const ymuint64 mask2   = 0x3333333333333333;
  const ymuint64 mask4   = 0x0f0f0f0f0f0f0f0f;
  const ymuint64 mask8   = 0x00ff00ff00ff00ff;
  const ymuint64 mask16  = 0x0000ffff0000ffff;
  const ymuint64 mask32  = 0x00000000ffffffff;

  ymuint64 tmp;
  {
    tmp = src_vec[0];
    ymuint64 tmp0 = tmp & mask1;
    ymuint64 tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x5555555555555555;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >> 2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >> 4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >> 8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[0] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask2;
    ymuint64 tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x2222222222222222;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >> 4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >> 8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[1] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask4;
    ymuint64 tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0404040404040404;
    tmp0 = (tmp0 & mask8)  + ((tmp0 >> 8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[2] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask8;
    ymuint64 tmp1 = (tmp >> 8) & mask8;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0008000800080008;
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[3] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask16;
    ymuint64 tmp1 = (tmp >> 16) & mask16;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0000001000000010;
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[4] += tmp0;
  }

  {
    ymuint64 tmp0 = tmp & mask32;
    ymuint64 tmp1 = (tmp >> 32) & mask32;
    tmp =tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0000000000000020;
    vec[5] += tmp0;
  }

  return tmp;
}

// 7入力の walsh_01 用サブルーティン
inline
ymint
walsh_01_7b(ymuint64* src_vec,
	    ymint vec[])
{
  ymint ans0 = walsh_01_6b(src_vec,                     vec);
  ymint ans1 = walsh_01_6b(src_vec + (1 << (6 - NIPW)), vec);
  vec[6] += ans0 + ((1 << 6) - ans1);
  return ans0 + ans1;
}

// 8入力の walsh_01 用サブルーティン
inline
ymint
walsh_01_8b(ymuint64* src_vec,
	    ymint vec[])
{
  ymint ans0 = walsh_01_7b(src_vec,                     vec);
  ymint ans1 = walsh_01_7b(src_vec + (1 << (7 - NIPW)), vec);
  vec[7] += ans0 + ((1 << 7) - ans1);
  return ans0 + ans1;
}

// 9入力の walsh_01 用サブルーティン
inline
ymint
walsh_01_9b(ymuint64* src_vec,
	    ymint vec[])
{
  ymint ans0 = walsh_01_8b(src_vec,                     vec);
  ymint ans1 = walsh_01_8b(src_vec + (1 << (8 - NIPW)), vec);
  vec[8] += ans0 + ((1 << 8) - ans1);
  return ans0 + ans1;
}

// 10入力の walsh_01 用サブルーティン
inline
ymint
walsh_01_10b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_9b(src_vec,                     vec);
  ymint ans1 = walsh_01_9b(src_vec + (1 << (9 - NIPW)), vec);
  vec[9] += ans0 + ((1 << 9) - ans1);
  return ans0 + ans1;
}

// 11入力の walsh_01 用サブルーティン
ymint
walsh_01_11b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_10b(src_vec,                      vec);
  ymint ans1 = walsh_01_10b(src_vec + (1 << (10 - NIPW)), vec);
  vec[10] += ans0 + ((1 << 10) - ans1);
  return ans0 + ans1;
}

// 12入力の walsh_01 用サブルーティン
ymint
walsh_01_12b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_11b(src_vec,                      vec);
  ymint ans1 = walsh_01_11b(src_vec + (1 << (11 - NIPW)), vec);
  vec[11] += ans0 + ((1 << 11) - ans1);
  return ans0 + ans1;
}

// 13入力の walsh_01 用サブルーティン
ymint
walsh_01_13b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_12b(src_vec,                      vec);
  ymint ans1 = walsh_01_12b(src_vec + (1 << (12 - NIPW)), vec);
  vec[12] += ans0 + ((1 << 12) - ans1);
  return ans0 + ans1;
}

// 14入力の walsh_01 用サブルーティン
ymint
walsh_01_14b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_13b(src_vec,                      vec);
  ymint ans1 = walsh_01_13b(src_vec + (1 << (13 - NIPW)), vec);
  vec[13] += ans0 + ((1 << 13) - ans1);
  return ans0 + ans1;
}

// 15入力の walsh_01 用サブルーティン
ymint
walsh_01_15b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_14b(src_vec,                      vec);
  ymint ans1 = walsh_01_14b(src_vec + (1 << (14 - NIPW)), vec);
  vec[14] += ans0 + ((1 << 14) - ans1);
  return ans0 + ans1;
}

// 16入力の walsh_01 用サブルーティン
ymint
walsh_01_16b(ymuint64* src_vec,
	     int vec[])
{
  ymint ans0 = walsh_01_15b(src_vec,                      vec);
  ymint ans1 = walsh_01_15b(src_vec + (1 << (15 - NIPW)), vec);
  vec[15] += ans0 + ((1 << 15) - ans1);
  return ans0 + ans1;
}

// 17入力の walsh_01 用サブルーティン
ymint
walsh_01_17b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_16b(src_vec,                      vec);
  ymint ans1 = walsh_01_16b(src_vec + (1 << (16 - NIPW)), vec);
  vec[16] += ans0 + ((1 << 16) - ans1);
  return ans0 + ans1;
}

// 18入力の walsh_01 用サブルーティン
ymint
walsh_01_18b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_17b(src_vec,                      vec);
  ymint ans1 = walsh_01_17b(src_vec + (1 << (17 - NIPW)), vec);
  vec[17] += ans0 + ((1 << 17) - ans1);
  return ans0 + ans1;
}

// 19入力の walsh_01 用サブルーティン
ymint
walsh_01_19b(ymuint64* src_vec,
	     ymint vec[])
{
  ymint ans0 = walsh_01_18b(src_vec,                      vec);
  ymint ans1 = walsh_01_18b(src_vec + (1 << (18 - NIPW)), vec);
  vec[18] += ans0 + ((1 << 18) - ans1);
  return ans0 + ans1;
}

// 1入力の walsh_01 本体
inline
ymint
walsh_01_1(ymuint64* src_vec,
	   ymint vec[])
{
  const ymuint64 mask1   = 0x3;

  const ymint n = (1 << 1);

  ymuint64 tmp;
  {
    tmp = src_vec[0];
    ymuint64 tmp0 = tmp & mask1;
    ymuint64 tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x1;
    vec[0] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 2入力の walsh_01 本体
inline
ymint
walsh_01_2(ymuint64* src_vec,
	   ymint vec[])
{
  const ymuint64 mask1   = 0x5;
  const ymuint64 mask2   = 0x3;

  const ymint n = (1 << 2);

  ymuint64 tmp;
  {
    tmp = src_vec[0];
    ymuint64 tmp0 = tmp & mask1;
    ymuint64 tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x5;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    vec[0] = n - tmp0 * 2;
  }

  {
    ymuint64 tmp0 = tmp & mask2;
    ymuint64 tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x2;
    vec[1] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 3入力の walsh_01 本体
inline
ymint
walsh_01_3(ymuint64* src_vec,
	   ymint vec[])
{
  const ymuint64 mask1   = 0x55;
  const ymuint64 mask2   = 0x33;
  const ymuint64 mask4   = 0x0f;

  const ymint n = (1 << 3);

  ymuint64 tmp;
  {
    tmp = src_vec[0];
    ymuint64 tmp0 = tmp & mask1;
    ymuint64 tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x55;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    vec[0] = n - tmp0 * 2;
  }

  {
    ymuint64 tmp0 = tmp & mask2;
    ymuint64 tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x22;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    vec[1] = n - tmp0 * 2;
  }

  {
    ymuint64 tmp0 = tmp & mask4;
    ymuint64 tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x04;
    vec[2] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 4入力の walsh_01 本体
inline
ymint
walsh_01_4(ymuint64* src_vec,
	   ymint vec[])
{
  const ymuint64 mask1   = 0x5555;
  const ymuint64 mask2   = 0x3333;
  const ymuint64 mask4   = 0x0f0f;
  const ymuint64 mask8   = 0x00ff;

  const ymint n = (1 << 4);

  ymuint64 tmp;
  {
    tmp = src_vec[0];
    ymuint64 tmp0 = tmp & mask1;
    ymuint64 tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x5555;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    vec[0] = n - tmp0 * 2;
  }

  {
    ymuint64 tmp0 = tmp & mask2;
    ymuint64 tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x2222;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    vec[1] = n - tmp0 * 2;
  }

  {
    ymuint64 tmp0 = tmp & mask4;
    ymuint64 tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0404;
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    vec[2] = n - tmp0 * 2;
  }

  {
    ymuint64 tmp0 = tmp & mask8;
    ymuint64 tmp1 = (tmp >> 8) & mask8;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0008;
    vec[3] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 5入力の walsh_01 本体
inline
ymint
walsh_01_5(ymuint64* src_vec,
	   ymint vec[])
{
  for (ymuint i = 0; i < 5; ++ i) {
    vec[i] = 0;
  }

  ymint ans = walsh_01_5b(src_vec, vec);

  const ymint n = (1 << 5);
  for (ymuint i = 0; i < 5; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  return n - ans * 2;
}

// 6入力の walsh_01 本体
inline
ymint
walsh_01_6(ymuint64* src_vec,
	   ymint vec[])
{
  for (ymuint i = 0; i < 6; ++ i) {
    vec[i] = 0;
  }

  ymint ans = walsh_01_6b(src_vec, vec);

  const ymint n = (1 << 6);
  for (ymuint i = 0; i < 6; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  return n - ans * 2;
}

// 7入力の walsh_01 本体
inline
ymint
walsh_01_7(ymuint64* src_vec,
	   ymint vec[])
{
  for (ymuint i = 0; i < 6; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_6b(src_vec, vec);
  ymint ans1 = walsh_01_6b(src_vec + (1 << (6 - NIPW)), vec);

  const ymint n = (1 << 7);
  for (ymuint i = 0; i < 6; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[6] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 8入力の walsh_01 本体
inline
ymint
walsh_01_8(ymuint64* src_vec,
	   ymint vec[])
{
  for (ymuint i = 0; i < 7; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_7b(src_vec, vec);
  ymint ans1 = walsh_01_7b(src_vec + (1 << (7 - NIPW)), vec);

  const ymint n = (1 << 8);
  for (ymuint i = 0; i < 7; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[7] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 9入力の walsh_01 本体
inline
ymint
walsh_01_9(ymuint64* src_vec,
	   ymint vec[])
{
  for (ymuint i = 0; i < 8; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_8b(src_vec, vec);
  ymint ans1 = walsh_01_8b(src_vec + (1 << (8 - NIPW)), vec);

  const ymint n = (1 << 9);
  for (ymuint i = 0; i < 8; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[8] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 10入力の walsh_01 本体
inline
ymint
walsh_01_10(ymuint64* src_vec,
	    int vec[])
{
  for (ymuint i = 0; i < 9; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_9b(src_vec, vec);
  ymint ans1 = walsh_01_9b(src_vec + (1 << (9 - NIPW)), vec);

  const ymint n = (1 << 10);
  for (ymuint i = 0; i < 9; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[9] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 11入力の walsh_01 本体
ymint
walsh_01_11(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 10; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_10b(src_vec, vec);
  ymint ans1 = walsh_01_10b(src_vec + (1 << (10 - NIPW)), vec);

  const ymint n = (1 << 11);
  for (ymuint i = 0; i < 10; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[10] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 12入力の walsh_01 本体
ymint
walsh_01_12(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 11; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_11b(src_vec, vec);
  ymint ans1 = walsh_01_11b(src_vec + (1 << (11 - NIPW)), vec);

  const ymint n = (1 << 12);
  for (ymuint i = 0; i < 11; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[11] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 13入力の walsh_01 本体
ymint
walsh_01_13(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 12; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_12b(src_vec, vec);
  ymint ans1 = walsh_01_12b(src_vec + (1 << (12 - NIPW)), vec);

  const ymint n = (1 << 13);
  for (ymuint i = 0; i < 12; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[12] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 14入力の walsh_01 本体
ymint
walsh_01_14(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 13; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_13b(src_vec, vec);
  ymint ans1 = walsh_01_13b(src_vec + (1 << (13 - NIPW)), vec);

  const ymint n = (1 << 14);
  for (ymuint i = 0; i < 13; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[13] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 15入力の walsh_01 本体
ymint
walsh_01_15(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 14; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_14b(src_vec, vec);
  ymint ans1 = walsh_01_14b(src_vec + (1 << (14 - NIPW)), vec);

  const ymint n = (1 << 15);
  for (ymuint i = 0; i < 14; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[14] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 16入力の walsh_01 本体
ymint
walsh_01_16(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 15; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_15b(src_vec, vec);
  ymint ans1 = walsh_01_15b(src_vec + (1 << (15 - NIPW)), vec);

  const ymint n = (1 << 16);
  for (ymuint i = 0; i < 15; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[15] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 17入力の walsh_01 本体
ymint
walsh_01_17(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 16; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_16b(src_vec, vec);
  ymint ans1 = walsh_01_16b(src_vec + (1 << (16 - NIPW)), vec);

  const ymint n = (1 << 17);
  for (ymuint i = 0; i < 16; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[16] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 18入力の walsh_01 本体
ymint
walsh_01_18(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 17; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_17b(src_vec, vec);
  ymint ans1 = walsh_01_17b(src_vec + (1 << (17 - NIPW)), vec);

  const ymint n = (1 << 18);
  for (ymuint i = 0; i < 17; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[17] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 19入力の walsh_01 本体
ymint
walsh_01_19(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 18; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_18b(src_vec, vec);
  ymint ans1 = walsh_01_18b(src_vec + (1 << (18 - NIPW)), vec);

  const ymint n = (1 << 19);
  for (ymuint i = 0; i < 18; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[18] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 20入力の walsh_01 本体
ymint
walsh_01_20(ymuint64* src_vec,
	    ymint vec[])
{
  for (ymuint i = 0; i < 19; ++ i) {
    vec[i] = 0;
  }

  ymint ans0 = walsh_01_19b(src_vec, vec);
  ymint ans1 = walsh_01_19b(src_vec + (1 << (19 - NIPW)), vec);

  const ymint n = (1 << 20);
  for (ymuint64 i = 0; i < 19; ++ i) {
    vec[i] = n - vec[i] * 2;
  }
  vec[19] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

END_NONAMESPACE

// 0次と 1次の Walsh 係数を求める．
ymint
TvFunc::walsh_01(ymint vec[]) const
{
  switch ( input_num() ) {
  case  1: return walsh_01_1(mVector, vec);
  case  2: return walsh_01_2(mVector, vec);
  case  3: return walsh_01_3(mVector, vec);
  case  4: return walsh_01_4(mVector, vec);
  case  5: return walsh_01_5(mVector, vec);
  case  6: return walsh_01_6(mVector, vec);
  case  7: return walsh_01_7(mVector, vec);
  case  8: return walsh_01_8(mVector, vec);
  case  9: return walsh_01_9(mVector, vec);
  case 10: return walsh_01_10(mVector, vec);
  case 11: return walsh_01_11(mVector, vec);
  case 12: return walsh_01_12(mVector, vec);
  case 13: return walsh_01_13(mVector, vec);
  case 14: return walsh_01_14(mVector, vec);
  case 15: return walsh_01_15(mVector, vec);
  case 16: return walsh_01_16(mVector, vec);
  case 17: return walsh_01_17(mVector, vec);
  case 18: return walsh_01_18(mVector, vec);
  case 19: return walsh_01_19(mVector, vec);
  case 20: return walsh_01_20(mVector, vec);
  default:
    ;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

BEGIN_NONAMESPACE

inline
ymuint64
pm2_1(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask1   = 0x5;
  const ymuint64 offset1 = 0x5;
  ymuint64 tmp0 = w & mask1;
  ymuint64 tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
ymuint64
pm2_2(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask2   = 0x3;
  const ymuint64 offset2 = 0x2;
  ymuint64 tmp0 = w & mask2;
  ymuint64 tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
ymuint64
pm3_1(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask1   = 0x55;
  const ymuint64 offset1 = 0x55;
  ymuint64 tmp0 = w & mask1;
  ymuint64 tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
ymuint64
pm3_2(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask2   = 0x33;
  const ymuint64 offset2 = 0x22;
  ymuint64 tmp0 = w & mask2;
  ymuint64 tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
ymuint64
pm3_4(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask4   = 0x0f;
  const ymuint64 offset4 = 0x04;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
ymuint64
p_3_4(ymuint64 w)
{
  const ymuint64 mask4   = 0x0f;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
ymuint64
pm4_1(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask1   = 0x5555;
  const ymuint64 offset1 = 0x5555;
  ymuint64 tmp0 = w & mask1;
  ymuint64 tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
ymuint64
pm4_2(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask2   = 0x3333;
  const ymuint64 offset2 = 0x2222;
  ymuint64 tmp0 = w & mask2;
  ymuint64 tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
ymuint64
pm4_4(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask4   = 0x0f0f;
  const ymuint64 offset4 = 0x0404;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
ymuint64
p_4_4(ymuint64 w)
{
  const ymuint64 mask4   = 0x0f0f;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
ymuint64
pm4_8(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask8   = 0x00ff;
  const ymuint64 offset8 = 0x0008;
  ymuint64 tmp0 = w & mask8;
  ymuint64 tmp1 = (w >> 8) & mask8;
  m = tmp0 - tmp1 + offset8;
  return tmp0 + tmp1;
}

inline
ymuint64
p_4_8(ymuint64 w)
{
  const ymuint64 mask8   = 0x00ff;
  ymuint64 tmp0 = w & mask8;
  ymuint64 tmp1 = (w >> 8) & mask8;
  return tmp0 + tmp1;
}

inline
ymuint64
pm5_1(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask1   = 0x55555555;
  const ymuint64 offset1 = 0x55555555;
  ymuint64 tmp0 = w & mask1;
  ymuint64 tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
ymuint64
pm5_2(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask2   = 0x33333333;
  const ymuint64 offset2 = 0x22222222;
  ymuint64 tmp0 = w & mask2;
  ymuint64 tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
ymuint64
pm5_4(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask4   = 0x0f0f0f0f;
  const ymuint64 offset4 = 0x04040404;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
ymuint64
p_5_4(ymuint64 w)
{
  const ymuint64 mask4   = 0x0f0f0f0f;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
ymuint64
pm5_8(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask8   = 0x00ff00ff;
  const ymuint64 offset8 = 0x00080008;
  ymuint64 tmp0 = w & mask8;
  ymuint64 tmp1 = (w >> 8) & mask8;
  m = tmp0 - tmp1 + offset8;
  return tmp0 + tmp1;
}

inline
ymuint64
p_5_8(ymuint64 w)
{
  const ymuint64 mask8   = 0x00ff00ff;
  ymuint64 tmp0 = w & mask8;
  ymuint64 tmp1 = (w >> 8) & mask8;
  return tmp0 + tmp1;
}

inline
ymuint64
pm5_16(ymuint64 w,
       ymuint64& m)
{
  const ymuint64 mask16   = 0x0000ffff;
  const ymuint64 offset16 = 0x00000010;
  ymuint64 tmp0 = w & mask16;
  ymuint64 tmp1 = (w >> 16) & mask16;
  m = tmp0 - tmp1 + offset16;
  return tmp0 + tmp1;
}

inline
ymuint64
p_5_16(ymuint64 w)
{
  const ymuint64 mask16   = 0x0000ffff;
  ymuint64 tmp0 = w & mask16;
  ymuint64 tmp1 = (w >> 16) & mask16;
  return tmp0 + tmp1;
}

inline
ymuint64
pm6_1(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask1   = 0x5555555555555555;
  const ymuint64 offset1 = 0x5555555555555555;
  ymuint64 tmp0 = w & mask1;
  ymuint64 tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
ymuint64
pm6_2(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask2   = 0x3333333333333333;
  const ymuint64 offset2 = 0x2222222222222222;
  ymuint64 tmp0 = w & mask2;
  ymuint64 tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
ymuint64
pm6_4(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask4   = 0x0f0f0f0f0f0f0f0f;
  const ymuint64 offset4 = 0x0404040404040404;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
ymuint64
p_6_4(ymuint64 w)
{
  const ymuint64 mask4   = 0x0f0f0f0f0f0f0f0f;
  ymuint64 tmp0 = w & mask4;
  ymuint64 tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
ymuint64
pm6_8(ymuint64 w,
      ymuint64& m)
{
  const ymuint64 mask8   = 0x00ff00ff00ff00ff;
  const ymuint64 offset8 = 0x0008000800080008;
  ymuint64 tmp0 = w & mask8;
  ymuint64 tmp1 = (w >> 8) & mask8;
  m = tmp0 - tmp1 + offset8;
  return tmp0 + tmp1;
}

inline
ymuint64
p_6_8(ymuint64 w)
{
  const ymuint64 mask8   = 0x00ff00ff00ff00ff;
  ymuint64 tmp0 = w & mask8;
  ymuint64 tmp1 = (w >> 8) & mask8;
  return tmp0 + tmp1;
}

inline
ymuint64
pm6_16(ymuint64 w,
       ymuint64& m)
{
  const ymuint64 mask16   = 0x0000ffff0000ffff;
  const ymuint64 offset16 = 0x0000001000000010;
  ymuint64 tmp0 = w & mask16;
  ymuint64 tmp1 = (w >> 16) & mask16;
  m = tmp0 - tmp1 + offset16;
  return tmp0 + tmp1;
}

inline
ymuint64
p_6_16(ymuint64 w)
{
  const ymuint64 mask16   = 0x0000ffff0000ffff;
  ymuint64 tmp0 = w & mask16;
  ymuint64 tmp1 = (w >> 16) & mask16;
  return tmp0 + tmp1;
}

inline
ymuint64
pm6_32(ymuint64 w,
       ymuint64& m)
{
  const ymuint64 mask32   = 0x00000000ffffffff;
  const ymuint64 offset32 = 0x0000000000000020;
  ymuint64 tmp0 = w & mask32;
  ymuint64 tmp1 = (w >> 32) & mask32;
  m = tmp0 - tmp1 + offset32;
  return tmp0 + tmp1;
}

inline
ymuint64
p_6_32(ymuint64 w)
{
  const ymuint64 mask32   = 0x00000000ffffffff;
  ymuint64 tmp0 = w & mask32;
  ymuint64 tmp1 = (w >> 32) & mask32;
  return tmp0 + tmp1;
}

inline
ymuint
w2pos(ymuint ni,
      ymuint i,
      ymuint j)
{
  return i * ni + j;
}

// 5入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_5b(ymuint64* src_vec,
	     ymuint ni,
	     ymint vec1[],
	     ymint vec2[])
{
  const ymint n = 1 << 5;

  ymint ans;

  ymuint64 tmp_1;
  ymuint64 tmp = pm5_1(src_vec[0], tmp_1);
  // tmp  : 0
  // tmp_1: 1
  {
    ymuint64 tmp_2;
    tmp = pm5_2(tmp, tmp_2);
    // tmp  : 00
    // tmp_2: 01
    {
      ymuint64 tmp_3;
      tmp = pm5_4(tmp, tmp_3);
      // tmp  : 000
      // tmp_3: 001
      {
	ymuint64 tmp_4;
	tmp = pm5_8(tmp, tmp_4);
	// tmp  : 0000
	// tmp_4: 0001
	{
	  ymuint64 tmp_5;
	  tmp = pm5_16(tmp, tmp_5);
	  // tmp  : 00000
	  // tmp_5: 00001
	  ans = n - tmp * 2;
	  vec1[4] = n - tmp_5 * 2;
	}
	{
	  ymuint64 tmp_5;
	  tmp = pm5_16(tmp_4, tmp_5);
	  // tmp  : 00010
	  // tmp_5: 00011
	  vec1[3] = n - tmp * 2;
	  vec2[w2pos(ni, 3, 4)] += n - tmp_5 * 2;
	}
      }
      {
	ymuint64 tmp_4;
	tmp = pm5_8(tmp_3, tmp_4);
	// tmp  : 0010
	// tmp_4: 0011
	{
	  ymuint64 tmp_5;
	  tmp = pm5_16(tmp, tmp_5);
	  // tmp  : 00100
	  // tmp_5: 00101
	  vec1[2] = n - tmp * 2;
	  vec2[w2pos(ni, 2, 4)] += n - tmp_5 * 2;
	}
	tmp = p_5_16(tmp_4);
	// tmp  : 00110
	vec2[w2pos(ni, 2, 3)] += n - tmp * 2;
      }
    }
    {
      ymuint64 tmp_3;
      tmp = pm5_4(tmp_2, tmp_3);
      // tmp  : 010
      // tmp_3: 011
      {
	ymuint64 tmp_4;
	tmp = pm5_8(tmp, tmp_4);
	// tmp  : 0100
	// tmp_4: 0101
	{
	  ymuint64 tmp_5;
	  tmp = pm5_16(tmp, tmp_5);
	  // tmp  : 01000
	  // tmp_5: 01001
	  vec1[1] = n - tmp * 2;
	  vec2[w2pos(ni, 1, 4)] += n - tmp_5 * 2;
	}
	tmp = p_5_16(tmp_4);
	// tmp  : 01010
	vec2[w2pos(ni, 1, 3)] += n - tmp * 2;
      }
      tmp = p_5_8(tmp_3);
      // tmp  : 0110
      tmp = p_5_16(tmp);
      // tmp  : 01100
      vec2[w2pos(ni, 1, 2)] += n - tmp * 2;
    }
  }
  {
    ymuint64 tmp_2;
    tmp = pm5_2(tmp_1, tmp_2);
    // tmp  : 10
    // tmp_2: 11
    {
      ymuint64 tmp_3;
      tmp = pm5_4(tmp, tmp_3);
      // tmp  : 100
      // tmp_3: 101
      {
	ymuint64 tmp_4;
	tmp = pm5_8(tmp, tmp_4);
	// tmp  : 1000
	// tmp_4: 1001
	{
	  ymuint64 tmp_5;
	  tmp = pm5_16(tmp, tmp_5);
	  // tmp  : 10000
	  // tmp_5: 10001
	  vec1[0] = n - tmp * 2;
	  vec2[w2pos(ni, 0, 4)] += n - tmp_5 * 2;
	}
	tmp = p_5_16(tmp_4);
	// tmp  : 10010
	vec2[w2pos(ni, 0, 3)] += n - tmp * 2;
      }
      tmp = p_5_8(tmp_3);
      // tmp  : 1010
      tmp = p_5_16(tmp);
      // tmp  : 10100
      vec2[w2pos(ni, 0, 2)] += n - tmp * 2;
    }
    tmp = p_5_4(tmp_2);
    // tmp  : 110
    tmp = p_5_8(tmp);
    // tmp  : 1100
    tmp = p_5_16(tmp);
    // tmp  : 11000
    vec2[w2pos(ni, 0, 1)] += n - tmp * 2;
  }
  return ans;
}

// 6 入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_6b(ymuint64* src_vec,
	     ymuint ni,
	     ymint vec1[],
	     ymint vec2[])
{
  const ymint n = 1 << 6;

  ymint ans;

  ymuint64 tmp_1;
  ymuint64 tmp = pm6_1(src_vec[0], tmp_1);
  // tmp  : 0
  // tmp_1: 1
  {
    ymuint64 tmp_2;
    tmp = pm6_2(tmp, tmp_2);
    // tmp  : 00
    // tmp_2: 01
    {
      ymuint64 tmp_3;
      tmp = pm6_4(tmp, tmp_3);
      // tmp  : 000
      // tmp_3: 001
      {
	ymuint64 tmp_4;
	tmp = pm6_8(tmp, tmp_4);
	// tmp  : 0000
	// tmp_4: 0001
	{
	  ymuint64 tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 00000
	  // tmp_5: 00001
	  {
	    ymuint64 tmp_6;
	    tmp = pm6_32(tmp, tmp_6);
	    // tmp  : 000000
	    // tmp_6: 000001
	    ans = n - tmp * 2;
	    vec1[5] = n - tmp_6 * 2;
	  }
	  {
	    ymuint64 tmp_6;
	    tmp = pm6_32(tmp_5, tmp_6);
	    // tmp  : 000010
	    // tmp_6: 000011
	    vec1[4] = n - tmp * 2;
	    vec2[w2pos(ni, 4, 5)] += n - tmp_6 * 2;
	  }
	}
	{
	  ymuint64 tmp_5;
	  tmp = pm6_16(tmp_4, tmp_5);
	  // tmp  : 00010
	  // tmp_5: 00011
	  {
	    ymuint64 tmp_6;
	    tmp = pm6_32(tmp, tmp_6);
	    // tmp  : 000100
	    // tmp_6: 000101
	    vec1[3] = n - tmp * 2;
	    vec2[w2pos(ni, 3, 5)] += n - tmp_6 * 2;
	  }
	  tmp = p_6_32(tmp_5);
	  // tmp  : 000110
	  vec2[w2pos(ni, 3, 4)] += n - tmp * 2;
	}
      }
      {
	ymuint64 tmp_4;
	tmp = pm6_8(tmp_3, tmp_4);
	// tmp  : 0010
	// tmp_4: 0011
	{
	  ymuint64 tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 00100
	  // tmp_5: 00101
	  {
	    ymuint64 tmp_6;
	    tmp = pm6_32(tmp, tmp_6);
	    // tmp  : 001000
	    // tmp_6: 001001
	    vec1[2] = n - tmp * 2;
	    vec2[w2pos(ni, 2, 5)] += n - tmp_6 * 2;
	  }
	  tmp = p_6_32(tmp_5);
	  // tmp  : 001010
	  vec2[w2pos(ni, 2, 4)] += n - tmp * 2;
	}
	tmp = p_6_16(tmp_4);
	// tmp  : 00110
	tmp = p_6_32(tmp);
	// tmp  : 001100
	vec2[w2pos(ni, 2, 3)] += n - tmp * 2;
      }
    }
    {
      ymuint64 tmp_3;
      tmp = pm6_4(tmp_2, tmp_3);
      // tmp  : 010
      // tmp_3: 011
      {
	ymuint64 tmp_4;
	tmp = pm6_8(tmp, tmp_4);
	// tmp  : 0100
	// tmp_4: 0101
	{
	  ymuint64 tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 01000
	  // tmp_5: 01001
	  {
	    ymuint64 tmp_6;
	    tmp = pm6_32(tmp, tmp_6);
	    // tmp  : 010000
	    // tmp_6: 010001
	    vec1[1] = n - tmp * 2;
	    vec2[w2pos(ni, 1, 5)] += n - tmp_6 * 2;
	  }
	  tmp = p_6_32(tmp_5);
	  // tmp  : 010010
	  vec2[w2pos(ni, 1, 4)] += n - tmp * 2;
	}
	tmp = p_6_16(tmp_4);
	// tmp  : 01010
	tmp = p_6_32(tmp);
	// tmp  : 010100
	vec2[w2pos(ni, 1, 3)] += n - tmp * 2;
      }
      tmp = p_6_8(tmp_3);
      // tmp  : 0110
      tmp = p_6_16(tmp);
      // tmp  : 01100
      tmp = p_6_32(tmp);
      // tmp  : 011000
      vec2[w2pos(ni, 1, 2)] += n - tmp * 2;
    }
  }
  {
    ymuint64 tmp_2;
    tmp = pm6_2(tmp_1, tmp_2);
    // tmp  : 10
    // tmp_2: 11
    {
      ymuint64 tmp_3;
      tmp = pm6_4(tmp, tmp_3);
      // tmp  : 100
      // tmp_3: 101
      {
	ymuint64 tmp_4;
	tmp = pm6_8(tmp, tmp_4);
	// tmp  : 1000
	// tmp_4: 1001
	{
	  ymuint64 tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 10000
	  // tmp_5: 10001
	  {
	    ymuint64 tmp_6;
	    tmp = pm6_32(tmp, tmp_6);
	    // tmp  : 100000
	    // tmp_6: 100001
	    vec1[0] = n - tmp * 2;
	    vec2[w2pos(ni, 0, 5)] += n - tmp_6 * 2;
	  }
	  tmp = p_6_32(tmp_5);
	  // tmp  : 100010
	  vec2[w2pos(ni, 0, 4)] += n - tmp * 2;
	}
	tmp = p_6_16(tmp_4);
	// tmp  : 10010
	tmp = p_6_32(tmp);
	// tmp  : 100100
	vec2[w2pos(ni, 0, 3)] += n - tmp * 2;
      }
      tmp = p_6_8(tmp_3);
      // tmp  : 1010
      tmp = p_6_16(tmp);
      // tmp  : 10100
      tmp = p_6_32(tmp);
      // tmp  : 101000
      vec2[w2pos(ni, 0, 2)] += n - tmp * 2;
    }
    tmp = p_6_4(tmp_2);
    // tmp  : 110
    tmp = p_6_8(tmp);
    // tmp  : 1100
    tmp = p_6_16(tmp);
    // tmp  : 11000
    tmp = p_6_32(tmp);
    // tmp  : 110000
    vec2[w2pos(ni, 0, 1)] += n - tmp * 2;
  }
  return ans;
}

// 7入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_7b(ymuint64* src_vec,
	     ymuint ni,
	     ymint vec1[],
	     ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_6b(src_vec,                     ni, vec1  , vec2);
  ymint ans1 = walsh_012_6b(src_vec + (1 << (6 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 6; ++ i) {
    vec2[w2pos(ni, i, 6)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 6; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[6] = ans0 - ans1;
  return ans0 + ans1;
}

// 8入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_8b(ymuint64* src_vec,
	     ymuint ni,
	     ymint vec1[],
	     ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_7b(src_vec,                     ni, vec1  , vec2);
  ymint ans1 = walsh_012_7b(src_vec + (1 << (7 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 7; ++ i) {
    vec2[w2pos(ni, i, 7)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 7; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[7] = ans0 - ans1;
  return ans0 + ans1;
}

// 9入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_9b(ymuint64* src_vec,
	     ymuint ni,
	     ymint vec1[],
	     ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_8b(src_vec,                     ni, vec1,   vec2);
  ymint ans1 = walsh_012_8b(src_vec + (1 << (8 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 8; ++ i) {
    vec2[w2pos(ni, i, 8)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 8; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[8] = ans0 - ans1;
  return ans0 + ans1;
}

// 10入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_10b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_9b(src_vec,                     ni, vec1  , vec2);
  ymint ans1 = walsh_012_9b(src_vec + (1 << (9 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 9; ++ i) {
    vec2[w2pos(ni, i, 9)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 9; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[9] = ans0 - ans1;
  return ans0 + ans1;
}

// 11入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_11b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_10b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_10b(src_vec + (1 << (10 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 10; ++ i) {
    vec2[w2pos(ni, i, 10)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 10; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[10] = ans0 - ans1;
  return ans0 + ans1;
}

// 12入力の walsh_012 用サブルーティン
inline
ymint
walsh_012_12b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_11b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_11b(src_vec + (1 << (11 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 11; ++ i) {
    vec2[w2pos(ni, i, 11)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 11; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[11] = ans0 - ans1;
  return ans0 + ans1;
}

// 13入力の walsh_012 用サブルーティン
ymint
walsh_012_13b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_12b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_12b(src_vec + (1 << (12 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 12; ++ i) {
    vec2[w2pos(ni, i, 12)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 12; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[12] = ans0 - ans1;
  return ans0 + ans1;
}

// 14入力の walsh_012 用サブルーティン
ymint
walsh_012_14b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_13b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_13b(src_vec + (1 << (13 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 13; ++ i) {
    vec2[w2pos(ni, i, 13)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 13; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[13] = ans0 - ans1;
  return ans0 + ans1;
}

// 15入力の walsh_012 用サブルーティン
ymint
walsh_012_15b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_14b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_14b(src_vec + (1 << (14 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 14; ++ i) {
    vec2[w2pos(ni, i, 14)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 14; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[14] = ans0 - ans1;
  return ans0 + ans1;
}

// 16入力の walsh_012 用サブルーティン
ymint
walsh_012_16b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_15b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_15b(src_vec + (1 << (15 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 15; ++ i) {
    vec2[w2pos(ni, i, 15)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 15; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[15] = ans0 - ans1;
  return ans0 + ans1;
}

// 17入力の walsh_012 用サブルーティン
ymint
walsh_012_17b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_16b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_16b(src_vec + (1 << (16 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 16; ++ i) {
    vec2[w2pos(ni, i, 16)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 16; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[16] = ans0 - ans1;
  return ans0 + ans1;
}

// 18入力の walsh_012 用サブルーティン
ymint
walsh_012_18b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_17b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_17b(src_vec + (1 << (17 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 17; ++ i) {
    vec2[w2pos(ni, i, 17)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 17; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[17] = ans0 - ans1;
  return ans0 + ans1;
}

// 19入力の walsh_012 用サブルーティン
ymint
walsh_012_19b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_18b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_18b(src_vec + (1 << (18 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 18; ++ i) {
    vec2[w2pos(ni, i, 18)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 18; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[18] = ans0 - ans1;
  return ans0 + ans1;
}

// 20入力の walsh_012 用サブルーティン
ymint
walsh_012_20b(ymuint64* src_vec,
	      ymuint ni,
	      ymint vec1[],
	      ymint vec2[])
{
  ymint vec1_1[TvFunc::kMaxNi];
  ymint ans0 = walsh_012_19b(src_vec,                      ni, vec1  , vec2);
  ymint ans1 = walsh_012_19b(src_vec + (1 << (19 - NIPW)), ni, vec1_1, vec2);
  for (ymuint i = 0; i < 19; ++ i) {
    vec2[w2pos(ni, i, 19)] += vec1[i] - vec1_1[i];
  }
  for (ymuint i = 0; i < 19; ++ i) {
    vec1[i] += vec1_1[i];
  }
  vec1[19] = ans0 - ans1;
  return ans0 + ans1;
}

// 2入力の walsh_012 本体
ymint
walsh_012_2(ymuint64* src_vec,
	    ymint vec1[],
	    ymint vec2[])
{
  const ymint n = 1 << 2;

  ymint ans;

  ymuint64 tmp_1;
  ymuint64 tmp = pm2_1(src_vec[0], tmp_1);
  {
    ymuint64 tmp_2;
    tmp = pm2_2(tmp, tmp_2);
    ans = n - tmp * 2;
    vec1[1] = n - tmp_2 * 2;
  }
  {
    ymuint64 tmp_2;
    tmp = pm2_2(tmp_1, tmp_2);
    vec1[0] = n - tmp * 2;
    vec2[w2pos(2, 1, 0)] = vec2[w2pos(2, 0, 1)] = n - tmp_2 * 2;
  }

  return ans;
}

// 3入力の walsh_012 本体
ymint
walsh_012_3(ymuint64* src_vec,
	    ymint vec1[],
	    ymint vec2[])
{
  const ymint n = 1 << 3;

  ymint ans;

  ymuint64 tmp_1;
  ymuint64 tmp = pm3_1(src_vec[0], tmp_1);
  {
    ymuint64 tmp_2;
    tmp = pm3_2(tmp, tmp_2);
    {
      ymuint64 tmp_3;
      tmp = pm3_4(tmp, tmp_3);
      ans = n - tmp * 2;
      vec1[2] = n - tmp_3 * 2;
    }
    {
      ymuint64 tmp_3;
      tmp = pm3_4(tmp_2, tmp_3);
      vec1[1] = n - tmp * 2;
      vec2[w2pos(3, 2, 1)] = vec2[w2pos(3, 1, 2)] = n - tmp_3 * 2;
    }
  }
  {
    ymuint64 tmp_2;
    tmp = pm3_2(tmp_1, tmp_2);
    {
      ymuint64 tmp_3;
      tmp = pm3_4(tmp, tmp_3);
      vec1[0] = n - tmp * 2;
      vec2[w2pos(3, 2, 0)] = vec2[w2pos(3, 0, 2)] = n - tmp_3 * 2;
    }
    {
      tmp = p_3_4(tmp_2);
      vec2[w2pos(3, 1, 0)] = vec2[w2pos(3, 0, 1)] = n - tmp * 2;
    }
  }

  return ans;
}

// 4入力の walsh_012 本体
ymint
walsh_012_4(ymuint64* src_vec,
	    ymint vec1[],
	    ymint vec2[])
{
  const ymint n = 1 << 4;

  ymint ans;

  ymuint64 tmp_1;
  ymuint64 tmp = pm4_1(src_vec[0], tmp_1);
  {
    ymuint64 tmp_2;
    tmp = pm4_2(tmp, tmp_2);
    {
      ymuint64 tmp_3;
      tmp = pm4_4(tmp, tmp_3);
      {
	ymuint64 tmp_4;
	tmp = pm4_8(tmp, tmp_4);
	ans = n - tmp * 2;
	vec1[3] = n - tmp_4 * 2;
      }
      {
	ymuint64 tmp_4;
	tmp = pm4_8(tmp_3, tmp_4);
	vec1[2] = n - tmp * 2;
	vec2[w2pos(4, 3, 2)] = vec2[w2pos(4, 2, 3)] = n - tmp_4 * 2;
      }
    }
    {
      ymuint64 tmp_3;
      tmp = pm4_4(tmp_2, tmp_3);
      {
	ymuint64 tmp_4;
	tmp = pm4_8(tmp, tmp_4);
	vec1[1] = n - tmp * 2;
	vec2[w2pos(4, 3, 1)] = vec2[w2pos(4, 1, 3)] = n - tmp_4 * 2;
      }
      tmp = p_4_8(tmp_3);
      vec2[w2pos(4, 2, 1)] = vec2[w2pos(4, 1, 2)] = n - tmp * 2;
    }
  }
  {
    ymuint64 tmp_2;
    tmp = pm4_2(tmp_1, tmp_2);
    {
      ymuint64 tmp_3;
      tmp = pm4_4(tmp, tmp_3);
      {
	ymuint64 tmp_4;
	tmp = pm4_8(tmp, tmp_4);
	vec1[0] = n - tmp * 2;
	vec2[w2pos(4, 3, 0)] = vec2[w2pos(4, 0, 3)] = n - tmp_4 * 2;
      }
      tmp = p_4_8(tmp_3);
      vec2[w2pos(4, 2, 0)] = vec2[w2pos(4, 0, 2)] = n - tmp * 2;
    }
    tmp = p_4_4(tmp_2);
    tmp =  p_4_8(tmp);
    vec2[w2pos(4, 1, 0)] = vec2[w2pos(4, 0, 1)] = n - tmp * 2;
  }

  return ans;
}

// 5入力の walsh_012 本体
ymint
walsh_012_5(ymuint64* src_vec,
	    ymint vec1[],
	    ymint vec2[])
{
  const ymuint nn = 5 * 5;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_5b(src_vec, 5, vec1, vec2);

  for (ymuint i = 0; i < 4; ++ i) {
    for (ymuint j = i + 1; j < 5; ++ j) {
      vec2[w2pos(5, j, i)] = vec2[w2pos(5, i, j)];
    }
  }

  return ans;
}

// 6入力の walsh_012 本体
int
walsh_012_6(ymuint64* src_vec,
	    int vec1[],
	    int vec2[])
{
  const ymuint nn = 6 * 6;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_6b(src_vec, 6, vec1, vec2);

  for (ymuint i = 0; i < 5; ++ i) {
    for (ymuint j = i + 1; j < 6; ++ j) {
      vec2[w2pos(6, j, i)] = vec2[w2pos(6, i, j)];
    }
  }

  return ans;
}

// 7入力の walsh_012 本体
int
walsh_012_7(ymuint64* src_vec,
	    int vec1[],
	    int vec2[])
{
  const ymuint nn = 7 * 7;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_7b(src_vec, 7, vec1, vec2);

  for (ymuint i = 0; i < 6; ++ i) {
    for (ymuint j = i + 1; j < 7; ++ j) {
      vec2[w2pos(7, j, i)] = vec2[w2pos(7, i, j)];
    }
  }

  return ans;
}

// 8入力の walsh_012 本体
int
walsh_012_8(ymuint64* src_vec,
	    int vec1[],
	    int vec2[])
{
  const ymuint nn = 8 * 8;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_8b(src_vec, 8, vec1, vec2);

  for (ymuint i = 0; i < 7; ++ i) {
    for (ymuint j = i + 1; j < 8; ++ j) {
      vec2[w2pos(8, j, i)] = vec2[w2pos(8, i, j)];
    }
  }

  return ans;
}

// 9入力の walsh_012 本体
int
walsh_012_9(ymuint64* src_vec,
	    int vec1[],
	    int vec2[])
{
  const ymuint nn = 9 * 9;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_9b(src_vec, 9, vec1, vec2);

  for (ymuint i = 0; i < 8; ++ i) {
    for (ymuint j = i + 1; j < 9; ++ j) {
      vec2[w2pos(9, j, i)] = vec2[w2pos(9, i, j)];
    }
  }

  return ans;
}

// 10入力の walsh_012 本体
int
walsh_012_10(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 10 * 10;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_10b(src_vec, 10, vec1, vec2);

  for (ymuint i = 0; i < 9; ++ i) {
    for (ymuint j = i + 1; j < 10; ++ j) {
      vec2[w2pos(10, j, i)] = vec2[w2pos(10, i, j)];
    }
  }

  return ans;
}

// 11入力の walsh_012 本体
int
walsh_012_11(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 11 * 11;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_11b(src_vec, 11, vec1, vec2);

  for (ymuint i = 0; i < 10; ++ i) {
    for (ymuint j = i + 1; j < 11; ++ j) {
      vec2[w2pos(11, j, i)] = vec2[w2pos(11, i, j)];
    }
  }

  return ans;
}

// 12入力の walsh_012 本体
int
walsh_012_12(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 12 * 12;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_12b(src_vec, 12, vec1, vec2);

  for (ymuint i = 0; i < 11; ++ i) {
    for (ymuint j = i + 1; j < 12; ++ j) {
      vec2[w2pos(12, j, i)] = vec2[w2pos(12, i, j)];
    }
  }

  return ans;
}

// 13入力の walsh_012 本体
int
walsh_012_13(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 13 * 13;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_13b(src_vec, 13, vec1, vec2);

  for (ymuint i = 0; i < 12; ++ i) {
    for (ymuint j = i + 1; j < 13; ++ j) {
      vec2[w2pos(13, j, i)] = vec2[w2pos(13, i, j)];
    }
  }

  return ans;
}

// 14入力の walsh_012 本体
int
walsh_012_14(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 14 * 14;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_14b(src_vec, 14, vec1, vec2);

  for (ymuint i = 0; i < 13; ++ i) {
    for (ymuint j = i + 1; j < 14; ++ j) {
      vec2[w2pos(14, j, i)] = vec2[w2pos(14, i, j)];
    }
  }

  return ans;
}

// 15入力の walsh_012 本体
int
walsh_012_15(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 15 * 15;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_15b(src_vec, 15, vec1, vec2);

  for (ymuint i = 0; i < 14; ++ i) {
    for (ymuint j = i + 1; j < 15; ++ j) {
      vec2[w2pos(15, j, i)] = vec2[w2pos(15, i, j)];
    }
  }

  return ans;
}

// 16入力の walsh_012 本体
int
walsh_012_16(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 16 * 16;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_16b(src_vec, 16, vec1, vec2);

  for (ymuint i = 0; i < 15; ++ i) {
    for (ymuint j = i + 1; j < 16; ++ j) {
      vec2[w2pos(16, j, i)] = vec2[w2pos(16, i, j)];
    }
  }

  return ans;
}

// 17入力の walsh_012 本体
int
walsh_012_17(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 17 * 17;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_17b(src_vec, 17, vec1, vec2);

  for (ymuint i = 0; i < 16; ++ i) {
    for (ymuint j = i + 1; j < 17; ++ j) {
      vec2[w2pos(17, j, i)] = vec2[w2pos(17, i, j)];
    }
  }

  return ans;
}

// 18入力の walsh_012 本体
int
walsh_012_18(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 18 * 18;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_18b(src_vec, 18, vec1, vec2);

  for (ymuint i = 0; i < 17; ++ i) {
    for (ymuint j = i + 1; j < 18; ++ j) {
      vec2[w2pos(18, j, i)] = vec2[w2pos(18, i, j)];
    }
  }

  return ans;
}

// 19入力の walsh_012 本体
int
walsh_012_19(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 19 * 19;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_19b(src_vec, 19, vec1, vec2);

  for (ymuint i = 0; i < 18; ++ i) {
    for (ymuint j = i + 1; j < 19; ++ j) {
      vec2[w2pos(19, j, i)] = vec2[w2pos(19, i, j)];
    }
  }

  return ans;
}

// 20入力の walsh_01 本体
int
walsh_012_20(ymuint64* src_vec,
	     int vec1[],
	     int vec2[])
{
  const ymuint nn = 20 * 20;
  for (ymuint i = 0; i < nn; ++ i) {
    vec2[i] = 0;
  }

  int ans = walsh_012_20b(src_vec, 20, vec1, vec2);

  for (ymuint i = 0; i < 19; ++ i) {
    for (ymuint j = i + 1; j < 20; ++ j) {
      vec2[w2pos(20, j, i)] = vec2[w2pos(20, i, j)];
    }
  }

  return ans;
}

END_NONAMESPACE

// 0次と 1次と 2次の Walsh 係数を求める．
int
TvFunc::walsh_012(int vec1[],
		  int vec2[]) const
{
  switch ( input_num() ) {
  case  2: return walsh_012_2(mVector, vec1, vec2);
  case  3: return walsh_012_3(mVector, vec1, vec2);
  case  4: return walsh_012_4(mVector, vec1, vec2);
  case  5: return walsh_012_5(mVector, vec1, vec2);
  case  6: return walsh_012_6(mVector, vec1, vec2);
  case  7: return walsh_012_7(mVector, vec1, vec2);
  case  8: return walsh_012_8(mVector, vec1, vec2);
  case  9: return walsh_012_9(mVector, vec1, vec2);
  case 10: return walsh_012_10(mVector, vec1, vec2);
  case 11: return walsh_012_11(mVector, vec1, vec2);
  case 12: return walsh_012_12(mVector, vec1, vec2);
  case 13: return walsh_012_13(mVector, vec1, vec2);
  case 14: return walsh_012_14(mVector, vec1, vec2);
  case 15: return walsh_012_15(mVector, vec1, vec2);
  case 16: return walsh_012_16(mVector, vec1, vec2);
  case 17: return walsh_012_17(mVector, vec1, vec2);
  case 18: return walsh_012_18(mVector, vec1, vec2);
  case 19: return walsh_012_19(mVector, vec1, vec2);
  case 20: return walsh_012_20(mVector, vec1, vec2);
  default:
    ;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

BEGIN_NONAMESPACE

inline
int
walsh_w0_2(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  ymuint64 tmp = src_vec[0];
  int nall;
  int c;
  switch ( w ) {
  case 0:
    nall = 1;
    c = (tmp >> (0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 2;
    c  = (tmp >> (1 ^ ibits)) & 1;
    c += (tmp >> (2 ^ ibits)) & 1;
    break;
  case 2:
    nall = 1;
    c = (tmp >> (3 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
    nall = 0;
    c = 0;
  }

  return nall - c * 2;
}

inline
int
walsh_w0_3(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  ymuint64 tmp = src_vec[0];
  int nall;
  int c;
  switch ( w ) {
  case 0:
    nall = 1;
    c = (tmp >> (0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 3;
    c  = (tmp >> (1 ^ ibits)) & 1;
    c += (tmp >> (2 ^ ibits)) & 1;
    c += (tmp >> (4 ^ ibits)) & 1;
    break;
  case 2:
    nall = 3;
    c  = (tmp >> (3 ^ ibits)) & 1;
    c += (tmp >> (5 ^ ibits)) & 1;
    c += (tmp >> (6 ^ ibits)) & 1;
    break;
  case 3:
    nall = 1;
    c = (tmp >> (7 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
    nall = 0;
    c = 0;
  }

  return nall - c * 2;
}

inline
int
walsh_w0_4(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  ymuint64 tmp = src_vec[0];
  int nall;
  int c;
  switch ( w ) {
  case 0:
    nall = 1;
    c = (tmp >> (0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 4;
    c  = (tmp >> (1 ^ ibits)) & 1;
    c += (tmp >> (2 ^ ibits)) & 1;
    c += (tmp >> (4 ^ ibits)) & 1;
    c += (tmp >> (8 ^ ibits)) & 1;
    break;
  case 2:
    nall = 6;
    c  = (tmp >> (3 ^ ibits)) & 1;
    c += (tmp >> (5 ^ ibits)) & 1;
    c += (tmp >> (6 ^ ibits)) & 1;
    c += (tmp >> (9 ^ ibits)) & 1;
    c += (tmp >> (10 ^ ibits)) & 1;
    c += (tmp >> (12 ^ ibits)) & 1;
    break;
  case 3:
    nall = 4;
    c  = (tmp >> (7 ^ ibits)) & 1;
    c += (tmp >> (11 ^ ibits)) & 1;
    c += (tmp >> (13 ^ ibits)) & 1;
    c += (tmp >> (14 ^ ibits)) & 1;
    break;
  case 4:
    nall = 1;
    c = (tmp >> (15 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
    nall = 0;
    c = 0;
  }

  return nall - c * 2;
}

inline
int
walsh_w0_5_0(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int nall = 1;
  int c = (tmp >> (0 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_5_1(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int nall = 5;
  int c = (tmp >> (1 ^ ibits)) & 1;
  c += (tmp >> (2 ^ ibits)) & 1;
  c += (tmp >> (4 ^ ibits)) & 1;
  c += (tmp >> (8 ^ ibits)) & 1;
  c += (tmp >> (16 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_5_2(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int nall = 10;
  int c  = (tmp >> (3 ^ ibits)) & 1;
  c += (tmp >> (5 ^ ibits)) & 1;
  c += (tmp >> (6 ^ ibits)) & 1;
  c += (tmp >> (9 ^ ibits)) & 1;
  c += (tmp >> (10 ^ ibits)) & 1;
  c += (tmp >> (12 ^ ibits)) & 1;
  c += (tmp >> (17 ^ ibits)) & 1;
  c += (tmp >> (18 ^ ibits)) & 1;
  c += (tmp >> (20 ^ ibits)) & 1;
  c += (tmp >> (24 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_5_3(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int nall = 10;
  int c  = (tmp >> (7 ^ ibits)) & 1;
  c += (tmp >> (11 ^ ibits)) & 1;
  c += (tmp >> (13 ^ ibits)) & 1;
  c += (tmp >> (14 ^ ibits)) & 1;
  c += (tmp >> (19 ^ ibits)) & 1;
  c += (tmp >> (21 ^ ibits)) & 1;
  c += (tmp >> (22 ^ ibits)) & 1;
  c += (tmp >> (25 ^ ibits)) & 1;
  c += (tmp >> (26 ^ ibits)) & 1;
  c += (tmp >> (28 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_5_4(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int nall = 5;
  int c  = (tmp >> (15 ^ ibits)) & 1;
  c += (tmp >> (23 ^ ibits)) & 1;
  c += (tmp >> (27 ^ ibits)) & 1;
  c += (tmp >> (29 ^ ibits)) & 1;
  c += (tmp >> (30 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_5_5(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int nall = 1;
  int c = (tmp >> (31 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_5(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  switch ( w ) {
  case 0: return walsh_w0_5_0(src_vec, ibits);
  case 1: return walsh_w0_5_1(src_vec, ibits);
  case 2: return walsh_w0_5_2(src_vec, ibits);
  case 3: return walsh_w0_5_3(src_vec, ibits);
  case 4: return walsh_w0_5_4(src_vec, ibits);
  case 5: return walsh_w0_5_5(src_vec, ibits);
  default:
    ASSERT_NOT_REACHED;
  }

  return 0;
}

inline
int
walsh_w0_6_0(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 1;
  int c = (tmp >> (0 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6_1(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 6;
  int c = (tmp >> (1 ^ ibits)) & 1;
  c += (tmp >> (2 ^ ibits)) & 1;
  c += (tmp >> (4 ^ ibits)) & 1;
  c += (tmp >> (8 ^ ibits)) & 1;
  c += (tmp >> (16 ^ ibits)) & 1;
  c += (tmp >> (32 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6_2(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 15;
  int c = (tmp >> (3 ^ ibits)) & 1;
  c += (tmp >> (5 ^ ibits)) & 1;
  c += (tmp >> (6 ^ ibits)) & 1;
  c += (tmp >> (9 ^ ibits)) & 1;
  c += (tmp >> (10 ^ ibits)) & 1;
  c += (tmp >> (12 ^ ibits)) & 1;
  c += (tmp >> (17 ^ ibits)) & 1;
  c += (tmp >> (18 ^ ibits)) & 1;
  c += (tmp >> (20 ^ ibits)) & 1;
  c += (tmp >> (24 ^ ibits)) & 1;
  c += (tmp >> (33 ^ ibits)) & 1;
  c += (tmp >> (34 ^ ibits)) & 1;
  c += (tmp >> (36 ^ ibits)) & 1;
  c += (tmp >> (40 ^ ibits)) & 1;
  c += (tmp >> (48 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6_3(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 20;
  int c = (tmp >> (7 ^ ibits)) & 1;
  c += (tmp >> (11 ^ ibits)) & 1;
  c += (tmp >> (13 ^ ibits)) & 1;
  c += (tmp >> (14 ^ ibits)) & 1;
  c += (tmp >> (19 ^ ibits)) & 1;
  c += (tmp >> (21 ^ ibits)) & 1;
  c += (tmp >> (22 ^ ibits)) & 1;
  c += (tmp >> (25 ^ ibits)) & 1;
  c += (tmp >> (26 ^ ibits)) & 1;
  c += (tmp >> (28 ^ ibits)) & 1;
  c += (tmp >> (35 ^ ibits)) & 1;
  c += (tmp >> (37 ^ ibits)) & 1;
  c += (tmp >> (38 ^ ibits)) & 1;
  c += (tmp >> (41 ^ ibits)) & 1;
  c += (tmp >> (42 ^ ibits)) & 1;
  c += (tmp >> (44 ^ ibits)) & 1;
  c += (tmp >> (49 ^ ibits)) & 1;
  c += (tmp >> (50 ^ ibits)) & 1;
  c += (tmp >> (52 ^ ibits)) & 1;
  c += (tmp >> (56 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6_4(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 15;
  int c = (tmp >> (15 ^ ibits)) & 1;
  c += (tmp >> (23 ^ ibits)) & 1;
  c += (tmp >> (27 ^ ibits)) & 1;
  c += (tmp >> (29 ^ ibits)) & 1;
  c += (tmp >> (30 ^ ibits)) & 1;
  c += (tmp >> (39 ^ ibits)) & 1;
  c += (tmp >> (43 ^ ibits)) & 1;
  c += (tmp >> (45 ^ ibits)) & 1;
  c += (tmp >> (46 ^ ibits)) & 1;
  c += (tmp >> (51 ^ ibits)) & 1;
  c += (tmp >> (53 ^ ibits)) & 1;
  c += (tmp >> (54 ^ ibits)) & 1;
  c += (tmp >> (57 ^ ibits)) & 1;
  c += (tmp >> (58 ^ ibits)) & 1;
  c += (tmp >> (60 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6_5(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 6;
  int c = (tmp >> (31 ^ ibits)) & 1;
  c += (tmp >> (47 ^ ibits)) & 1;
  c += (tmp >> (55 ^ ibits)) & 1;
  c += (tmp >> (59 ^ ibits)) & 1;
  c += (tmp >> (61 ^ ibits)) & 1;
  c += (tmp >> (62 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6_6(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint64 tmp = src_vec[0];
  int n = 1;
  int c = (tmp >> (63 ^ ibits)) & 1;
  return n - c * 2;
}

inline
int
walsh_w0_6(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  switch ( w ) {
  case 0: return walsh_w0_6_0(src_vec, ibits);
  case 1: return walsh_w0_6_1(src_vec, ibits);
  case 2: return walsh_w0_6_2(src_vec, ibits);
  case 3: return walsh_w0_6_3(src_vec, ibits);
  case 4: return walsh_w0_6_4(src_vec, ibits);
  case 5: return walsh_w0_6_5(src_vec, ibits);
  case 6: return walsh_w0_6_6(src_vec, ibits);
  default:
    ASSERT_NOT_REACHED;
  }

  return 0;
}

inline
int
walsh_w0_7_0(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset = flag * (1 << (6 - NIPW));
  return walsh_w0_6_0(src_vec + offset, ibits);
}

inline
int
walsh_w0_7_1(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset1 = flag * (1 << (6 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (6 - NIPW));
  int ans1 = walsh_w0_6_1(src_vec + offset1, ibits);
  int ans2 = walsh_w0_6_0(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_7_2(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset1 = flag * (1 << (6 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (6 - NIPW));
  int ans1 = walsh_w0_6_2(src_vec + offset1, ibits);
  int ans2 = walsh_w0_6_1(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_7_3(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset1 = flag * (1 << (6 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (6 - NIPW));
  int ans1 = walsh_w0_6_3(src_vec + offset1, ibits);
  int ans2 = walsh_w0_6_2(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_7_4(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset1 = flag * (1 << (6 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (6 - NIPW));
  int ans1 = walsh_w0_6_4(src_vec + offset1, ibits);
  int ans2 = walsh_w0_6_3(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_7_5(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset1 = flag * (1 << (6 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (6 - NIPW));
  int ans1 = walsh_w0_6_5(src_vec + offset1, ibits);
  int ans2 = walsh_w0_6_4(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_7_6(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset1 = flag * (1 << (6 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (6 - NIPW));
  int ans1 = walsh_w0_6_6(src_vec + offset1, ibits);
  int ans2 = walsh_w0_6_5(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_7_7(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 6) & 1);
  ymuint offset = (1 - flag) * (1 << (6 - NIPW));
  return walsh_w0_6_6(src_vec + offset, ibits);
}

inline
int
walsh_w0_7(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  switch ( w ) {
  case 0: return walsh_w0_7_0(src_vec, ibits);
  case 1: return walsh_w0_7_1(src_vec, ibits);
  case 2: return walsh_w0_7_2(src_vec, ibits);
  case 3: return walsh_w0_7_3(src_vec, ibits);
  case 4: return walsh_w0_7_4(src_vec, ibits);
  case 5: return walsh_w0_7_5(src_vec, ibits);
  case 6: return walsh_w0_7_6(src_vec, ibits);
  case 7: return walsh_w0_7_7(src_vec, ibits);
  default:
    ASSERT_NOT_REACHED;
  }

  return 0;
}

inline
int
walsh_w0_8_0(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset = flag * (1 << (7 - NIPW));
  return walsh_w0_7_0(src_vec + offset, ibits);
}

inline
int
walsh_w0_8_1(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_1(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_0(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_2(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_2(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_1(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_3(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_3(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_2(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_4(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_4(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_3(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_5(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_5(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_4(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_6(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_6(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_5(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_7(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset1 = flag * (1 << (7 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (7 - NIPW));
  int ans1 = walsh_w0_7_7(src_vec + offset1, ibits);
  int ans2 = walsh_w0_7_6(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_8_8(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 7) & 1);
  ymuint offset = (1 - flag) * (1 << (7 - NIPW));
  return walsh_w0_7_7(src_vec + offset, ibits);
}

inline
int
walsh_w0_8(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  switch ( w ) {
  case 0: return walsh_w0_8_0(src_vec, ibits);
  case 1: return walsh_w0_8_1(src_vec, ibits);
  case 2: return walsh_w0_8_2(src_vec, ibits);
  case 3: return walsh_w0_8_3(src_vec, ibits);
  case 4: return walsh_w0_8_4(src_vec, ibits);
  case 5: return walsh_w0_8_5(src_vec, ibits);
  case 6: return walsh_w0_8_6(src_vec, ibits);
  case 7: return walsh_w0_8_7(src_vec, ibits);
  case 8: return walsh_w0_8_8(src_vec, ibits);
  default:
    ASSERT_NOT_REACHED;
  }

  return 0;
}

inline
int
walsh_w0_9_0(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset = flag * (1 << (8 - NIPW));
  return walsh_w0_8_0(src_vec + offset, ibits);
}

inline
int
walsh_w0_9_1(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_1(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_0(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_2(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_2(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_1(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_3(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_3(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_2(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_4(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_4(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_3(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_5(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_5(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_4(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_6(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_6(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_5(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_7(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_7(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_6(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_8(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset1 = flag * (1 << (8 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (8 - NIPW));
  int ans1 = walsh_w0_8_8(src_vec + offset1, ibits);
  int ans2 = walsh_w0_8_7(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_9_9(ymuint64* src_vec,
	     ymuint ibits)
{
  ymuint flag = ((ibits >> 8) & 1);
  ymuint offset = (1 - flag) * (1 << (8 - NIPW));
  return walsh_w0_8_8(src_vec + offset, ibits);
}

inline
int
walsh_w0_9(ymuint64* src_vec,
	   ymuint ibits,
	   ymuint w)
{
  switch ( w ) {
  case 0: return walsh_w0_9_0(src_vec, ibits);
  case 1: return walsh_w0_9_1(src_vec, ibits);
  case 2: return walsh_w0_9_2(src_vec, ibits);
  case 3: return walsh_w0_9_3(src_vec, ibits);
  case 4: return walsh_w0_9_4(src_vec, ibits);
  case 5: return walsh_w0_9_5(src_vec, ibits);
  case 6: return walsh_w0_9_6(src_vec, ibits);
  case 7: return walsh_w0_9_7(src_vec, ibits);
  case 8: return walsh_w0_9_8(src_vec, ibits);
  case 9: return walsh_w0_9_9(src_vec, ibits);
  default:
    ASSERT_NOT_REACHED;
  }

  return 0;
}

inline
int
walsh_w0_10_0(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset = flag * (1 << (9 - NIPW));
  return walsh_w0_9_0(src_vec + offset, ibits);
}

inline
int
walsh_w0_10_1(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_1(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_0(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_2(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_2(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_1(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_3(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_3(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_2(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_4(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_4(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_3(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_5(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_5(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_4(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_6(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_6(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_5(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_7(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_7(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_6(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_8(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_8(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_7(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_9(ymuint64* src_vec,
	      ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset1 = flag * (1 << (9 - NIPW));
  ymuint offset2 = (1 - flag) * (1 << (9 - NIPW));
  int ans1 = walsh_w0_9_9(src_vec + offset1, ibits);
  int ans2 = walsh_w0_9_8(src_vec + offset2, ibits);
  return ans1 + ans2;
}

inline
int
walsh_w0_10_10(ymuint64* src_vec,
	       ymuint ibits)
{
  ymuint flag = ((ibits >> 9) & 1);
  ymuint offset = (1 - flag) * (1 << (9 - NIPW));
  return walsh_w0_9_9(src_vec + offset, ibits);
}

inline
int
walsh_w0_10(ymuint64* src_vec,
	    ymuint ibits,
	    ymuint w)
{
  switch ( w ) {
  case 0: return walsh_w0_10_0(src_vec, ibits);
  case 1: return walsh_w0_10_1(src_vec, ibits);
  case 2: return walsh_w0_10_2(src_vec, ibits);
  case 3: return walsh_w0_10_3(src_vec, ibits);
  case 4: return walsh_w0_10_4(src_vec, ibits);
  case 5: return walsh_w0_10_5(src_vec, ibits);
  case 6: return walsh_w0_10_6(src_vec, ibits);
  case 7: return walsh_w0_10_7(src_vec, ibits);
  case 8: return walsh_w0_10_8(src_vec, ibits);
  case 9: return walsh_w0_10_9(src_vec, ibits);
  case 10: return walsh_w0_10_10(src_vec, ibits);
  default:
    ASSERT_NOT_REACHED;
  }

  return 0;
}


END_NONAMESPACE

// 重み別の 0 次の Walsh 係数を求める．
int
TvFunc::walsh_w0(ymuint w,
		 bool oinv,
		 ymuint ibits) const
{
  int ans;
  switch ( input_num() ) {
  case  2: ans = walsh_w0_2(mVector, ibits, w); break;
  case  3: ans = walsh_w0_3(mVector, ibits, w); break;
  case  4: ans = walsh_w0_4(mVector, ibits, w); break;
  case  5: ans = walsh_w0_5(mVector, ibits, w); break;
  case  6: ans = walsh_w0_6(mVector, ibits, w); break;
  case  7: ans = walsh_w0_7(mVector, ibits, w); break;
  case  8: ans = walsh_w0_8(mVector, ibits, w); break;
  case  9: ans = walsh_w0_9(mVector, ibits, w); break;
  case 10: ans = walsh_w0_10(mVector, ibits, w); break;

#if 0
  case 11:
    switch ( w ) {
    case 0: ans = walsh_w0_11_0(mVector, ibits); break;
    case 1: ans = walsh_w0_11_1(mVector, ibits); break;
    case 2: ans = walsh_w0_11_2(mVector, ibits); break;
    case 3: ans = walsh_w0_11_3(mVector, ibits); break;
    case 4: ans = walsh_w0_11_4(mVector, ibits); break;
    case 5: ans = walsh_w0_11_5(mVector, ibits); break;
    case 6: ans = walsh_w0_11_6(mVector, ibits); break;
    case 7: ans = walsh_w0_11_7(mVector, ibits); break;
    case 8: ans = walsh_w0_11_8(mVector, ibits); break;
    case 9: ans = walsh_w0_11_9(mVector, ibits); break;
    case 10: ans = walsh_w0_11_10(mVector, ibits); break;
    case 11: ans = walsh_w0_11_11(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 12:
    switch ( w ) {
    case 0: ans = walsh_w0_12_0(mVector, ibits); break;
    case 1: ans = walsh_w0_12_1(mVector, ibits); break;
    case 2: ans = walsh_w0_12_2(mVector, ibits); break;
    case 3: ans = walsh_w0_12_3(mVector, ibits); break;
    case 4: ans = walsh_w0_12_4(mVector, ibits); break;
    case 5: ans = walsh_w0_12_5(mVector, ibits); break;
    case 6: ans = walsh_w0_12_6(mVector, ibits); break;
    case 7: ans = walsh_w0_12_7(mVector, ibits); break;
    case 8: ans = walsh_w0_12_8(mVector, ibits); break;
    case 9: ans = walsh_w0_12_9(mVector, ibits); break;
    case 10: ans = walsh_w0_12_10(mVector, ibits); break;
    case 11: ans = walsh_w0_12_11(mVector, ibits); break;
    case 12: ans = walsh_w0_12_12(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 13:
    switch ( w ) {
    case 0: ans = walsh_w0_13_0(mVector, ibits); break;
    case 1: ans = walsh_w0_13_1(mVector, ibits); break;
    case 2: ans = walsh_w0_13_2(mVector, ibits); break;
    case 3: ans = walsh_w0_13_3(mVector, ibits); break;
    case 4: ans = walsh_w0_13_4(mVector, ibits); break;
    case 5: ans = walsh_w0_13_5(mVector, ibits); break;
    case 6: ans = walsh_w0_13_6(mVector, ibits); break;
    case 7: ans = walsh_w0_13_7(mVector, ibits); break;
    case 8: ans = walsh_w0_13_8(mVector, ibits); break;
    case 9: ans = walsh_w0_13_9(mVector, ibits); break;
    case 10: ans = walsh_w0_13_10(mVector, ibits); break;
    case 11: ans = walsh_w0_13_11(mVector, ibits); break;
    case 12: ans = walsh_w0_13_12(mVector, ibits); break;
    case 13: ans = walsh_w0_13_13(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 14:
    switch ( w ) {
    case 0: ans = walsh_w0_14_0(mVector, ibits); break;
    case 1: ans = walsh_w0_14_1(mVector, ibits); break;
    case 2: ans = walsh_w0_14_2(mVector, ibits); break;
    case 3: ans = walsh_w0_14_3(mVector, ibits); break;
    case 4: ans = walsh_w0_14_4(mVector, ibits); break;
    case 5: ans = walsh_w0_14_5(mVector, ibits); break;
    case 6: ans = walsh_w0_14_6(mVector, ibits); break;
    case 7: ans = walsh_w0_14_7(mVector, ibits); break;
    case 8: ans = walsh_w0_14_8(mVector, ibits); break;
    case 9: ans = walsh_w0_14_9(mVector, ibits); break;
    case 10: ans = walsh_w0_14_10(mVector, ibits); break;
    case 11: ans = walsh_w0_14_11(mVector, ibits); break;
    case 12: ans = walsh_w0_14_12(mVector, ibits); break;
    case 13: ans = walsh_w0_14_13(mVector, ibits); break;
    case 14: ans = walsh_w0_14_14(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 15:
    switch ( w ) {
    case 0: ans = walsh_w0_15_0(mVector, ibits); break;
    case 1: ans = walsh_w0_15_1(mVector, ibits); break;
    case 2: ans = walsh_w0_15_2(mVector, ibits); break;
    case 3: ans = walsh_w0_15_3(mVector, ibits); break;
    case 4: ans = walsh_w0_15_4(mVector, ibits); break;
    case 5: ans = walsh_w0_15_5(mVector, ibits); break;
    case 6: ans = walsh_w0_15_6(mVector, ibits); break;
    case 7: ans = walsh_w0_15_7(mVector, ibits); break;
    case 8: ans = walsh_w0_15_8(mVector, ibits); break;
    case 9: ans = walsh_w0_15_9(mVector, ibits); break;
    case 10: ans = walsh_w0_15_10(mVector, ibits); break;
    case 11: ans = walsh_w0_15_11(mVector, ibits); break;
    case 12: ans = walsh_w0_15_12(mVector, ibits); break;
    case 13: ans = walsh_w0_15_13(mVector, ibits); break;
    case 14: ans = walsh_w0_15_14(mVector, ibits); break;
    case 15: ans = walsh_w0_15_15(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 16:
    switch ( w ) {
    case 0: ans = walsh_w0_16_0(mVector, ibits); break;
    case 1: ans = walsh_w0_16_1(mVector, ibits); break;
    case 2: ans = walsh_w0_16_2(mVector, ibits); break;
    case 3: ans = walsh_w0_16_3(mVector, ibits); break;
    case 4: ans = walsh_w0_16_4(mVector, ibits); break;
    case 5: ans = walsh_w0_16_5(mVector, ibits); break;
    case 6: ans = walsh_w0_16_6(mVector, ibits); break;
    case 7: ans = walsh_w0_16_7(mVector, ibits); break;
    case 8: ans = walsh_w0_16_8(mVector, ibits); break;
    case 9: ans = walsh_w0_16_9(mVector, ibits); break;
    case 10: ans = walsh_w0_16_10(mVector, ibits); break;
    case 11: ans = walsh_w0_16_11(mVector, ibits); break;
    case 12: ans = walsh_w0_16_12(mVector, ibits); break;
    case 13: ans = walsh_w0_16_13(mVector, ibits); break;
    case 14: ans = walsh_w0_16_14(mVector, ibits); break;
    case 15: ans = walsh_w0_16_15(mVector, ibits); break;
    case 16: ans = walsh_w0_16_16(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 17:
    switch ( w ) {
    case 0: ans = walsh_w0_17_0(mVector, ibits); break;
    case 1: ans = walsh_w0_17_1(mVector, ibits); break;
    case 2: ans = walsh_w0_17_2(mVector, ibits); break;
    case 3: ans = walsh_w0_17_3(mVector, ibits); break;
    case 4: ans = walsh_w0_17_4(mVector, ibits); break;
    case 5: ans = walsh_w0_17_5(mVector, ibits); break;
    case 6: ans = walsh_w0_17_6(mVector, ibits); break;
    case 7: ans = walsh_w0_17_7(mVector, ibits); break;
    case 8: ans = walsh_w0_17_8(mVector, ibits); break;
    case 9: ans = walsh_w0_17_9(mVector, ibits); break;
    case 10: ans = walsh_w0_17_10(mVector, ibits); break;
    case 11: ans = walsh_w0_17_11(mVector, ibits); break;
    case 12: ans = walsh_w0_17_12(mVector, ibits); break;
    case 13: ans = walsh_w0_17_13(mVector, ibits); break;
    case 14: ans = walsh_w0_17_14(mVector, ibits); break;
    case 15: ans = walsh_w0_17_15(mVector, ibits); break;
    case 16: ans = walsh_w0_17_16(mVector, ibits); break;
    case 17: ans = walsh_w0_17_17(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 18:
    switch ( w ) {
    case 0: ans = walsh_w0_18_0(mVector, ibits); break;
    case 1: ans = walsh_w0_18_1(mVector, ibits); break;
    case 2: ans = walsh_w0_18_2(mVector, ibits); break;
    case 3: ans = walsh_w0_18_3(mVector, ibits); break;
    case 4: ans = walsh_w0_18_4(mVector, ibits); break;
    case 5: ans = walsh_w0_18_5(mVector, ibits); break;
    case 6: ans = walsh_w0_18_6(mVector, ibits); break;
    case 7: ans = walsh_w0_18_7(mVector, ibits); break;
    case 8: ans = walsh_w0_18_8(mVector, ibits); break;
    case 9: ans = walsh_w0_18_9(mVector, ibits); break;
    case 10: ans = walsh_w0_18_10(mVector, ibits); break;
    case 11: ans = walsh_w0_18_11(mVector, ibits); break;
    case 12: ans = walsh_w0_18_12(mVector, ibits); break;
    case 13: ans = walsh_w0_18_13(mVector, ibits); break;
    case 14: ans = walsh_w0_18_14(mVector, ibits); break;
    case 15: ans = walsh_w0_18_15(mVector, ibits); break;
    case 16: ans = walsh_w0_18_16(mVector, ibits); break;
    case 17: ans = walsh_w0_18_17(mVector, ibits); break;
    case 18: ans = walsh_w0_18_18(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 19:
    switch ( w ) {
    case 0: ans = walsh_w0_19_0(mVector, ibits); break;
    case 1: ans = walsh_w0_19_1(mVector, ibits); break;
    case 2: ans = walsh_w0_19_2(mVector, ibits); break;
    case 3: ans = walsh_w0_19_3(mVector, ibits); break;
    case 4: ans = walsh_w0_19_4(mVector, ibits); break;
    case 5: ans = walsh_w0_19_5(mVector, ibits); break;
    case 6: ans = walsh_w0_19_6(mVector, ibits); break;
    case 7: ans = walsh_w0_19_7(mVector, ibits); break;
    case 8: ans = walsh_w0_19_8(mVector, ibits); break;
    case 9: ans = walsh_w0_19_9(mVector, ibits); break;
    case 10: ans = walsh_w0_19_10(mVector, ibits); break;
    case 11: ans = walsh_w0_19_11(mVector, ibits); break;
    case 12: ans = walsh_w0_19_12(mVector, ibits); break;
    case 13: ans = walsh_w0_19_13(mVector, ibits); break;
    case 14: ans = walsh_w0_19_14(mVector, ibits); break;
    case 15: ans = walsh_w0_19_15(mVector, ibits); break;
    case 16: ans = walsh_w0_19_16(mVector, ibits); break;
    case 17: ans = walsh_w0_19_17(mVector, ibits); break;
    case 18: ans = walsh_w0_19_18(mVector, ibits); break;
    case 19: ans = walsh_w0_19_19(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED; break;
    break;

  case 20:
    switch ( w ) {
    case 0: ans = walsh_w0_20_0(mVector, ibits); break;
    case 1: ans = walsh_w0_20_1(mVector, ibits); break;
    case 2: ans = walsh_w0_20_2(mVector, ibits); break;
    case 3: ans = walsh_w0_20_3(mVector, ibits); break;
    case 4: ans = walsh_w0_20_4(mVector, ibits); break;
    case 5: ans = walsh_w0_20_5(mVector, ibits); break;
    case 6: ans = walsh_w0_20_6(mVector, ibits); break;
    case 7: ans = walsh_w0_20_7(mVector, ibits); break;
    case 8: ans = walsh_w0_20_8(mVector, ibits); break;
    case 9: ans = walsh_w0_20_9(mVector, ibits); break;
    case 10: ans = walsh_w0_20_10(mVector, ibits); break;
    case 11: ans = walsh_w0_20_11(mVector, ibits); break;
    case 12: ans = walsh_w0_20_12(mVector, ibits); break;
    case 13: ans = walsh_w0_20_13(mVector, ibits); break;
    case 14: ans = walsh_w0_20_14(mVector, ibits); break;
    case 15: ans = walsh_w0_20_15(mVector, ibits); break;
    case 16: ans = walsh_w0_20_16(mVector, ibits); break;
    case 17: ans = walsh_w0_20_17(mVector, ibits); break;
    case 18: ans = walsh_w0_20_18(mVector, ibits); break;
    case 19: ans = walsh_w0_20_19(mVector, ibits); break;
    case 20: ans = walsh_w0_20_20(mVector, ibits); break;
    }
    ASSERT_NOT_REACHED;
    break;
#endif
  default: // input_num() > 20
    {
      int nall = 0;
      int c = 0;
      ymuint ibits1 = ibits >> NIPW;
      ymuint ibits2 = ibits & 0x001F;
      for (ymuint u = 0; u <= w; ++ u ) {
	ymuint v = w - u;
	if ( v > NIPW ) continue;
	// u: ブロック番号中の1の重み
	// v: ブロックの中の1の重み
	ymuint start1 = pidx[u];
	ymuint end1 = pidx[u + 1];
	ymuint start2 = s_pidx[v];
	ymuint end2 = s_pidx[v + 1];
	ymuint n2 = end2 - start2;
	ymuint pat = 0;
	ymuint* endp2 = &s_plist[end2];
	for (ymuint* p2 = &s_plist[start2]; p2 != endp2; ++ p2) {
	  ymuint pos2 = *p2 ^ ibits2;
	  pat |= (1UL << pos2);
	}
	ymuint* endp1 = &plist[end1];
	for (ymuint* p1 = &plist[start1]; p1 != endp1; ++ p1) {
	  ymuint pos0 = *p1;
	  if ( pos0 >= mBlockNum ) break;
	  ymuint pos1 = pos0 ^ ibits1;
	  c += count_onebits(mVector[pos1] & pat);
	  nall += n2;
	}
      }
      ans = nall - c * 2;
    }
  }

  if ( oinv ) {
    ans = -ans;
  }
  return ans;
}

// 重み別の 1 次の Walsh 係数を求める．
int
TvFunc::walsh_w1(VarId var,
		 ymuint w,
		 bool oinv,
		 ymuint ibits) const
{
  ymuint idx = var.val();
  switch ( input_num() ) {
  case 2:
    {
      int tmp = mVector[0];
      if ( oinv ) {
	tmp ^= 0x0000000F;
      }
      if ( ibits & (1 << idx) ) {
	tmp ^= ~c_masks[idx];
      }
      else {
	tmp ^= c_masks[idx];
      }
      ymuint mask;
      switch ( w ) {
      case 0:
	mask = (1 << (0 ^ ibits));
	return 1 - count_onebits_2(tmp & mask) * 2;
      case 1:
	mask = (1 << (1 ^ ibits)) | (1 << (2 ^ ibits));
	return 2 - count_onebits_2(tmp & mask) * 2;
      case 2:
	mask = (1 << (3 ^ ibits));
	return 1 - count_onebits_2(tmp & mask) * 2;
      }
    }
    break;
  case 3:
    {
      int tmp = mVector[0];
      if ( oinv ) {
	tmp ^= 0x000000FF;
      }
      if ( ibits & (1 << idx) ) {
	tmp ^= ~c_masks[idx];
      }
      else {
	tmp ^= c_masks[idx];
      }
      ymuint mask;
      switch ( w ) {
      case 0:
	mask = (1 << (0 ^ ibits));
	return 1 - count_onebits_3(tmp & mask) * 2;
      case 1:
	mask = (1 << (1 ^ ibits)) | (1 << (2 ^ ibits)) | (1 << (4 ^ ibits));
	return 3 - count_onebits_3(tmp & mask) * 2;
      case 2:
	mask = (1 << (3 ^ ibits)) | (1 << (5 ^ ibits)) | (1 << (6 ^ ibits));
	return 3 - count_onebits_3(tmp & mask) * 2;
      case 3:
	mask = (1 << (7 ^ ibits));
	return 1 - count_onebits_3(tmp & mask) * 2;
      }
    }
    break;
  case 4:
    {
      int tmp = mVector[0];
      if ( oinv ) {
	tmp ^= 0x0000FFFF;
      }
      if ( ibits & (1 << idx) ) {
	tmp ^= ~c_masks[idx];
      }
      else {
	tmp ^= c_masks[idx];
      }
      ymuint mask;
      switch ( w ) {
      case 0:
	mask = (1 << (0 ^ ibits));
	return 1 - count_onebits_4(tmp & mask) * 2;
      case 1:
	mask =
	  (1 << (1 ^ ibits)) | (1 << (2 ^ ibits)) | (1 << (4 ^ ibits)) |
	  (1 << (8 ^ ibits));
	return 4 - count_onebits_4(tmp & mask) * 2;
      case 2:
	mask =
	  (1 << (3 ^ ibits)) | (1 << (5 ^ ibits)) | (1 << (6 ^ ibits)) |
	  (1 << (9 ^ ibits)) | (1 << (10 ^ ibits)) | (1 << (12 ^ ibits));
	return 6 - count_onebits_4(tmp & mask) * 2;
      case 3:
	mask =
	  (1 << (7 ^ ibits)) | (1 << (11 ^ ibits)) |
	  (1 << (13 ^ ibits)) | (1 << (14 ^ ibits));
	return 4 - count_onebits_4(tmp & mask) * 2;
      case 4:
	mask = (1 << (15 ^ ibits));
	return 1 - count_onebits_4(tmp & mask) * 2;
      }
    }
    break;
  case 5:
    {
      int tmp;
      if ( oinv ) {
	tmp = ~mVector[0];
      }
      else {
	tmp = mVector[0];
      }
      if ( ibits & (1 << idx) ) {
	tmp ^= ~c_masks[idx];
      }
      else {
	tmp ^= c_masks[idx];
      }
      int nall;
      int c;
      switch ( w ) {
      case 0:
	nall = 1;
	c = (tmp >> (0 ^ ibits)) & 1;
	break;
      case 1:
	nall = 5;
	c  = (tmp >> (1 ^ ibits)) & 1;
	c += (tmp >> (2 ^ ibits)) & 1;
	c += (tmp >> (4 ^ ibits)) & 1;
	c += (tmp >> (8 ^ ibits)) & 1;
	c += (tmp >> (16 ^ ibits)) & 1;
	break;
      case 2:
	nall = 10;
	c  = (tmp >> (3 ^ ibits)) & 1;
	c += (tmp >> (5 ^ ibits)) & 1;
	c += (tmp >> (6 ^ ibits)) & 1;
	c += (tmp >> (9 ^ ibits)) & 1;
	c += (tmp >> (10 ^ ibits)) & 1;
	c += (tmp >> (12 ^ ibits)) & 1;
	c += (tmp >> (17 ^ ibits)) & 1;
	c += (tmp >> (18 ^ ibits)) & 1;
	c += (tmp >> (20 ^ ibits)) & 1;
	c += (tmp >> (24 ^ ibits)) & 1;
	break;
      case 3:
	nall = 10;
	c  = (tmp >> (7 ^ ibits)) & 1;
	c += (tmp >> (11 ^ ibits)) & 1;
	c += (tmp >> (13 ^ ibits)) & 1;
	c += (tmp >> (14 ^ ibits)) & 1;
	c += (tmp >> (19 ^ ibits)) & 1;
	c += (tmp >> (21 ^ ibits)) & 1;
	c += (tmp >> (22 ^ ibits)) & 1;
	c += (tmp >> (25 ^ ibits)) & 1;
	c += (tmp >> (26 ^ ibits)) & 1;
	c += (tmp >> (28 ^ ibits)) & 1;
	break;
      case 4:
	nall = 5;
	c  = (tmp >> (15 ^ ibits)) & 1;
	c += (tmp >> (23 ^ ibits)) & 1;
	c += (tmp >> (27 ^ ibits)) & 1;
	c += (tmp >> (29 ^ ibits)) & 1;
	c += (tmp >> (30 ^ ibits)) & 1;
	break;
      case 5:
	nall = 1;
	c = (tmp >> (31 ^ ibits)) & 1;
	break;
      default:
	ASSERT_NOT_REACHED;
	nall = 0;
	c = 0;
      }
      return nall - c * 2;
    }
    break;
  default: // input_num() > 5
    ;
  }

  int n = 0;
  int c = 0;
  ymuint ibits1 = ibits >> NIPW;
  ymuint ibits2 = ibits & ((1UL << NIPW) - 1UL);
  if ( idx < NIPW ) {
    ymuint mask2;
    if ( ibits2 & (1 << idx) ) {
      mask2 = ~c_masks[idx];
    }
    else {
      mask2 = c_masks[idx];
    }
    for (ymuint u = 0; u <= w; ++ u ) {
      ymuint v = w - u;
      if ( v > NIPW ) continue;
      // u: ブロック番号中の1の重み
      // v: ブロックの中の1の重み
      ymuint start1 = pidx[u];
      ymuint end1 = pidx[u + 1];
      ymuint start2 = s_pidx[v];
      ymuint end2 = s_pidx[v + 1];
      ymuint n2 = end2 - start2;
      ymuint pat = 0;
      ymuint* endp2 = &s_plist[end2];
      for (ymuint* p2 = &s_plist[start2]; p2 != endp2; ++ p2) {
	ymuint pos2 = *p2 ^ ibits2;
	pat |= (1UL << pos2);
      }
      ymuint* endp1 = &plist[end1];
      for (ymuint* p1 = &plist[start1]; p1 != endp1; ++ p1) {
	ymuint pos0 = *p1;
	if ( pos0 >= mBlockNum ) break;
	ymuint pos1 = pos0 ^ ibits1;
	c += count_onebits((mVector[pos1] ^ mask2) & pat);
	n += n2;
      }
    }
  }
  else {
    ymuint var5 = idx - NIPW;
    for (ymuint u = 0; u <= w; ++ u ) {
      ymuint v = w - u;
      if ( v > NIPW ) continue;
      // u: ブロック番号中の1の重み
      // v: ブロックの中の1の重み
      ymuint start1 = pidx[u];
      ymuint end1 = pidx[u + 1];
      ymuint start2 = s_pidx[v];
      ymuint end2 = s_pidx[v + 1];
      ymuint n2 = end2 - start2;
      ymuint pat = 0;
      ymuint* endp2 = &s_plist[end2];
      for (ymuint* p2 = &s_plist[start2]; p2 != endp2; ++ p2) {
	ymuint pos2 = *p2 ^ ibits2;
	pat |= (1UL << pos2);
      }
      ymuint* endp1 = &plist[end1];
      for (ymuint* p1 = &plist[start1]; p1 != endp1; ++ p1) {
	ymuint pos0 = *p1;
	if ( pos0 >= mBlockNum ) break;
	ymuint pos1 = pos0 ^ ibits1;
	// 2行下の式は1行下の式と同じ意味
	// ymuint mask3 = (pos0 & (1 << var5)) ? 0xFFFFFFFF : 0x00000000;
	ymuint mask3 = 0UL - ((pos0 >> var5) & 1UL);
	c += count_onebits((mVector[pos1] ^ mask3) & pat);
	n += n2;
      }
    }
  }

  int ans = n - c * 2;
  if ( oinv ) {
    ans = -ans;
  }
  return ans;
}

// 変数がサポートの時 true を返す．
bool
TvFunc::check_sup(VarId var) const
{
  ymuint i = var.val();
  if ( i < NIPW ) {
    // ブロックごとにチェック
    ymuint dist = 1 << i;
    ymuint64 mask = c_masks[i];
    for (ymuint b = 0; b < mBlockNum; ++ b) {
      ymuint64 word = mVector[b];
      if ( (word ^ (word << dist)) & mask ) {
	return true;
      }
    }
  }
  else {
    // ブロック単位でチェック
    ymuint i5 = i - NIPW;
    ymuint check = 1 << i5;
    for (ymuint b = 0; b < mBlockNum; ++ b) {
      if ( (b & check) && (mVector[b] != mVector[b ^ check]) ) {
	return true;
      }
    }
  }
  return false;
}

// i 番目と j 番目の変数が対称のとき true を返す．
bool
TvFunc::check_sym(VarId var1,
		  VarId var2,
		  bool inv) const
{
  ymuint i = var1.val();
  ymuint j = var2.val();

  // i と j を正規化する．
  if ( i < j ) {
    ymuint tmp = i;
    i = j;
    j = tmp;
  }
  // ここ以降では必ず i > j が成り立つ．

  bool ans = true;
  if ( j >= NIPW ) {
    // i >= NIPW (実際には i > NIPW)
    // j >= NIPW
    // ブロック単位で比較する．
    ymuint mask_i = (1 << (i - NIPW));
    ymuint mask_j = (1 << (j - NIPW));
    ymuint mask_all = mask_i | mask_j;
    ymuint cond;
    if ( inv ) {
      cond = 0UL;
    }
    else {
      cond = mask_j;
    }
    for (ymuint v = 0; v < mBlockNum; ++ v) {
      if ( (v & mask_all) == cond &&
	   mVector[v] != mVector[v ^ mask_all] ) {
	ans = false;
	break;
      }
    }
  }
  else if ( i >= NIPW ) {
    // i >= NIPW
    // j < NIPW
    ymuint mask_i = (1 << (i - NIPW));
    ymuint cond;
    if ( inv ) {
      cond = 0UL;
    }
    else {
      cond = mask_i;
    }
    ymuint64 mask2 = ~c_masks[j];
    ymuint s = 1 << j;
    for (ymuint v = 0; v < mBlockNum; ++ v) {
      if ( (v & mask_i) == cond &&
	   (mVector[v] ^ (mVector[v ^ mask_i] >> s)) & mask2 ) {
	ans = false;
	break;
      }
    }
  }
  else {
    // i < NIPW
    // j < NIPW
    if ( inv ) {
      ymuint64 mask = sym_masks3[(i * (i - 1)) / 2 + j];
      ymuint s = (1 << i) + (1 << j);
      for (ymuint b = 0; b < mBlockNum; ++ b) {
	ymuint64 word = mVector[b];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
    else {
      ymuint64 mask = sym_masks2[(i * (i - 1)) / 2 + j];
      ymuint s = (1 << i) - (1 << j);
      for (ymuint b = 0; b < mBlockNum; ++ b) {
	ymuint64 word = mVector[b];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  return ans;
}

// npnmap に従った変換を行う．
TvFunc
TvFunc::xform(const NpnMap& npnmap) const
{
  ymuint ni_pow = 1UL << mInputNum;

#if defined(DEBUG)
  cout << "xform" << endl
       << *this << endl
       << npnmap << endl;
#endif

  ymuint imask = 0UL;
  ymuint ipat[kMaxNi];
  for (ymuint i = 0; i < mInputNum; ++ i) {
    VarId src_var(i);
    NpnVmap imap = npnmap.imap(src_var);
    if ( imap.inv() ) {
      imask |= (1UL << i);
    }
    VarId dst_var = imap.var();
    ymuint j = dst_var.val();
    ipat[i] = 1UL << j;
  }
  ymuint omask = npnmap.oinv() ? 1U : 0U;

  TvFunc ans(mInputNum);
  for (ymuint i = 0; i < ni_pow; ++ i) {
    ymuint new_i = 0;
    ymuint tmp = i;
    for (ymuint b = 0; b < mInputNum; ++ b, tmp >>= 1) {
      if ( tmp & 1 ) {
	new_i |= ipat[b];
      }
    }
    ymuint64 pat = (value(i ^ imask) ^ omask);
    ans.mVector[block(new_i)] |= pat << shift(new_i);
  }

#if defined(DEBUG)
  cout << ans << endl;
#endif

  return ans;
}

// ハッシュ値を返す．
ymuint
TvFunc::hash() const
{
  ymuint64 ans = 0;
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    ans ^= mVector[i];
  }
  return ans + mInputNum;
}

// 比較関数
int
compare(const TvFunc& func1,
	const TvFunc& func2)
{
  // まず入力数を比較する．
  if ( func1.mInputNum < func2.mInputNum ) {
    return -1;
  }
  if ( func1.mInputNum > func2.mInputNum ) {
    return 1;
  }

  // 以降は入力数が等しい場合
  ymuint n = func1.mBlockNum;
  for (ymuint i = 0; i < n; ++ i) {
    ymuint64 w1 = func1.mVector[n - i - 1];
    ymuint64 w2 = func2.mVector[n - i - 1];
    if ( w1 < w2 ) {
      return -1;
    }
    if ( w1 > w2 ) {
      return 1;
    }
  }
  return 0;
}

// @relates TvFunc
// @brief 交差チェック
bool
operator&&(const TvFunc& func1,
	   const TvFunc& func2)
{
  if ( func1.mInputNum != func2.mInputNum ) {
    return false;
  }
  ymuint n = func1.mBlockNum;
  for (ymuint i = 0; i < n; ++ i) {
    ymuint64 w1 = func1.mVector[n - i - 1];
    ymuint64 w2 = func2.mVector[n - i - 1];
    if ( (w1 & w2) != 0U ) {
      return true;
    }
  }
  return false;
}

// 内容の出力
// mode は 2 か 16
void
TvFunc::print(ostream& s,
	      int mode) const
{
  ymuint ni_pow = 1UL << mInputNum;
  const ymuint wordsize = sizeof(ymuint64) * 8;
  if ( mode == 2 ) {
    ymuint64* bp = mVector;
    ymuint offset = 0;
    ymuint64 tmp = *bp;
    for (ymuint i = 0; i < ni_pow; ++ i) {
      s << (tmp & 1L);
      tmp >>= 1;
      ++ offset;
      if ( offset == wordsize ) {
	offset = 0;
	++ bp;
	tmp = *bp;
      }
    }
  }
  else if ( mode == 16 ) {
    ymuint ni_pow4 = ni_pow / 4;
    ymuint64* bp = mVector;
    ymuint offset = 0;
    ymuint64 tmp = *bp;
    for (ymuint i = 0; i < ni_pow4; ++ i) {
      ymuint64 tmp1 = (tmp & 0xF);
      if ( tmp1 < 10 ) {
	s << static_cast<char>('0' + tmp1);
      }
      else {
	s << static_cast<char>('A' + tmp1 - 10);
      }
      tmp >>= 4;
      offset += 4;
      if ( offset == wordsize ) {
	offset = 0;
	++ bp;
	tmp = *bp;
      }
    }
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief バイナリファイルの書き出し
// @param[in] s 出力先のストリーム
void
TvFunc::dump(ODO& s) const
{
  s << mInputNum;
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    s << mVector[i];
  }
}

// @brief バイナリファイルの読み込み
// @param[in] s 入力元のストリーム
void
TvFunc::restore(IDO& s)
{
  s >> mInputNum;
  ymuint32 nblk = nblock(mInputNum);
  if ( mBlockNum != nblk ) {
    delete [] mVector;
    mBlockNum = nblk;
    mVector = new ymuint64[mBlockNum];
  }
  for (ymuint i = 0; i < mBlockNum; ++ i) {
    s >> mVector[i];
  }
}

END_NAMESPACE_YM
