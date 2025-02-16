
/// @file TvFunc.cc
/// @brief TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/Range.h"


// 1 ワード当たりの入力数
#define NIPW 6

//#define DEBUG

BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// コファクターマスク
// 0〜5番目の変数に対するリテラル関数
// のビットパタン
// これで AND を取れば正のコファクター
// 否定の AND を取れば負のコファクター
// が得られる．
TvFunc::WordType c_masks[] = {
  0xAAAAAAAAAAAAAAAAULL,
  0xCCCCCCCCCCCCCCCCULL,
  0xF0F0F0F0F0F0F0F0ULL,
  0xFF00FF00FF00FF00ULL,
  0xFFFF0000FFFF0000ULL,
  0xFFFFFFFF00000000ULL
};

// 対称性を調べるためのテーブルその1
// 同位相の時に用いる．
TvFunc::WordType sym_masks2[] = {
  0x2222222222222222ULL, // (1, 0)
  0x0A0A0A0A0A0A0A0AULL, // (2, 0)
  0x0C0C0C0C0C0C0C0CULL, // (2, 1)
  0x00AA00AA00AA00AAULL, // (3, 0)
  0x00CC00CC00CC00CCULL, // (3, 1)
  0x00F000F000F000F0ULL, // (3, 2)
  0x0000AAAA0000AAAAULL, // (4, 0)
  0x0000CCCC0000CCCCULL, // (4, 1)
  0x0000F0F00000F0F0ULL, // (4, 2)
  0x0000FF000000FF00ULL, // (4, 3)
  0x00000000AAAAAAAAULL, // (5, 0)
  0x00000000CCCCCCCCULL, // (5, 1)
  0x00000000F0F0F0F0ULL, // (5, 2)
  0x00000000FF00FF00ULL, // (5, 3)
  0x00000000FFFF0000ULL  // (5, 4)
};

// 対称性を調べるためのテーブルその2
// 逆位相の時に用いる．
TvFunc::WordType sym_masks3[] = {
  0x1111111111111111ULL, // (1, 0)
  0x0505050505050505ULL, // (2, 0)
  0x0303030303030303ULL, // (2, 1)
  0x0055005500550055ULL, // (3, 0)
  0x0033003300330033ULL, // (3, 1)
  0x000F000F000F000FULL, // (3, 2)
  0x0000555500005555ULL, // (4, 0)
  0x0000333300003333ULL, // (4, 1)
  0x00000F0F00000F0FULL, // (4, 2)
  0x000000FF000000FFULL, // (4, 3)
  0x0000000055555555ULL, // (5, 0)
  0x0000000033333333ULL, // (5, 1)
  0x000000000F0F0F0FULL, // (5, 2)
  0x0000000000FF00FFULL, // (5, 3)
  0x000000000000FFFFULL  // (5, 4)
};

// @brief 入力数が NIPW 以下の時のマスクを返す関数
// @param[in] ni 入力数
inline
TvFunc::WordType
vec_mask(
  SizeType ni
)
{
  TvFunc::WordType mask = ~0ULL;
  if ( ni < NIPW ) {
    SizeType ni_exp = 1 << ni;
    mask >>= (64 - ni_exp);
  }
  return mask;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コファクターを計算し自分に代入する．
TvFunc&
TvFunc::cofactor_int(
  SizeType varid,
  bool inv
)
{
  if ( is_valid() ) {
    _check_varid(varid);

    SizeType pos = varid;
    if ( pos < NIPW ) {
      TvFunc::WordType mask = c_masks[pos];
      if ( inv ) {
	mask = ~mask;
      }
      SizeType shift = 1 << pos;
      for ( SizeType b: Range(mBlockNum) ) {
	TvFunc::WordType pat = mVector[b] & mask;
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
      SizeType bit = 1U << pos;
      for ( SizeType b: Range(mBlockNum) ) {
	if ( inv ) {
	  if ( (b & bit) == bit ) {
	    mVector[b] = mVector[b ^ bit];
	  }
	}
	else {
	  if ( (b & bit) == 0U ) {
	    mVector[b] = mVector[b ^ bit];
	  }
	}
      }
    }
  }
  return *this;
}

// @brief 外側の変数で分割する．
void
TvFunc::decompose(
  TvFunc& func0,
  TvFunc& func1
) const
{
  if ( is_invalid() ) {
    throw std::invalid_argument{"invalid function"};
  }

  auto ni = input_num();
  auto ni1 = ni - 1;
  func0 = TvFunc(ni1);
  func1 = TvFunc(ni1);
  if ( ni1 < NIPW ) {
    auto mask1 = c_masks[ni1];
    auto mask0 = ~mask1;
    SizeType shift = (1 << ni1);
    func0.mVector[0] = mVector[0] & mask0;
    func1.mVector[0] = (mVector[0] & mask1) >> shift;
  }
  else {
    SizeType block_num = (1 << (ni1 - NIPW));
    for ( SizeType b: Range(block_num) ) {
      func0.mVector[b] = mVector[b];
      func1.mVector[b] = mVector[b + block_num];
    }
  }
}

// 変数がサポートの時 true を返す．
bool
TvFunc::check_sup(
  SizeType var
) const
{
  if ( is_invalid() ) {
    // 不正な関数ならサポートではない．
    return false;
  }

  _check_varid(var);

  SizeType i = var;
  if ( i < NIPW ) {
    // ブロックごとにチェック
    SizeType dist = 1 << i;
    TvFunc::WordType mask = c_masks[i];
    for ( SizeType b: Range(mBlockNum) ) {
      TvFunc::WordType word = mVector[b];
      if ( (word ^ (word << dist)) & mask ) {
	return true;
      }
    }
  }
  else {
    // ブロック単位でチェック
    SizeType i5 = i - NIPW;
    SizeType check = 1 << i5;
    for ( SizeType b: Range(mBlockNum) ) {
      if ( (b & check) && (mVector[b] != mVector[b ^ check]) ) {
	return true;
      }
    }
  }
  return false;
}

// @brief unateness を調べる．
int
TvFunc::check_unate(
  SizeType varid
) const
{
  if ( is_invalid() ) {
    // 不正な関数なら true ?
    return 3;
  }

  _check_varid(varid);

  SizeType i = varid;
  bool positive_unate = true;
  bool negative_unate = true;
  if ( i < NIPW ) {
    // ブロックごとにチェック
    SizeType dist = 1 << i;
    TvFunc::WordType mask = c_masks[i];
    for ( SizeType b: Range(mBlockNum) ) {
      TvFunc::WordType word = mVector[b];
      auto pat0 = word & ~mask;
      auto pat1 = word &  mask;
      pat0 |= pat0 << dist;
      pat1 |= pat1 >> dist;
      if ( pat0 == pat1 ) {
	continue;
      }
      auto patc = pat0 & pat1;
      if ( patc == pat0 ) {
	negative_unate = false;
      }
      else if ( patc == pat1 ) {
	positive_unate = false;
      }
      else {
	return 0;
      }
    }
  }
  else {
    // ブロック単位でチェック
    SizeType i5 = i - NIPW;
    SizeType check = 1 << i5;
    for ( SizeType b: Range(mBlockNum) ) {
      if ( b & check ) {
	auto f0 = mVector[b ^ check];
	auto f1 = mVector[b];
	if ( f0 == f1 ) {
	  continue;
	}
	auto fc = f0 & f1;
	if ( fc == f0 ) {
	  negative_unate = false;
	}
	else if ( fc == f1 ) {
	  positive_unate = false;
	}
	else {
	  return 0;
	}
      }
    }
  }
  int ans = 0;
  if ( positive_unate ) {
    ans |= 1;
  }
  if ( negative_unate ) {
    ans |= 2;
  }
  return ans;
}

// i 番目と j 番目の変数が対称のとき true を返す．
bool
TvFunc::check_sym(
  SizeType var1,
  SizeType var2,
  bool inv
) const
{
  if ( is_invalid() ) {
    // 不正な関数なら区別できない．
    return true;
  }

  _check_varid(var1, "var1");
  _check_varid(var2, "var2");

  SizeType i = var1;
  SizeType j = var2;

  // i と j を正規化する．
  if ( i < j ) {
    std::swap(i, j);
  }
  // ここ以降では必ず i > j が成り立つ．

  bool ans = true;
  if ( j >= NIPW ) {
    // i >= NIPW (実際には i > NIPW)
    // j >= NIPW
    // ブロック単位で比較する．
    SizeType mask_i = (1 << (i - NIPW));
    SizeType mask_j = (1 << (j - NIPW));
    SizeType mask_all = mask_i | mask_j;
    SizeType cond;
    if ( inv ) {
      cond = 0UL;
    }
    else {
      cond = mask_j;
    }
    for ( SizeType b: Range(mBlockNum) ) {
      if ( (b & mask_all) == cond &&
	   mVector[b] != mVector[b ^ mask_all] ) {
	ans = false;
	break;
      }
    }
  }
  else if ( i >= NIPW ) {
    // i >= NIPW
    // j < NIPW
    SizeType mask_i = (1 << (i - NIPW));
    SizeType cond;
    if ( inv ) {
      cond = 0UL;
    }
    else {
      cond = mask_i;
    }
    TvFunc::WordType mask2 = ~c_masks[j];
    SizeType s = 1 << j;
    for ( SizeType b: Range(mBlockNum) ) {
      if ( (b & mask_i) == cond &&
	   (mVector[b] ^ (mVector[b ^ mask_i] >> s)) & mask2 ) {
	ans = false;
	break;
      }
    }
  }
  else {
    // i < NIPW
    // j < NIPW
    if ( inv ) {
      TvFunc::WordType mask = sym_masks3[(i * (i - 1)) / 2 + j];
      SizeType s = (1 << i) + (1 << j);
      for ( SizeType b: Range(mBlockNum) ) {
	TvFunc::WordType word = mVector[b];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
    else {
      TvFunc::WordType mask = sym_masks2[(i * (i - 1)) / 2 + j];
      SizeType s = (1 << i) - (1 << j);
      for ( SizeType b: Range(mBlockNum) ) {
	TvFunc::WordType word = mVector[b];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  return ans;
}

// @brief コファクターマスクを得る．
TvFunc::WordType
TvFunc::c_mask(
  SizeType pos
)
{
  return c_masks[pos];
}

// @brief リテラル関数のビットパタンを得る
TvFunc::WordType
TvFunc::lit_pat(
  SizeType var_id,
  SizeType block_id
)
{
  if ( var_id < NIPW ) {
    // block_id に無関係
    return c_masks[var_id];
  }
  else {
    // ちょっとトリッキーなコード
    // block_id の var_id1 ビットめが1の時 0 - 1 = ~0が
    // 0 の 0 が返される．
    SizeType var_id1 = var_id - NIPW;
    return 0ULL - ((block_id >> var_id1) & 1ULL);
  }
}

END_NAMESPACE_YM
