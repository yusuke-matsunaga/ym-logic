
/// @file TvFunc.cc
/// @brief TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/Range.h"


// 1 ワード当たりの入力数
#define NIPW 6

//#define DEBUG

BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 0 - 63 までの数をその数を2進表記したときの1のビット数ごとにならべたもの
// 以下行ごとに 0, 1, 2, 3, 4, 5, 6 ビットの順になっている．
int s_plist[] = {
   0,
   1,  2,  4,  8, 16, 32,
   3,  5,  6,  9, 10, 12, 17, 18, 20, 24, 33, 34, 36, 40, 48,
   7, 11, 13, 14, 19, 21, 22, 25, 26, 28, 35, 37, 38, 41, 42, 44, 49, 50, 52, 56,
  15, 23, 27, 29, 30, 39, 43, 45, 46, 51, 53, 54, 57, 58, 60,
  31, 47, 55, 59, 61, 62,
  63
};

// s_plist 中の各行の最初の位置を記したリスト
// sentinel として 7 つめの要素がある．
int s_pidx[] = {
  0, 1, 7, 22, 42, 57, 63, 64
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// word の中の 1 のビットを数える．
// 0入力用
inline
int
count_onebits_0(
  TvFunc::WordType word
)
{
  return static_cast<int>(word & 0x1ULL);
}

// word の中の 1 のビットを数える．
// 1入力用
inline
int
count_onebits_1(
  TvFunc::WordType word
)
{
  const TvFunc::WordType mask1 = 0x1ULL;

  word = (word & mask1) + ((word >> 1) & mask1);
  return static_cast<int>(word);
}

// word の中の 1 のビットを数える．
// 2入力用
inline
int
count_onebits_2(
  TvFunc::WordType word
)
{
  const TvFunc::WordType mask1  = 0x5ULL;
  const TvFunc::WordType mask2  = 0x3ULL;

  word = (word & mask1) + ((word >> 1) & mask1);
  word = (word & mask2) + ((word >> 2) & mask2);
  return static_cast<int>(word);
}

// word の中の 1 のビットを数える．
// 3入力用
inline
int
count_onebits_3(
  TvFunc::WordType word
)
{
  const TvFunc::WordType mask1  = 0x55ULL;
  const TvFunc::WordType mask2  = 0x33ULL;
  const TvFunc::WordType mask4  = 0x0FULL;

  word = (word & mask1) + ((word >> 1) & mask1);
  word = (word & mask2) + ((word >> 2) & mask2);
  word = (word & mask4) + ((word >> 4) & mask4);
  return static_cast<int>(word);
}

// word の中の 1 のビットを数える．
// 4入力用
inline
int
count_onebits_4(
  TvFunc::WordType word
)
{
  const TvFunc::WordType mask1  = 0x5555ULL;
  const TvFunc::WordType mask2  = 0x3333ULL;
  const TvFunc::WordType mask4  = 0x0f0FULL;
  const TvFunc::WordType mask8  = 0x00FFULL;

  word = (word & mask1) + ((word >> 1) & mask1);
  word = (word & mask2) + ((word >> 2) & mask2);
  word = (word & mask4) + ((word >> 4) & mask4);
  word = (word & mask8) + ((word >> 8) & mask8);
  return static_cast<int>(word);
}

// word の中の 1 のビットを数える．
// 5入力用
inline
int
count_onebits_5(
  TvFunc::WordType word
)
{
  const TvFunc::WordType mask1   = 0x55555555ULL;
  const TvFunc::WordType mask2   = 0x33333333ULL;
  const TvFunc::WordType mask4   = 0x0F0F0F0FULL;
  const TvFunc::WordType mask8   = 0x00FF00FFULL;
  const TvFunc::WordType mask16  = 0x0000FFFFULL;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  return static_cast<int>(word);
}

// word の中の 1 のビットを数える．
// 6入力用
inline
int
count_onebits_6(
  TvFunc::WordType word
)
{
  const TvFunc::WordType mask1  = 0x5555555555555555ULL;
  const TvFunc::WordType mask2  = 0x3333333333333333ULL;
  const TvFunc::WordType mask4  = 0x0F0F0F0F0F0F0F0FULL;
  const TvFunc::WordType mask8  = 0x00FF00FF00FF00FFULL;
  const TvFunc::WordType mask16 = 0x0000FFFF0000FFFFULL;
  const TvFunc::WordType mask32 = 0x00000000FFFFFFFFULL;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  word = (word & mask32) + ((word >> 32) & mask32);
  return static_cast<int>(word);
}

// word の中の 1 のビットを数える．
inline
int
count_onebits(
  TvFunc::WordType word
)
{
  return count_onebits_6(word);
}

END_NONAMESPACE

// 0 の数を数える．
SizeType
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

  SizeType ans = 0;
  for ( SizeType b: Range(mBlockNum) ) {
    ans += count_onebits(mVector[b]);
  }
  return (1 << input_num()) - ans;
}

// 1 の数を数える．
SizeType
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

  SizeType ans = 0;
  for ( SizeType b: Range(mBlockNum) ) {
    ans += count_onebits(mVector[b]);
  }
  return ans;
}

// 0次の Walsh 係数を求める．
int
TvFunc::walsh_0() const
{
  if ( is_invalid() ) {
    return 0;
  }

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

  int ans = 0;
  for ( SizeType b: Range(mBlockNum) ) {
    ans += count_onebits(mVector[b]);
  }
  return (1 << input_num()) - ans * 2;
}

// 1次の Walsh 係数を求める．
int
TvFunc::walsh_1(
  SizeType varid
) const
{
  if ( is_invalid() ) {
    return 0;
  }

  check_varid(varid);

  switch ( input_num() ) {
  case 0: ASSERT_NOT_REACHED;
  case 1: return (1 << 1) - count_onebits_1(mVector[0] ^ c_mask(varid)) * 2;
  case 2: return (1 << 2) - count_onebits_2(mVector[0] ^ c_mask(varid)) * 2;
  case 3: return (1 << 3) - count_onebits_3(mVector[0] ^ c_mask(varid)) * 2;
  case 4: return (1 << 4) - count_onebits_4(mVector[0] ^ c_mask(varid)) * 2;
  case 5: return (1 << 5) - count_onebits_5(mVector[0] ^ c_mask(varid)) * 2;
  case 6: return (1 << 6) - count_onebits_6(mVector[0] ^ c_mask(varid)) * 2;

  default:
    ;
  }

  // n > 6
  SizeType c = 0;
  int n = 1 << input_num();
  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType mask = lit_pat(varid, b);
    c += count_onebits(mVector[b] ^ mask);
  }
  return n - c * 2;
}

// 2次の Walsh 係数を求める．
int
TvFunc::walsh_2(
  SizeType var1,
  SizeType var2
) const
{
  if ( is_invalid() ) {
    return 0;
  }

  check_varid(var1, "var1");
  check_varid(var2, "var2");

  if ( var1 == var2 ) {
    return 0;
  }

  switch ( input_num() ) {
  case 0:
  case 1: ASSERT_NOT_REACHED;
  case 2: return (1 << 2) - count_onebits_2(mVector[0] ^ c_mask(var1) ^ c_mask(var2)) * 2;
  case 3: return (1 << 3) - count_onebits_3(mVector[0] ^ c_mask(var1) ^ c_mask(var2)) * 2;
  case 4: return (1 << 4) - count_onebits_4(mVector[0] ^ c_mask(var1) ^ c_mask(var2)) * 2;
  case 5: return (1 << 5) - count_onebits_5(mVector[0] ^ c_mask(var1) ^ c_mask(var2)) * 2;
  case 6: return (1 << 6) - count_onebits_6(mVector[0] ^ c_mask(var1) ^ c_mask(var2)) * 2;
  }

  // var1 と var2 を正規化する．
  if ( var1 < var2 ) {
    std::swap(var1, var2);
  }

  SizeType c = 0;
  if ( var1 < NIPW ) {
    TvFunc::WordType mask = c_mask(var1) ^ c_mask(var2);
    for ( SizeType b: Range(mBlockNum) ) {
      c += count_onebits(mVector[b] ^ mask);
    }
  }
  else if ( var2 < NIPW ) {
    // int check = 1 << (var1 - 5);
    // int mask1 = c_mask(var2);
    // int mask0 = ~mask1;
    // for (int b: Range(mBlockNum)) {
    //   if ( b & check ) {
    //     c += count_onebits(mVector[b] ^ mask0);
    //   } else {
    //     c += count_onebits(mVector[b] ^ mask1);
    //   }
    // }
    SizeType i5 = var1 - NIPW;
    TvFunc::WordType mask = c_mask(var2);
    for ( int b: Range(mBlockNum) ) {
      TvFunc::WordType mask1 = 0UL - ((b >> i5) & 1UL);
      c += count_onebits(mVector[b] ^ mask ^ mask1);
    }
  }
  else {
    // int check1 = 1 << (var1 - 5);
    // int check2 = 1 << (var2 - 5);
    // int check = check1 | check2;
    // for (int b: Range(mBlockNum)) {
    //   int tmp = b & check;
    //   if ( tmp == check1 || tmp == check2 ) {
    //     c += count_onebits(~mVector[b]);
    //   } else {
    //     c += count_onebits(mVector[b]);
    //   }
    // }
    SizeType i5 = var1 - NIPW;
    SizeType j5 = var2 - NIPW;
    for ( SizeType b: Range(mBlockNum) ) {
      TvFunc::WordType mask = 0UL - (((b >> i5) ^ (b >> j5)) & 1UL);
      c += count_onebits(mVector[b] ^ mask);
    }
  }
  return (1 << input_num()) - c * 2;
}

BEGIN_NONAMESPACE

// 5入力の walsh_01 用サブルーティン
inline
int
walsh_01_5b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  const TvFunc::WordType mask1   = 0x55555555ULL;
  const TvFunc::WordType mask2   = 0x33333333ULL;
  const TvFunc::WordType mask4   = 0x0f0f0f0fULL;
  const TvFunc::WordType mask8   = 0x00ff00ffULL;
  const TvFunc::WordType mask16  = 0x0000ffffULL;

  TvFunc::WordType tmp;
  {
    tmp = src_vec[0];
    TvFunc::WordType tmp0 = tmp & mask1;
    TvFunc::WordType tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x55555555ULL;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[0] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask2;
    TvFunc::WordType tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x22222222ULL;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[1] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask4;
    TvFunc::WordType tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x04040404ULL;
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[2] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask8;
    TvFunc::WordType tmp1 = (tmp >> 8) & mask8;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x00080008ULL;
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    vec[3] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask16;
    TvFunc::WordType tmp1 = (tmp >> 16) & mask16;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x00000010ULL;
    vec[4] += tmp0;
  }

  return tmp;
}

// 6 入力の walsh_01 用サブルーティン
inline
int
walsh_01_6b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  const TvFunc::WordType mask1   = 0x5555555555555555ULL;
  const TvFunc::WordType mask2   = 0x3333333333333333ULL;
  const TvFunc::WordType mask4   = 0x0f0f0f0f0f0f0f0fULL;
  const TvFunc::WordType mask8   = 0x00ff00ff00ff00ffULL;
  const TvFunc::WordType mask16  = 0x0000ffff0000ffffULL;
  const TvFunc::WordType mask32  = 0x00000000ffffffffULL;

  TvFunc::WordType tmp;
  {
    tmp = src_vec[0];
    TvFunc::WordType tmp0 = tmp & mask1;
    TvFunc::WordType tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x5555555555555555ULL;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >> 2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >> 4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >> 8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[0] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask2;
    TvFunc::WordType tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x2222222222222222ULL;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >> 4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >> 8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[1] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask4;
    TvFunc::WordType tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0404040404040404ULL;
    tmp0 = (tmp0 & mask8)  + ((tmp0 >> 8) & mask8);
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[2] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask8;
    TvFunc::WordType tmp1 = (tmp >> 8) & mask8;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0008000800080008ULL;
    tmp0 = (tmp0 & mask16) + ((tmp0 >> 16) & mask16);
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[3] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask16;
    TvFunc::WordType tmp1 = (tmp >> 16) & mask16;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0000001000000010ULL;
    tmp0 = (tmp0 & mask32) + ((tmp0 >> 32) & mask32);
    vec[4] += tmp0;
  }

  {
    TvFunc::WordType tmp0 = tmp & mask32;
    TvFunc::WordType tmp1 = (tmp >> 32) & mask32;
    tmp =tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0000000000000020ULL;
    vec[5] += tmp0;
  }

  return tmp;
}

// 7入力の walsh_01 用サブルーティン
inline
int
walsh_01_7b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_6b(src_vec,                     vec);
  int ans1 = walsh_01_6b(src_vec + (1 << (6 - NIPW)), vec);
  vec[6] += ans0 + ((1 << 6) - ans1);
  return ans0 + ans1;
}

// 8入力の walsh_01 用サブルーティン
inline
int
walsh_01_8b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_7b(src_vec,                     vec);
  int ans1 = walsh_01_7b(src_vec + (1 << (7 - NIPW)), vec);
  vec[7] += ans0 + ((1 << 7) - ans1);
  return ans0 + ans1;
}

// 9入力の walsh_01 用サブルーティン
inline
int
walsh_01_9b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_8b(src_vec,                     vec);
  int ans1 = walsh_01_8b(src_vec + (1 << (8 - NIPW)), vec);
  vec[8] += ans0 + ((1 << 8) - ans1);
  return ans0 + ans1;
}

// 10入力の walsh_01 用サブルーティン
inline
int
walsh_01_10b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_9b(src_vec,                     vec);
  int ans1 = walsh_01_9b(src_vec + (1 << (9 - NIPW)), vec);
  vec[9] += ans0 + ((1 << 9) - ans1);
  return ans0 + ans1;
}

// 11入力の walsh_01 用サブルーティン
int
walsh_01_11b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_10b(src_vec,                      vec);
  int ans1 = walsh_01_10b(src_vec + (1 << (10 - NIPW)), vec);
  vec[10] += ans0 + ((1 << 10) - ans1);
  return ans0 + ans1;
}

// 12入力の walsh_01 用サブルーティン
int
walsh_01_12b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_11b(src_vec,                      vec);
  int ans1 = walsh_01_11b(src_vec + (1 << (11 - NIPW)), vec);
  vec[11] += ans0 + ((1 << 11) - ans1);
  return ans0 + ans1;
}

// 13入力の walsh_01 用サブルーティン
int
walsh_01_13b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_12b(src_vec,                      vec);
  int ans1 = walsh_01_12b(src_vec + (1 << (12 - NIPW)), vec);
  vec[12] += ans0 + ((1 << 12) - ans1);
  return ans0 + ans1;
}

// 14入力の walsh_01 用サブルーティン
int
walsh_01_14b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_13b(src_vec,                      vec);
  int ans1 = walsh_01_13b(src_vec + (1 << (13 - NIPW)), vec);
  vec[13] += ans0 + ((1 << 13) - ans1);
  return ans0 + ans1;
}

// 15入力の walsh_01 用サブルーティン
int
walsh_01_15b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_14b(src_vec,                      vec);
  int ans1 = walsh_01_14b(src_vec + (1 << (14 - NIPW)), vec);
  vec[14] += ans0 + ((1 << 14) - ans1);
  return ans0 + ans1;
}

// 16入力の walsh_01 用サブルーティン
int
walsh_01_16b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_15b(src_vec,                      vec);
  int ans1 = walsh_01_15b(src_vec + (1 << (15 - NIPW)), vec);
  vec[15] += ans0 + ((1 << 15) - ans1);
  return ans0 + ans1;
}

// 17入力の walsh_01 用サブルーティン
int
walsh_01_17b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_16b(src_vec,                      vec);
  int ans1 = walsh_01_16b(src_vec + (1 << (16 - NIPW)), vec);
  vec[16] += ans0 + ((1 << 16) - ans1);
  return ans0 + ans1;
}

// 18入力の walsh_01 用サブルーティン
int
walsh_01_18b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_17b(src_vec,                      vec);
  int ans1 = walsh_01_17b(src_vec + (1 << (17 - NIPW)), vec);
  vec[17] += ans0 + ((1 << 17) - ans1);
  return ans0 + ans1;
}

// 19入力の walsh_01 用サブルーティン
int
walsh_01_19b(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  int ans0 = walsh_01_18b(src_vec,                      vec);
  int ans1 = walsh_01_18b(src_vec + (1 << (18 - NIPW)), vec);
  vec[18] += ans0 + ((1 << 18) - ans1);
  return ans0 + ans1;
}

// 0入力の walsh_01 本体
inline
int
walsh_01_0(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  return 1 - src_vec[0] * 2;
}

// 1入力の walsh_01 本体
inline
int
walsh_01_1(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  const TvFunc::WordType mask1   = 0x1ULL;

  const int n = (1 << 1);

  TvFunc::WordType tmp;
  {
    tmp = src_vec[0];
    TvFunc::WordType tmp0 = (tmp >> 0) & mask1;
    TvFunc::WordType tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x1ULL;
    vec[0] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 2入力の walsh_01 本体
inline
int
walsh_01_2(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  const TvFunc::WordType mask1   = 0x5ULL;
  const TvFunc::WordType mask2   = 0x3ULL;

  const int n = (1 << 2);

  TvFunc::WordType tmp;
  {
    tmp = src_vec[0];
    TvFunc::WordType tmp0 = (tmp >> 0) & mask1;
    TvFunc::WordType tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x5ULL;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    vec[0] = n - tmp0 * 2;
  }

  {
    TvFunc::WordType tmp0 = (tmp >> 0) & mask2;
    TvFunc::WordType tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x2ULL;
    vec[1] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 3入力の walsh_01 本体
inline
int
walsh_01_3(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  const TvFunc::WordType mask1   = 0x55ULL;
  const TvFunc::WordType mask2   = 0x33ULL;
  const TvFunc::WordType mask4   = 0x0fULL;

  const int n = (1 << 3);

  TvFunc::WordType tmp;
  {
    tmp = src_vec[0];
    TvFunc::WordType tmp0 = (tmp >> 0) & mask1;
    TvFunc::WordType tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x55ULL;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    vec[0] = n - tmp0 * 2;
  }

  {
    TvFunc::WordType tmp0 = (tmp >> 0) & mask2;
    TvFunc::WordType tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x22ULL;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    vec[1] = n - tmp0 * 2;
  }

  {
    TvFunc::WordType tmp0 = (tmp >> 0) & mask4;
    TvFunc::WordType tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x04ULL;
    vec[2] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 4入力の walsh_01 本体
inline
int
walsh_01_4(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  const TvFunc::WordType mask1   = 0x5555ULL;
  const TvFunc::WordType mask2   = 0x3333ULL;
  const TvFunc::WordType mask4   = 0x0f0fULL;
  const TvFunc::WordType mask8   = 0x00ffULL;

  const int n = (1 << 4);

  TvFunc::WordType tmp;
  {
    tmp = src_vec[0];
    TvFunc::WordType tmp0 = (tmp >> 0) & mask1;
    TvFunc::WordType tmp1 = (tmp >> 1) & mask1;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x5555ULL;
    tmp0 = (tmp0 & mask2)  + ((tmp0 >>  2) & mask2);
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    vec[0] = n - tmp0 * 2;
  }

  {
    TvFunc::WordType tmp0 = (tmp >> 0) & mask2;
    TvFunc::WordType tmp1 = (tmp >> 2) & mask2;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x2222ULL;
    tmp0 = (tmp0 & mask4)  + ((tmp0 >>  4) & mask4);
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    vec[1] = n - tmp0 * 2;
  }

  {
    TvFunc::WordType tmp0 = (tmp >> 0) & mask4;
    TvFunc::WordType tmp1 = (tmp >> 4) & mask4;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0404ULL;
    tmp0 = (tmp0 & mask8)  + ((tmp0 >>  8) & mask8);
    vec[2] = n - tmp0 * 2;
  }

  {
    TvFunc::WordType tmp0 = (tmp >> 0) & mask8;
    TvFunc::WordType tmp1 = (tmp >> 8) & mask8;
    tmp = tmp0 + tmp1;
    tmp0 = tmp0 - tmp1 + 0x0008ULL;
    vec[3] = n - tmp0 * 2;
  }

  return n - tmp * 2;
}

// 5入力の walsh_01 本体
inline
int
walsh_01_5(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4} ) {
    vec[i] = 0;
  }

  int ans = walsh_01_5b(src_vec, vec);

  const int n = (1 << 5);
  for ( int i: {0, 1, 2, 3, 4} ) {
    vec[i] = n - vec[i] * 2;
  }
  return n - ans * 2;
}

// 6入力の walsh_01 本体
inline
int
walsh_01_6(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    vec[i] = 0;
  }

  int ans = walsh_01_6b(src_vec, vec);

  const int n = (1 << 6);
  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    vec[i] = n - vec[i] * 2;
  }
  return n - ans * 2;
}

// 7入力の walsh_01 本体
inline
int
walsh_01_7(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_6b(src_vec, vec);
  int ans1 = walsh_01_6b(src_vec + (1 << (6 - NIPW)), vec);

  const int n = (1 << 7);
  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[6] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 8入力の walsh_01 本体
inline
int
walsh_01_8(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_7b(src_vec, vec);
  int ans1 = walsh_01_7b(src_vec + (1 << (7 - NIPW)), vec);

  const int n = (1 << 8);
  for ( int i: {0, 1, 2, 3, 4, 5, 6} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[7] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 9入力の walsh_01 本体
inline
int
walsh_01_9(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_8b(src_vec, vec);
  int ans1 = walsh_01_8b(src_vec + (1 << (8 - NIPW)), vec);

  const int n = (1 << 9);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[8] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 10入力の walsh_01 本体
inline
int
walsh_01_10(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_9b(src_vec, vec);
  int ans1 = walsh_01_9b(src_vec + (1 << (9 - NIPW)), vec);

  const int n = (1 << 10);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[9] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 11入力の walsh_01 本体
int
walsh_01_11(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_10b(src_vec, vec);
  int ans1 = walsh_01_10b(src_vec + (1 << (10 - NIPW)), vec);

  const int n = (1 << 11);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[10] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 12入力の walsh_01 本体
int
walsh_01_12(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_11b(src_vec, vec);
  int ans1 = walsh_01_11b(src_vec + (1 << (11 - NIPW)), vec);

  const int n = (1 << 12);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[11] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 13入力の walsh_01 本体
int
walsh_01_13(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_12b(src_vec, vec);
  int ans1 = walsh_01_12b(src_vec + (1 << (12 - NIPW)), vec);

  const int n = (1 << 13);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[12] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 14入力の walsh_01 本体
int
walsh_01_14(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_13b(src_vec, vec);
  int ans1 = walsh_01_13b(src_vec + (1 << (13 - NIPW)), vec);

  const int n = (1 << 14);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[13] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 15入力の walsh_01 本体
int
walsh_01_15(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_14b(src_vec, vec);
  int ans1 = walsh_01_14b(src_vec + (1 << (14 - NIPW)), vec);

  const int n = (1 << 15);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[14] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 16入力の walsh_01 本体
int
walsh_01_16(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_15b(src_vec, vec);
  int ans1 = walsh_01_15b(src_vec + (1 << (15 - NIPW)), vec);

  const int n = (1 << 16);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[15] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 17入力の walsh_01 本体
int
walsh_01_17(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_16b(src_vec, vec);
  int ans1 = walsh_01_16b(src_vec + (1 << (16 - NIPW)), vec);

  const int n = (1 << 17);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[16] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 18入力の walsh_01 本体
int
walsh_01_18(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_17b(src_vec, vec);
  int ans1 = walsh_01_17b(src_vec + (1 << (17 - NIPW)), vec);

  const int n = (1 << 18);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[17] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 19入力の walsh_01 本体
int
walsh_01_19(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_18b(src_vec, vec);
  int ans1 = walsh_01_18b(src_vec + (1 << (18 - NIPW)), vec);

  const int n = (1 << 19);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[18] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

// 20入力の walsh_01 本体
int
walsh_01_20(
  TvFunc::WordType* src_vec,
  int vec[]
)
{
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18} ) {
    vec[i] = 0;
  }

  int ans0 = walsh_01_19b(src_vec, vec);
  int ans1 = walsh_01_19b(src_vec + (1 << (19 - NIPW)), vec);

  const int n = (1 << 20);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18} ) {
    vec[i] = n - vec[i] * 2;
  }
  vec[19] = (ans1 - ans0) * 2;
  return n - (ans0 + ans1) * 2;
}

END_NONAMESPACE

// 0次と 1次の Walsh 係数を求める．
int
TvFunc::walsh_01(
  int vec[]
) const
{
  if ( is_invalid() ) {
    return 0;
  }

  switch ( input_num() ) {
  case  0: return walsh_01_0(mVector, vec);
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
TvFunc::WordType
pm2_1(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask1   = 0x5ULL;
  const TvFunc::WordType offset1 = 0x5ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask1;
  TvFunc::WordType tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm2_2(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask2   = 0x3ULL;
  const TvFunc::WordType offset2 = 0x2ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask2;
  TvFunc::WordType tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm3_1(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask1   = 0x55ULL;
  const TvFunc::WordType offset1 = 0x55ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask1;
  TvFunc::WordType tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm3_2(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask2   = 0x33ULL;
  const TvFunc::WordType offset2 = 0x22ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask2;
  TvFunc::WordType tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm3_4(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask4   = 0x0fULL;
  const TvFunc::WordType offset4 = 0x04ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_3_4(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask4   = 0x0fULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm4_1(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask1   = 0x5555ULL;
  const TvFunc::WordType offset1 = 0x5555ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask1;
  TvFunc::WordType tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm4_2(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask2   = 0x3333ULL;
  const TvFunc::WordType offset2 = 0x2222ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask2;
  TvFunc::WordType tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm4_4(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask4   = 0x0f0fULL;
  const TvFunc::WordType offset4 = 0x0404ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_4_4(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask4   = 0x0f0fULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm4_8(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask8   = 0x00ffULL;
  const TvFunc::WordType offset8 = 0x0008ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask8;
  TvFunc::WordType tmp1 = (w >> 8) & mask8;
  m = tmp0 - tmp1 + offset8;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_4_8(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask8   = 0x00ffULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask8;
  TvFunc::WordType tmp1 = (w >> 8) & mask8;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm5_1(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask1   = 0x55555555ULL;
  const TvFunc::WordType offset1 = 0x55555555ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask1;
  TvFunc::WordType tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm5_2(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask2   = 0x33333333ULL;
  const TvFunc::WordType offset2 = 0x22222222ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask2;
  TvFunc::WordType tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm5_4(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask4   = 0x0f0f0f0fULL;
  const TvFunc::WordType offset4 = 0x04040404ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_5_4(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask4   = 0x0f0f0f0fULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm5_8(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask8   = 0x00ff00ffULL;
  const TvFunc::WordType offset8 = 0x00080008ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask8;
  TvFunc::WordType tmp1 = (w >> 8) & mask8;
  m = tmp0 - tmp1 + offset8;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_5_8(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask8   = 0x00ff00ffULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask8;
  TvFunc::WordType tmp1 = (w >> 8) & mask8;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm5_16(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask16   = 0x0000ffffULL;
  const TvFunc::WordType offset16 = 0x00000010ULL;
  TvFunc::WordType tmp0 = (w >>  0) & mask16;
  TvFunc::WordType tmp1 = (w >> 16) & mask16;
  m = tmp0 - tmp1 + offset16;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_5_16(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask16   = 0x0000ffffULL;
  TvFunc::WordType tmp0 = (w >>  0) & mask16;
  TvFunc::WordType tmp1 = (w >> 16) & mask16;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm6_1(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask1   = 0x5555555555555555ULL;
  const TvFunc::WordType offset1 = 0x5555555555555555ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask1;
  TvFunc::WordType tmp1 = (w >> 1) & mask1;
  m = tmp0 - tmp1 + offset1;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm6_2(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask2   = 0x3333333333333333ULL;
  const TvFunc::WordType offset2 = 0x2222222222222222ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask2;
  TvFunc::WordType tmp1 = (w >> 2) & mask2;
  m = tmp0 - tmp1 + offset2;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm6_4(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask4   = 0x0f0f0f0f0f0f0f0fULL;
  const TvFunc::WordType offset4 = 0x0404040404040404ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  m = tmp0 - tmp1 + offset4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_6_4(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask4   = 0x0f0f0f0f0f0f0f0fULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask4;
  TvFunc::WordType tmp1 = (w >> 4) & mask4;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm6_8(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask8   = 0x00ff00ff00ff00ffULL;
  const TvFunc::WordType offset8 = 0x0008000800080008ULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask8;
  TvFunc::WordType tmp1 = (w >> 8) & mask8;
  m = tmp0 - tmp1 + offset8;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_6_8(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask8   = 0x00ff00ff00ff00ffULL;
  TvFunc::WordType tmp0 = (w >> 0) & mask8;
  TvFunc::WordType tmp1 = (w >> 8) & mask8;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm6_16(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask16   = 0x0000ffff0000ffffULL;
  const TvFunc::WordType offset16 = 0x0000001000000010ULL;
  TvFunc::WordType tmp0 = (w >>  0) & mask16;
  TvFunc::WordType tmp1 = (w >> 16) & mask16;
  m = tmp0 - tmp1 + offset16;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_6_16(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask16   = 0x0000ffff0000ffffULL;
  TvFunc::WordType tmp0 = (w >>  0) & mask16;
  TvFunc::WordType tmp1 = (w >> 16) & mask16;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
pm6_32(
  TvFunc::WordType w,
  TvFunc::WordType& m
)
{
  const TvFunc::WordType mask32   = 0x00000000ffffffffULL;
  const TvFunc::WordType offset32 = 0x0000000000000020ULL;
  TvFunc::WordType tmp0 = (w >>  0) & mask32;
  TvFunc::WordType tmp1 = (w >> 32) & mask32;
  m = tmp0 - tmp1 + offset32;
  return tmp0 + tmp1;
}

inline
TvFunc::WordType
p_6_32(
  TvFunc::WordType w
)
{
  const TvFunc::WordType mask32   = 0x00000000ffffffffULL;
  TvFunc::WordType tmp0 = (w >>  0) & mask32;
  TvFunc::WordType tmp1 = (w >> 32) & mask32;
  return tmp0 + tmp1;
}

inline
SizeType
w2pos(
  SizeType ni,
  SizeType i,
  SizeType j
)
{
  return i * ni + j;
}

// 5入力の walsh_012 用サブルーティン
inline
int
walsh_012_5b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  const int n = 1 << 5;

  int ans;

  TvFunc::WordType tmp_1;
  TvFunc::WordType tmp = pm5_1(src_vec[0], tmp_1);
  // tmp  : 0
  // tmp_1: 1
  {
    TvFunc::WordType tmp_2;
    tmp = pm5_2(tmp, tmp_2);
    // tmp  : 00
    // tmp_2: 01
    {
      TvFunc::WordType tmp_3;
      tmp = pm5_4(tmp, tmp_3);
      // tmp  : 000
      // tmp_3: 001
      {
	TvFunc::WordType tmp_4;
	tmp = pm5_8(tmp, tmp_4);
	// tmp  : 0000
	// tmp_4: 0001
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm5_16(tmp, tmp_5);
	  // tmp  : 00000
	  // tmp_5: 00001
	  ans = n - tmp * 2;
	  vec1[4] = n - tmp_5 * 2;
	}
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm5_16(tmp_4, tmp_5);
	  // tmp  : 00010
	  // tmp_5: 00011
	  vec1[3] = n - tmp * 2;
	  vec2[w2pos(ni, 3, 4)] += n - tmp_5 * 2;
	}
      }
      {
	TvFunc::WordType tmp_4;
	tmp = pm5_8(tmp_3, tmp_4);
	// tmp  : 0010
	// tmp_4: 0011
	{
	  TvFunc::WordType tmp_5;
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
      TvFunc::WordType tmp_3;
      tmp = pm5_4(tmp_2, tmp_3);
      // tmp  : 010
      // tmp_3: 011
      {
	TvFunc::WordType tmp_4;
	tmp = pm5_8(tmp, tmp_4);
	// tmp  : 0100
	// tmp_4: 0101
	{
	  TvFunc::WordType tmp_5;
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
    TvFunc::WordType tmp_2;
    tmp = pm5_2(tmp_1, tmp_2);
    // tmp  : 10
    // tmp_2: 11
    {
      TvFunc::WordType tmp_3;
      tmp = pm5_4(tmp, tmp_3);
      // tmp  : 100
      // tmp_3: 101
      {
	TvFunc::WordType tmp_4;
	tmp = pm5_8(tmp, tmp_4);
	// tmp  : 1000
	// tmp_4: 1001
	{
	  TvFunc::WordType tmp_5;
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
int
walsh_012_6b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  const int n = 1 << 6;

  int ans;

  TvFunc::WordType tmp_1;
  TvFunc::WordType tmp = pm6_1(src_vec[0], tmp_1);
  // tmp  : 0
  // tmp_1: 1
  {
    TvFunc::WordType tmp_2;
    tmp = pm6_2(tmp, tmp_2);
    // tmp  : 00
    // tmp_2: 01
    {
      TvFunc::WordType tmp_3;
      tmp = pm6_4(tmp, tmp_3);
      // tmp  : 000
      // tmp_3: 001
      {
	TvFunc::WordType tmp_4;
	tmp = pm6_8(tmp, tmp_4);
	// tmp  : 0000
	// tmp_4: 0001
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 00000
	  // tmp_5: 00001
	  {
	    TvFunc::WordType tmp_6;
	    tmp = pm6_32(tmp, tmp_6);
	    // tmp  : 000000
	    // tmp_6: 000001
	    ans = n - tmp * 2;
	    vec1[5] = n - tmp_6 * 2;
	  }
	  {
	    TvFunc::WordType tmp_6;
	    tmp = pm6_32(tmp_5, tmp_6);
	    // tmp  : 000010
	    // tmp_6: 000011
	    vec1[4] = n - tmp * 2;
	    vec2[w2pos(ni, 4, 5)] += n - tmp_6 * 2;
	  }
	}
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm6_16(tmp_4, tmp_5);
	  // tmp  : 00010
	  // tmp_5: 00011
	  {
	    TvFunc::WordType tmp_6;
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
	TvFunc::WordType tmp_4;
	tmp = pm6_8(tmp_3, tmp_4);
	// tmp  : 0010
	// tmp_4: 0011
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 00100
	  // tmp_5: 00101
	  {
	    TvFunc::WordType tmp_6;
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
      TvFunc::WordType tmp_3;
      tmp = pm6_4(tmp_2, tmp_3);
      // tmp  : 010
      // tmp_3: 011
      {
	TvFunc::WordType tmp_4;
	tmp = pm6_8(tmp, tmp_4);
	// tmp  : 0100
	// tmp_4: 0101
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 01000
	  // tmp_5: 01001
	  {
	    TvFunc::WordType tmp_6;
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
    TvFunc::WordType tmp_2;
    tmp = pm6_2(tmp_1, tmp_2);
    // tmp  : 10
    // tmp_2: 11
    {
      TvFunc::WordType tmp_3;
      tmp = pm6_4(tmp, tmp_3);
      // tmp  : 100
      // tmp_3: 101
      {
	TvFunc::WordType tmp_4;
	tmp = pm6_8(tmp, tmp_4);
	// tmp  : 1000
	// tmp_4: 1001
	{
	  TvFunc::WordType tmp_5;
	  tmp = pm6_16(tmp, tmp_5);
	  // tmp  : 10000
	  // tmp_5: 10001
	  {
	    TvFunc::WordType tmp_6;
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
int
walsh_012_7b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_6b(src_vec,                     ni, vec1  , vec2);
  int ans1 = walsh_012_6b(src_vec + (1 << (6 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    vec2[w2pos(ni, i, 6)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[6] = ans0 - ans1;
  return ans0 + ans1;
}

// 8入力の walsh_012 用サブルーティン
inline
int
walsh_012_8b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_7b(src_vec,                     ni, vec1  , vec2);
  int ans1 = walsh_012_7b(src_vec + (1 << (7 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6} ) {
    vec2[w2pos(ni, i, 7)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[7] = ans0 - ans1;
  return ans0 + ans1;
}

// 9入力の walsh_012 用サブルーティン
inline
int
walsh_012_9b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_8b(src_vec,                     ni, vec1,   vec2);
  int ans1 = walsh_012_8b(src_vec + (1 << (8 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7} ) {
    vec2[w2pos(ni, i, 8)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[8] = ans0 - ans1;
  return ans0 + ans1;
}

// 10入力の walsh_012 用サブルーティン
inline
int
walsh_012_10b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_9b(src_vec,                     ni, vec1  , vec2);
  int ans1 = walsh_012_9b(src_vec + (1 << (9 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8} ) {
    vec2[w2pos(ni, i, 9)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[9] = ans0 - ans1;
  return ans0 + ans1;
}

// 11入力の walsh_012 用サブルーティン
inline
int
walsh_012_11b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_10b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_10b(src_vec + (1 << (10 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ) {
    vec2[w2pos(ni, i, 10)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[10] = ans0 - ans1;
  return ans0 + ans1;
}

// 12入力の walsh_012 用サブルーティン
inline
int
walsh_012_12b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_11b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_11b(src_vec + (1 << (11 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ) {
    vec2[w2pos(ni, i, 11)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[11] = ans0 - ans1;
  return ans0 + ans1;
}

// 13入力の walsh_012 用サブルーティン
int
walsh_012_13b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_12b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_12b(src_vec + (1 << (12 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} ) {
    vec2[w2pos(ni, i, 12)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[12] = ans0 - ans1;
  return ans0 + ans1;
}

// 14入力の walsh_012 用サブルーティン
int
walsh_012_14b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_13b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_13b(src_vec + (1 << (13 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} ) {
    vec2[w2pos(ni, i, 13)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[13] = ans0 - ans1;
  return ans0 + ans1;
}

// 15入力の walsh_012 用サブルーティン
int
walsh_012_15b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_14b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_14b(src_vec + (1 << (14 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} ) {
    vec2[w2pos(ni, i, 14)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[14] = ans0 - ans1;
  return ans0 + ans1;
}

// 16入力の walsh_012 用サブルーティン
int
walsh_012_16b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_15b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_15b(src_vec + (1 << (15 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14} ) {
    vec2[w2pos(ni, i, 15)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[15] = ans0 - ans1;
  return ans0 + ans1;
}

// 17入力の walsh_012 用サブルーティン
int
walsh_012_17b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_16b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_16b(src_vec + (1 << (16 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ) {
    vec2[w2pos(ni, i, 16)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[16] = ans0 - ans1;
  return ans0 + ans1;
}

// 18入力の walsh_012 用サブルーティン
int
walsh_012_18b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_17b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_17b(src_vec + (1 << (17 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} ) {
    vec2[w2pos(ni, i, 17)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[17] = ans0 - ans1;
  return ans0 + ans1;
}

// 19入力の walsh_012 用サブルーティン
int
walsh_012_19b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_18b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_18b(src_vec + (1 << (18 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17} ) {
    vec2[w2pos(ni, i, 18)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[18] = ans0 - ans1;
  return ans0 + ans1;
}

// 20入力の walsh_012 用サブルーティン
int
walsh_012_20b(
  TvFunc::WordType* src_vec,
  SizeType ni,
  int vec1[],
  int vec2[]
)
{
  int vec1_1[TvFunc::kMaxNi];
  int ans0 = walsh_012_19b(src_vec,                      ni, vec1  , vec2);
  int ans1 = walsh_012_19b(src_vec + (1 << (19 - NIPW)), ni, vec1_1, vec2);
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18} ) {
    vec2[w2pos(ni, i, 19)] += vec1[i] - vec1_1[i];
  }
  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18} ) {
    vec1[i] += vec1_1[i];
  }
  vec1[19] = ans0 - ans1;
  return ans0 + ans1;
}

// 0入力の walsh_012 本体
int
walsh_012_0(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  return walsh_01_0(src_vec, vec1);
}

// 1入力の walsh_012 本体
int
walsh_012_1(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  return walsh_01_1(src_vec, vec1);
}

// 2入力の walsh_012 本体
int
walsh_012_2(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int n = 1 << 2;

  int ans;

  TvFunc::WordType tmp_1;
  TvFunc::WordType tmp = pm2_1(src_vec[0], tmp_1);
  {
    TvFunc::WordType tmp_2;
    tmp = pm2_2(tmp, tmp_2);
    ans = n - tmp * 2;
    vec1[1] = n - tmp_2 * 2;
  }
  {
    TvFunc::WordType tmp_2;
    tmp = pm2_2(tmp_1, tmp_2);
    vec1[0] = n - tmp * 2;
    vec2[w2pos(2, 1, 0)] = vec2[w2pos(2, 0, 1)] = n - tmp_2 * 2;
  }

  return ans;
}

// 3入力の walsh_012 本体
int
walsh_012_3(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int n = 1 << 3;

  int ans;

  TvFunc::WordType tmp_1;
  TvFunc::WordType tmp = pm3_1(src_vec[0], tmp_1);
  {
    TvFunc::WordType tmp_2;
    tmp = pm3_2(tmp, tmp_2);
    {
      TvFunc::WordType tmp_3;
      tmp = pm3_4(tmp, tmp_3);
      ans = n - tmp * 2;
      vec1[2] = n - tmp_3 * 2;
    }
    {
      TvFunc::WordType tmp_3;
      tmp = pm3_4(tmp_2, tmp_3);
      vec1[1] = n - tmp * 2;
      vec2[w2pos(3, 2, 1)] = vec2[w2pos(3, 1, 2)] = n - tmp_3 * 2;
    }
  }
  {
    TvFunc::WordType tmp_2;
    tmp = pm3_2(tmp_1, tmp_2);
    {
      TvFunc::WordType tmp_3;
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
int
walsh_012_4(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int n = 1 << 4;

  int ans;

  TvFunc::WordType tmp_1;
  TvFunc::WordType tmp = pm4_1(src_vec[0], tmp_1);
  {
    TvFunc::WordType tmp_2;
    tmp = pm4_2(tmp, tmp_2);
    {
      TvFunc::WordType tmp_3;
      tmp = pm4_4(tmp, tmp_3);
      {
	TvFunc::WordType tmp_4;
	tmp = pm4_8(tmp, tmp_4);
	ans = n - tmp * 2;
	vec1[3] = n - tmp_4 * 2;
      }
      {
	TvFunc::WordType tmp_4;
	tmp = pm4_8(tmp_3, tmp_4);
	vec1[2] = n - tmp * 2;
	vec2[w2pos(4, 3, 2)] = vec2[w2pos(4, 2, 3)] = n - tmp_4 * 2;
      }
    }
    {
      TvFunc::WordType tmp_3;
      tmp = pm4_4(tmp_2, tmp_3);
      {
	TvFunc::WordType tmp_4;
	tmp = pm4_8(tmp, tmp_4);
	vec1[1] = n - tmp * 2;
	vec2[w2pos(4, 3, 1)] = vec2[w2pos(4, 1, 3)] = n - tmp_4 * 2;
      }
      tmp = p_4_8(tmp_3);
      vec2[w2pos(4, 2, 1)] = vec2[w2pos(4, 1, 2)] = n - tmp * 2;
    }
  }
  {
    TvFunc::WordType tmp_2;
    tmp = pm4_2(tmp_1, tmp_2);
    {
      TvFunc::WordType tmp_3;
      tmp = pm4_4(tmp, tmp_3);
      {
	TvFunc::WordType tmp_4;
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
int
walsh_012_5(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 5 * 5;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_5b(src_vec, 5, vec1, vec2);

  for ( int i: {0, 1, 2, 3} ) {
    for ( int j: Range(i + 1, 5) ) {
      vec2[w2pos(5, j, i)] = vec2[w2pos(5, i, j)];
    }
  }

  return ans;
}

// 6入力の walsh_012 本体
int
walsh_012_6(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 6 * 6;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_6b(src_vec, 6, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4} ) {
    for ( int j: Range(i + 1, 6) ) {
      vec2[w2pos(6, j, i)] = vec2[w2pos(6, i, j)];
    }
  }

  return ans;
}

// 7入力の walsh_012 本体
int
walsh_012_7(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 7 * 7;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_7b(src_vec, 7, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5} ) {
    for ( int j: Range(i + 1, 7) ) {
      vec2[w2pos(7, j, i)] = vec2[w2pos(7, i, j)];
    }
  }

  return ans;
}

// 8入力の walsh_012 本体
int
walsh_012_8(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 8 * 8;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_8b(src_vec, 8, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6} ) {
    for ( int j: Range(i + 1, 8) ) {
      vec2[w2pos(8, j, i)] = vec2[w2pos(8, i, j)];
    }
  }

  return ans;
}

// 9入力の walsh_012 本体
int
walsh_012_9(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 9 * 9;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_9b(src_vec, 9, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7} ) {
    for ( int j: Range(i + 1, 9) ) {
      vec2[w2pos(9, j, i)] = vec2[w2pos(9, i, j)];
    }
  }

  return ans;
}

// 10入力の walsh_012 本体
int
walsh_012_10(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 10 * 10;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_10b(src_vec, 10, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 5, 6, 7, 8} ) {
    for ( int j: Range(i + 1, 10) ) {
      vec2[w2pos(10, j, i)] = vec2[w2pos(10, i, j)];
    }
  }

  return ans;
}

// 11入力の walsh_012 本体
int
walsh_012_11(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 11 * 11;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_11b(src_vec, 11, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ) {
    for ( int j: Range(i + 1, 11) ) {
      vec2[w2pos(11, j, i)] = vec2[w2pos(11, i, j)];
    }
  }

  return ans;
}

// 12入力の walsh_012 本体
int
walsh_012_12(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 12 * 12;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_12b(src_vec, 12, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ) {
    for ( int j: Range(i + 1, 12) ) {
      vec2[w2pos(12, j, i)] = vec2[w2pos(12, i, j)];
    }
  }

  return ans;
}

// 13入力の walsh_012 本体
int
walsh_012_13(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 13 * 13;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_13b(src_vec, 13, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} ) {
    for ( int j: Range(i + 1, 13) ) {
      vec2[w2pos(13, j, i)] = vec2[w2pos(13, i, j)];
    }
  }

  return ans;
}

// 14入力の walsh_012 本体
int
walsh_012_14(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 14 * 14;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_14b(src_vec, 14, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12} ) {
    for ( int j: Range(i + 1, 14) ) {
      vec2[w2pos(14, j, i)] = vec2[w2pos(14, i, j)];
    }
  }

  return ans;
}

// 15入力の walsh_012 本体
int
walsh_012_15(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 15 * 15;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_15b(src_vec, 15, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} ) {
    for ( int j: Range(i + 1, 15) ) {
      vec2[w2pos(15, j, i)] = vec2[w2pos(15, i, j)];
    }
  }

  return ans;
}

// 16入力の walsh_012 本体
int
walsh_012_16(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 16 * 16;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_16b(src_vec, 16, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14} ) {
    for ( int j: Range(i + 1, 16) ) {
      vec2[w2pos(16, j, i)] = vec2[w2pos(16, i, j)];
    }
  }

  return ans;
}

// 17入力の walsh_012 本体
int
walsh_012_17(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 17 * 17;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_17b(src_vec, 17, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ) {
    for ( int j: Range(i + 1, 17) ) {
      vec2[w2pos(17, j, i)] = vec2[w2pos(17, i, j)];
    }
  }

  return ans;
}

// 18入力の walsh_012 本体
int
walsh_012_18(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 18 * 18;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_18b(src_vec, 18, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} ) {
    for ( int j: Range(i + 1, 18) ) {
      vec2[w2pos(18, j, i)] = vec2[w2pos(18, i, j)];
    }
  }

  return ans;
}

// 19入力の walsh_012 本体
int
walsh_012_19(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 19 * 19;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_19b(src_vec, 19, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17} ) {
    for ( int j: Range(i + 1, 19) ) {
      vec2[w2pos(19, j, i)] = vec2[w2pos(19, i, j)];
    }
  }

  return ans;
}

// 20入力の walsh_01 本体
int
walsh_012_20(
  TvFunc::WordType* src_vec,
  int vec1[],
  int vec2[]
)
{
  const int nn = 20 * 20;
  for ( int i: Range(nn) ) {
    vec2[i] = 0;
  }

  int ans = walsh_012_20b(src_vec, 20, vec1, vec2);

  for ( int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18} ) {
    for ( int j: Range(i + 1, 20) ) {
      vec2[w2pos(20, j, i)] = vec2[w2pos(20, i, j)];
    }
  }

  return ans;
}

END_NONAMESPACE

// 0次と 1次と 2次の Walsh 係数を求める．
int
TvFunc::walsh_012(
  int vec1[],
  int vec2[]
) const
{
  if ( is_invalid() ) {
    return 0;
  }

  switch ( input_num() ) {
  case  0: return walsh_012_0(mVector, vec1, vec2);
  case  1: return walsh_012_1(mVector, vec1, vec2);
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
walsh_w0_0(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  ASSERT_COND( w == 0 );

  int nall = 1;
  int c = (bitvec >> (0 ^ ibits)) & 1;
  return nall - c * 2;
}

inline
int
walsh_w0_1(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  int nall = 0;
  int c = 0;
  switch ( w ) {
  case 0:
    nall = 1;
    c  = (bitvec >> ( 0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 1;
    c  = (bitvec >> ( 1 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  return nall - c * 2;
}

inline
int
walsh_w0_2(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  int nall = 0;
  int c = 0;
  switch ( w ) {
  case 0:
    nall = 1;
    c  = (bitvec >> ( 0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 2;
    c  = (bitvec >> ( 1 ^ ibits)) & 1;
    c += (bitvec >> ( 2 ^ ibits)) & 1;
    break;
  case 2:
    nall = 1;
    c  = (bitvec >> ( 3 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  return nall - c * 2;
}

inline
int
walsh_w0_3(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  int nall = 0;
  int c = 0;
  switch ( w ) {
  case 0:
    nall = 1;
    c  = (bitvec >> ( 0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 3;
    c  = (bitvec >> ( 1 ^ ibits)) & 1;
    c += (bitvec >> ( 2 ^ ibits)) & 1;
    c += (bitvec >> ( 4 ^ ibits)) & 1;
    break;
  case 2:
    nall = 3;
    c  = (bitvec >> ( 3 ^ ibits)) & 1;
    c += (bitvec >> ( 5 ^ ibits)) & 1;
    c += (bitvec >> ( 6 ^ ibits)) & 1;
    break;
  case 3:
    nall = 1;
    c  = (bitvec >> ( 7 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  return nall - c * 2;
}

inline
int
walsh_w0_4(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  int nall = 0;
  int c = 0;
  switch ( w ) {
  case 0:
    nall = 1;
    c  = (bitvec >> ( 0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 4;
    c  = (bitvec >> ( 1 ^ ibits)) & 1;
    c += (bitvec >> ( 2 ^ ibits)) & 1;
    c += (bitvec >> ( 4 ^ ibits)) & 1;
    c += (bitvec >> ( 8 ^ ibits)) & 1;
    break;
  case 2:
    nall = 6;
    c  = (bitvec >> ( 3 ^ ibits)) & 1;
    c += (bitvec >> ( 5 ^ ibits)) & 1;
    c += (bitvec >> ( 6 ^ ibits)) & 1;
    c += (bitvec >> ( 9 ^ ibits)) & 1;
    c += (bitvec >> (10 ^ ibits)) & 1;
    c += (bitvec >> (12 ^ ibits)) & 1;
    break;
  case 3:
    nall = 4;
    c  = (bitvec >> ( 7 ^ ibits)) & 1;
    c += (bitvec >> (11 ^ ibits)) & 1;
    c += (bitvec >> (13 ^ ibits)) & 1;
    c += (bitvec >> (14 ^ ibits)) & 1;
    break;
  case 4:
    nall = 1;
    c  = (bitvec >> (15 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  return nall - c * 2;
}

inline
int
walsh_w0_5(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  int nall = 0;
  int c = 0;
  switch ( w ) {
  case 0:
    nall = 1;
    c  = (bitvec >> ( 0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 5;
    c  = (bitvec >> ( 1 ^ ibits)) & 1;
    c += (bitvec >> ( 2 ^ ibits)) & 1;
    c += (bitvec >> ( 4 ^ ibits)) & 1;
    c += (bitvec >> ( 8 ^ ibits)) & 1;
    c += (bitvec >> (16 ^ ibits)) & 1;
    break;
  case 2:
    nall = 10;
    c  = (bitvec >> ( 3 ^ ibits)) & 1;
    c += (bitvec >> ( 5 ^ ibits)) & 1;
    c += (bitvec >> ( 6 ^ ibits)) & 1;
    c += (bitvec >> ( 9 ^ ibits)) & 1;
    c += (bitvec >> (10 ^ ibits)) & 1;
    c += (bitvec >> (12 ^ ibits)) & 1;
    c += (bitvec >> (17 ^ ibits)) & 1;
    c += (bitvec >> (18 ^ ibits)) & 1;
    c += (bitvec >> (20 ^ ibits)) & 1;
    c += (bitvec >> (24 ^ ibits)) & 1;
    break;
  case 3:
    nall = 10;
    c  = (bitvec >> ( 7 ^ ibits)) & 1;
    c += (bitvec >> (11 ^ ibits)) & 1;
    c += (bitvec >> (13 ^ ibits)) & 1;
    c += (bitvec >> (14 ^ ibits)) & 1;
    c += (bitvec >> (19 ^ ibits)) & 1;
    c += (bitvec >> (21 ^ ibits)) & 1;
    c += (bitvec >> (22 ^ ibits)) & 1;
    c += (bitvec >> (25 ^ ibits)) & 1;
    c += (bitvec >> (26 ^ ibits)) & 1;
    c += (bitvec >> (28 ^ ibits)) & 1;
    break;
  case 4:
    nall = 5;
    c  = (bitvec >> (15 ^ ibits)) & 1;
    c += (bitvec >> (23 ^ ibits)) & 1;
    c += (bitvec >> (27 ^ ibits)) & 1;
    c += (bitvec >> (29 ^ ibits)) & 1;
    c += (bitvec >> (30 ^ ibits)) & 1;
    break;
  case 5:
    nall = 1;
    c  = (bitvec >> (31 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  return nall - c * 2;
}

inline
int
walsh_w0_6(
  TvFunc::WordType bitvec,
  int ibits,
  int w
)
{
  int nall = 0;
  int c = 0;
  switch ( w ) {
  case 0:
    nall = 1;
    c  = (bitvec >> ( 0 ^ ibits)) & 1;
    break;
  case 1:
    nall = 6;
    c  = (bitvec >> ( 1 ^ ibits)) & 1;
    c += (bitvec >> ( 2 ^ ibits)) & 1;
    c += (bitvec >> ( 4 ^ ibits)) & 1;
    c += (bitvec >> ( 8 ^ ibits)) & 1;
    c += (bitvec >> (16 ^ ibits)) & 1;
    c += (bitvec >> (32 ^ ibits)) & 1;
    break;
  case 2:
    nall = 15;
    c  = (bitvec >> ( 3 ^ ibits)) & 1;
    c += (bitvec >> ( 5 ^ ibits)) & 1;
    c += (bitvec >> ( 6 ^ ibits)) & 1;
    c += (bitvec >> ( 9 ^ ibits)) & 1;
    c += (bitvec >> (10 ^ ibits)) & 1;
    c += (bitvec >> (12 ^ ibits)) & 1;
    c += (bitvec >> (17 ^ ibits)) & 1;
    c += (bitvec >> (18 ^ ibits)) & 1;
    c += (bitvec >> (20 ^ ibits)) & 1;
    c += (bitvec >> (24 ^ ibits)) & 1;
    c += (bitvec >> (33 ^ ibits)) & 1;
    c += (bitvec >> (34 ^ ibits)) & 1;
    c += (bitvec >> (36 ^ ibits)) & 1;
    c += (bitvec >> (40 ^ ibits)) & 1;
    c += (bitvec >> (48 ^ ibits)) & 1;
    break;
  case 3:
    nall = 20;
    c  = (bitvec >> ( 7 ^ ibits)) & 1;
    c += (bitvec >> (11 ^ ibits)) & 1;
    c += (bitvec >> (13 ^ ibits)) & 1;
    c += (bitvec >> (14 ^ ibits)) & 1;
    c += (bitvec >> (19 ^ ibits)) & 1;
    c += (bitvec >> (21 ^ ibits)) & 1;
    c += (bitvec >> (22 ^ ibits)) & 1;
    c += (bitvec >> (25 ^ ibits)) & 1;
    c += (bitvec >> (26 ^ ibits)) & 1;
    c += (bitvec >> (28 ^ ibits)) & 1;
    c += (bitvec >> (35 ^ ibits)) & 1;
    c += (bitvec >> (37 ^ ibits)) & 1;
    c += (bitvec >> (38 ^ ibits)) & 1;
    c += (bitvec >> (41 ^ ibits)) & 1;
    c += (bitvec >> (42 ^ ibits)) & 1;
    c += (bitvec >> (44 ^ ibits)) & 1;
    c += (bitvec >> (49 ^ ibits)) & 1;
    c += (bitvec >> (50 ^ ibits)) & 1;
    c += (bitvec >> (52 ^ ibits)) & 1;
    c += (bitvec >> (56 ^ ibits)) & 1;
    break;
  case 4:
    nall = 15;
    c  = (bitvec >> (15 ^ ibits)) & 1;
    c += (bitvec >> (23 ^ ibits)) & 1;
    c += (bitvec >> (27 ^ ibits)) & 1;
    c += (bitvec >> (29 ^ ibits)) & 1;
    c += (bitvec >> (30 ^ ibits)) & 1;
    c += (bitvec >> (39 ^ ibits)) & 1;
    c += (bitvec >> (43 ^ ibits)) & 1;
    c += (bitvec >> (45 ^ ibits)) & 1;
    c += (bitvec >> (46 ^ ibits)) & 1;
    c += (bitvec >> (51 ^ ibits)) & 1;
    c += (bitvec >> (53 ^ ibits)) & 1;
    c += (bitvec >> (54 ^ ibits)) & 1;
    c += (bitvec >> (57 ^ ibits)) & 1;
    c += (bitvec >> (58 ^ ibits)) & 1;
    c += (bitvec >> (60 ^ ibits)) & 1;
    break;
  case 5:
    nall = 6;
    c  = (bitvec >> (31 ^ ibits)) & 1;
    c += (bitvec >> (47 ^ ibits)) & 1;
    c += (bitvec >> (55 ^ ibits)) & 1;
    c += (bitvec >> (59 ^ ibits)) & 1;
    c += (bitvec >> (61 ^ ibits)) & 1;
    c += (bitvec >> (62 ^ ibits)) & 1;
    break;
  case 6:
    nall = 1;
    c  = (bitvec >> (63 ^ ibits)) & 1;
    break;
  default:
    ASSERT_NOT_REACHED;
  }
  return nall - c * 2;
}

END_NONAMESPACE

// 重み別の 0 次の Walsh 係数を求める．
int
TvFunc::walsh_w0(
  int w,
  bool oinv,
  int ibits
) const
{
  if ( is_invalid() ) {
    return 0;
  }

  int ans;
  switch ( input_num() ) {
  case  0: ans = walsh_w0_0(mVector[0], ibits, w); break;
  case  1: ans = walsh_w0_1(mVector[0], ibits, w); break;
  case  2: ans = walsh_w0_2(mVector[0], ibits, w); break;
  case  3: ans = walsh_w0_3(mVector[0], ibits, w); break;
  case  4: ans = walsh_w0_4(mVector[0], ibits, w); break;
  case  5: ans = walsh_w0_5(mVector[0], ibits, w); break;
  case  6: ans = walsh_w0_6(mVector[0], ibits, w); break;
  default: // input_num() > 6
    { // 汎用のコード
      //
      // 基本的なアルゴリズムは全てのワードをスキャンしてその中から
      // 位置の重みが w であるビット位置の数 nall と1であるビット位置の
      // 数 c を求めて nall - c * 2 を返す．
      //
      // ただし複数のワードにまたがっているので対象の重みのビットマスクは
      // ワードごとに異なる．
      // s 番目のブロックの t ビット目は s * 64 + t 番目の位置に対応している．
      // なのでその位置の重みは s のビット重みと t のビット重みを足したものとなる．

      // そこで u + v = w であるような2つの整数 u, v を用意して
      // ブロック番号中の1の重み
      int nall = 0;
      int c = 0;
      // ブロック番号に対する入力反転ビットマスク
      int ibits1 = ibits >> NIPW;
      // ブロック内の入力反転ビットマスク
      int ibits2 = ibits & ((1UL << NIPW) - 1UL);
      for ( int b0: Range(mBlockNum) ) {
	// ブロック番号中の1の重み
	int u = count_onebits(b0);
	if ( u > w ) {
	  continue;
	}
	// ブロックの中の1の重み
	int v = w - u;
	if ( v > NIPW ) {
	  continue;
	}
	int start = s_pidx[v];
	int end = s_pidx[v + 1];
	int* endp = &s_plist[end];
	int b1 = b0 ^ ibits1;
	TvFunc::WordType bitvec = mVector[b1];
	for ( int* p = &s_plist[start]; p != endp; ++ p ) {
	  int bitpos = *p ^ ibits2;
	  c += (bitvec >> bitpos) & 1;
	}
	nall += (end - start);
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
TvFunc::walsh_w1(
  SizeType var,
  int w,
  bool oinv,
  int ibits
) const
{
  if ( is_invalid() ) {
    return 0;
  }

  int ans;
  if ( input_num() <= 6 ) {
    TvFunc::WordType bitvec = mVector[0];
    if ( ibits & (1U << var) ) {
      bitvec ^= ~c_mask(var);
    }
    else {
      bitvec ^= c_mask(var);
    }
    switch ( input_num() ) {
    case 0: ans = walsh_w0_0(bitvec, ibits, w); break;
    case 1: ans = walsh_w0_1(bitvec, ibits, w); break;
    case 2: ans = walsh_w0_2(bitvec, ibits, w); break;
    case 3: ans = walsh_w0_3(bitvec, ibits, w); break;
    case 4: ans = walsh_w0_4(bitvec, ibits, w); break;
    case 5: ans = walsh_w0_5(bitvec, ibits, w); break;
    case 6: ans = walsh_w0_6(bitvec, ibits, w); break;
    }
  }
  else {
    // 汎用のコード
    int nall = 0;
    int c = 0;
    // ブロック番号に対する入力反転ビットマスク
    int ibits1 = ibits >> NIPW;
    // ブロック内の入力反転ビットマスク
    int ibits2 = ibits & ((1UL << NIPW) - 1UL);

    for ( int b0: Range(mBlockNum) ) {
      // ブロック番号中の1の重み
      int u = count_onebits(b0);
      if ( u > w ) {
	continue;
      }
      // ブロックの中の1の重み
      int v = w - u;
      if ( v > NIPW ) {
	continue;
      }
      int start = s_pidx[v];
      int end = s_pidx[v + 1];
      int* endp = &s_plist[end];
      int b1 = b0 ^ ibits1;
      TvFunc::WordType mask;
      if ( var < NIPW ) {
	if ( ibits2 & (1 << var) ) {
	  mask = ~c_mask(var);
	}
	else {
	  mask = c_mask(var);
	}
      }
      else {
	int var5 = var - NIPW;
	// 2行下の式は1行下の式と同じ意味
	// mask = (b0 & (1 << var5)) ? 0xFFFFFFFF : 0x00000000;
	mask = 0UL - ((b0 >> var5) & 1UL);
      }
      TvFunc::WordType bitvec = mVector[b1] ^ mask;
      for ( int* p = &s_plist[start]; p != endp; ++ p ) {
	int bitpos = *p ^ ibits2;
	c += (bitvec >> bitpos) & 1;
      }
      nall += (end - start);
    }
    ans = nall - c * 2;
  }

  if ( oinv ) {
    ans = -ans;
  }
  return ans;
}

END_NAMESPACE_YM
