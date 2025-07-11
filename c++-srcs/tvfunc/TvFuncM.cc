
/// @file TvFuncM.cc
/// @brief TvFuncM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFuncM.h"
#include "ym/NpnMapM.h"
#include "ym/Range.h"


// 1 ワード当たりの入力数
#define NIPW 6

//#define DEBUG

BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// コファクターマスク
TvFuncM::WordType c_masks[] = {
  0xAAAAAAAAAAAAAAAAULL,
  0xCCCCCCCCCCCCCCCCULL,
  0xF0F0F0F0F0F0F0F0ULL,
  0xFF00FF00FF00FF00ULL,
  0xFFFF0000FFFF0000ULL,
  0xFFFFFFFF00000000ULL
};

// 対称性を調べるためのテーブルその1
// 同位相の時に用いる．
TvFuncM::WordType sym_masks2[] = {
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
TvFuncM::WordType sym_masks3[] = {
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

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

// 入力数と出力数のみ指定したコンストラクタ
TvFuncM::TvFuncM(
  SizeType ni,
  SizeType no
) : mInputNum{ni},
    mOutputNum{no},
    mBlockNum1{nblock(ni)},
    mBlockNum{mBlockNum1 * no},
    mVector(mBlockNum, 0ULL)
{
}

// @brief TvFunc を用いたコンストラクタ
TvFuncM::TvFuncM(
  const std::vector<TvFunc>& src_list
)
{
  ASSERT_COND( !src_list.empty() );
  const TvFunc& first = src_list.front();
  auto ni = first.input_num();
  auto no = src_list.size();
  // 全ての関数の入力数が等しいことを確認しておく．
  for ( auto f: src_list ) {
    ASSERT_COND( f.input_num() == ni );
  }

  mInputNum = ni;
  mOutputNum = no;
  mBlockNum1 = nblock(ni);
  mBlockNum = mBlockNum1 * no;
  mVector = std::vector<WordType>(mBlockNum);
  for ( SizeType i: Range(no) ) {
    SizeType offset = i * mBlockNum1;
    auto& src1 = src_list[i];
    for ( SizeType b: Range(mBlockNum1) ) {
      mVector[offset + b] = src1.raw_data(b);
    }
  }
}

// コピーコンストラクタ
TvFuncM::TvFuncM(
  const TvFuncM& src
) : mInputNum{src.mInputNum},
    mOutputNum{src.mOutputNum},
    mBlockNum1{src.mBlockNum1},
    mBlockNum{src.mBlockNum},
    mVector{src.mVector}
{
}

// ムーブコンストラクタ
TvFuncM::TvFuncM(
  TvFuncM&& src
) : mInputNum{src.mInputNum},
    mOutputNum{src.mOutputNum},
    mBlockNum1{src.mBlockNum1},
    mBlockNum{src.mBlockNum},
    mVector{std::move(src.mVector)}
{
  src.mInputNum = 0;
  src.mOutputNum = 0;
  src.mBlockNum1 = 0;
  src.mBlockNum = 0;
}

// @brief TvFunc からの変換用コンストラクタ
TvFuncM::TvFuncM(
  const TvFunc& src
) : mInputNum{src.mInputNum},
    mOutputNum{1},
    mBlockNum1{nblock(mInputNum)},
    mBlockNum{mBlockNum1},
    mVector{src.mVector}
{
}

// @brief TvFunc からの変換用ムーブコンストラクタ
TvFuncM::TvFuncM(
  TvFunc&& src
) : mInputNum{src.input_num()},
    mOutputNum{1},
    mBlockNum1{nblock(mInputNum)},
    mBlockNum{mBlockNum1},
    mVector{std::move(src.mVector)}
{
  src.mInputNum = 0;
  src.mBlockNum = 0;
}

// コピー代入演算子
TvFuncM&
TvFuncM::operator=(
  const TvFuncM& src
)
{
  if ( &src != this ) {
    mInputNum = src.mInputNum;
    mOutputNum = src.mOutputNum;
    mBlockNum1 = src.mBlockNum1;
    mBlockNum = src.mBlockNum;
    mVector = src.mVector;
  }

  return *this;
}

// ムーブ代入演算子
TvFuncM&
TvFuncM::operator=(
  TvFuncM&& src
)
{
  mInputNum = src.mInputNum;
  mOutputNum = src.mOutputNum;
  mBlockNum1 = src.mBlockNum1;
  mBlockNum = src.mBlockNum;
  std::swap(mVector, src.mVector);

  src.mInputNum = 0;
  src.mOutputNum = 0;
  src.mBlockNum1 = 0;
  src.mBlockNum = 0;

  return *this;
}

// デストラクタ
TvFuncM::~TvFuncM()
{
}

// 自分自身を否定する．
TvFuncM&
TvFuncM::invert_int()
{
  switch ( mInputNum ) {
  case 0:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0x1ULL;
    }
    break;

  case 1:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0x3ULL;
    }
    break;

  case 2:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0xFULL;
    }
    break;

  case 3:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0xFFULL;
    }
    break;

  case 4:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0xFFFFULL;
    }
    break;

  case 5:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0xFFFFFFFFULL;
    }
    break;

  case 6:
    for ( int i: Range(mOutputNum) ) {
      mVector[i] ^= 0xFFFFFFFFFFFFFFFFULL;
    }
    break;

  default:
    for ( int b: Range(mBlockNum) ) {
      mVector[b] ^= 0xFFFFFFFFFFFFFFFULL;
    }
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
TvFuncM&
TvFuncM::and_int(
  const TvFuncM& src1
)
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] &= src1.mVector[b];
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
TvFuncM&
TvFuncM::or_int(
  const TvFuncM& src1
)
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] |= src1.mVector[b];
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
TvFuncM&
TvFuncM::xor_int(
  const TvFuncM& src1
)
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] ^= src1.mVector[b];
  }
  return *this;
}

// @brief コファクターを計算し自分に代入する．
// @param[in] varid 変数番号
// @param[in] pol 極性
// @return 自身への参照を返す．
TvFuncM&
TvFuncM::cofactor_int(
  SizeType varid,
  bool inv
)
{
  if ( varid < NIPW ) {
    TvFuncM::WordType mask = c_masks[varid];
    if ( inv ) {
      mask = ~mask;
    }
    int shift = 1 << varid;
    for ( int b: Range(mBlockNum) ) {
      TvFuncM::WordType pat = mVector[b] & mask;
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
    varid -= NIPW;
    int bit = 1U << varid;
    for ( int i: Range(mOutputNum) ) {
      int offset = i * mBlockNum1;
      for ( int b: Range(mBlockNum1) ) {
	if ( inv ) {
	  if ( (b & bit) == bit ) {
	    mVector[b + offset] = mVector[(b ^ bit) + offset];
	  }
	}
	else {
	  if ( (b & bit) == 0U ) {
	    mVector[b + offset] = mVector[(b ^ bit) + offset];
	  }
	}
      }
    }
  }
  return *this;
}

// @brief 1出力の論理関数を切り出す．
// @param[in] ovar 出力番号
TvFunc
TvFuncM::slice(
  SizeType ovar
) const
{
#if 1
  TvFunc ans(input_num());
  int offset = mBlockNum1 * ovar;
  for ( int b: Range(mBlockNum1) ) {
    ans.mVector[b] = mVector[b + offset];
  }
  return ans;
#else
#warning "効率が悪い仮のコード"

  int np = 1U << input_num();
  vector<int> tmp(np);
  for ( int i = 0; i < np; ++ i ) {
    tmp[i] = value(ovar, i);
  }
  return TvFunc(input_num(), tmp);
#endif
}

// var がサポートの時 true を返す．
bool
TvFuncM::check_sup(
  SizeType var
) const
{
  if ( var < NIPW ) {
    // ブロックごとにチェック
    int dist = 1U << var;
    TvFuncM::WordType mask = c_masks[var];
    for ( int b: Range(mBlockNum) ) {
      TvFuncM::WordType word = mVector[b];
      if ( (word ^ (word << dist)) & mask ) {
	return true;
      }
    }
  }
  else {
    // ブロック単位でチェック
    int i5 = var - NIPW;
    int check = 1 << i5;
    for ( int i: Range(mOutputNum) ) {
      int offset = var * mBlockNum1;
      for ( int b: Range(mBlockNum1) ) {
	if ( (b & check) &&
	     (mVector[b + offset] != mVector[(b ^ check) + offset]) ) {
	  return true;
	}
      }
    }
  }
  return false;
}

// var1 と var2 の変数が対称のとき true を返す．
bool
TvFuncM::check_sym(
  SizeType var1,
  SizeType var2,
  bool inv
) const
{
  // var1 と var2 を正規化する．
  if ( var1 < var2 ) {
    std::swap(var1, var2);
  }
  // ここ以降では必ず var1 > var2 が成り立つ．

  bool ans = true;
  if ( var2 >= NIPW ) {
    // var1 >= NIPW
    // var2 >= NIPW
    // ブロック単位で比較する．
    int mask_i = (1 << (var1 - NIPW));
    int mask_j = (1 << (var2 - NIPW));
    int mask_all = mask_i | mask_j;
    int cond = inv ? 0 : mask_j;
    for ( int i: Range(mOutputNum) ) {
      int offset = i * mBlockNum1;
      for ( int v: Range(mBlockNum1) ) {
	if ( (v & mask_all) == cond &&
	     mVector[v + offset] != mVector[(v ^ mask_all) + offset] ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  else if ( var1 >= NIPW ) {
    // var1 >= NIPW
    // var2 < NIPW
    int mask_i = (1U << (var1 - NIPW));
    int cond = inv ? 0UL : mask_i;
    TvFuncM::WordType mask2 = ~c_masks[var2];
    int s = 1 << var2;
    for ( int i: Range(mOutputNum) ) {
      int offset = i * mBlockNum1;
      for ( int v: Range(mBlockNum1) ) {
	if ( (v & mask_i) == cond &&
	     (mVector[v + offset] ^ (mVector[(v ^ mask_i) + offset] >> s)) & mask2 ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  else {
    // var1 < NIPW
    // var2 < NIPW
    if ( inv ) {
      TvFuncM::WordType mask = sym_masks3[(var1 * (var1 - 1)) / 2 + var2];
      int s = (1U << var1) + (1U << var2);
      for ( int b: Range(mBlockNum) ) {
	TvFuncM::WordType word = mVector[b];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
    else {
      TvFuncM::WordType mask = sym_masks2[(var1 * (var1 - 1)) / 2 + var2];
      int s = (1U << var1) - (1U << var2);
      for ( int b: Range(mBlockNum) ) {
	TvFuncM::WordType word = mVector[b];
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
TvFuncM
TvFuncM::xform(
  const NpnMapM& npnmap
) const
{
  SizeType ni_pow = 1UL << mInputNum;

#if defined(DEBUG)
  cout << "xform" << endl
       << *this << endl
       << npnmap << endl;
#endif

  SizeType imask = 0;
  SizeType ipat[kMaxNi];
  for ( SizeType src_var: Range(mInputNum) ) {
    NpnVmap imap = npnmap.imap(src_var);
    if ( imap.inv() ) {
      imask |= (1UL << src_var);
    }
    auto dst_var = imap.var();
    ipat[src_var] = 1 << dst_var;
  }

  TvFuncM ans(mInputNum, mOutputNum);

  for ( SizeType src_var: Range(mOutputNum) ) {
    auto omap = npnmap.omap(src_var);
    auto dst_var = omap.var();
    TvFuncM::WordType omask = omap.inv() ? 1ULL : 0ULL;
    for ( SizeType p: Range(ni_pow) ) {
      SizeType new_p = 0;
      SizeType tmp = p;
      for ( int i: Range(mInputNum) ) {
	if ( tmp & 1 ) {
	  new_p |= ipat[i];
	}
	tmp >>= 1;
      }
      TvFuncM::WordType pat = (value(src_var, p ^ imask) ^ omask);
      ans.mVector[block(new_p) + dst_var * mBlockNum1] |= pat << shift(new_p);
    }
  }

#if defined(DEBUG)
  cout << ans << endl;
#endif

  return ans;
}

// ハッシュ値を返す．
SizeType
TvFuncM::hash() const
{
  SizeType ans = 0;
  for ( int b: Range(mBlockNum) ) {
    TvFuncM::WordType tmp = mVector[b];
    TvFuncM::WordType tmp_l = (tmp >>  0) & 0xFFFFFFFFULL;
    TvFuncM::WordType tmp_h = (tmp >> 32) & 0xFFFFFFFFULL;
    ans ^= (tmp_l ^ tmp_h);
  }
  return ans + mInputNum + (mOutputNum << 8);
}

// @relates TvFunc
// @brief 比較関数
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
int
compare(
  const TvFuncM& left,
  const TvFuncM& right
)
{
  if ( left.mInputNum < right.mInputNum ) {
    return -1;
  }
  if ( left.mInputNum > right.mInputNum ) {
    return 1;
  }

  if ( left.mOutputNum < right.mOutputNum ) {
    return -1;
  }
  if ( left.mOutputNum > right.mOutputNum ) {
    return 1;
  }

  SizeType n = left.mBlockNum;
  for ( SizeType b: Range(n) ) {
    TvFuncM::WordType w1 = left.mVector[b];
    TvFuncM::WordType w2 = right.mVector[b];
    if ( w1 < w2 ) {
      return -1;
    }
    if ( w1 > w2 ) {
      return 1;
    }
  }

  return 0;
}

// @relates TvFuncM
// @brief 交差チェック
bool
operator&&(
  const TvFuncM& func1,
  const TvFuncM& func2
)
{
  if ( func1.mInputNum != func2.mInputNum ||
       func1.mOutputNum != func2.mOutputNum ) {
    return false;
  }

  SizeType n = func1.mBlockNum;
  for ( SizeType b: Range(n) ) {
    TvFuncM::WordType w1 = func1.mVector[b];
    TvFuncM::WordType w2 = func2.mVector[b];
    if ( (w1 & w2) != 0U ) {
      return true;
    }
  }
  return false;
}

// 内容の出力
// mode は 2 か 16
void
TvFuncM::print(
  std::ostream& s,
  int mode
) const
{
  SizeType ni_pow = 1UL << mInputNum;
  const SizeType wordsize = sizeof(TvFuncM::WordType) * 8;
  if ( mode == 2 ) {
    auto bp = mVector.begin();
    TvFuncM::WordType tmp = *bp;
    const char* del = "";
    for ( int o: Range(mOutputNum) ) {
      s << del;
      del = "|";
      int offset = 0;
      for ( int p: Range(ni_pow) ) {
	s << (tmp & 1L);
	tmp >>= 1;
	++ offset;
	if ( offset == wordsize ) {
	  offset = 0;
	  ++ bp;
	  tmp = *bp;
	}
      }
      if ( offset != 0 ) {
	++ bp;
	tmp = *bp;
      }
    }
  }
  else if ( mode == 16 ) {
    SizeType ni_pow4 = ni_pow / 4;
    auto  bp = mVector.begin();
    TvFuncM::WordType tmp = *bp;
    for ( int o: Range(mOutputNum) ) {
      const char* del = "";
      int offset = 0;
      for ( int p: Range(ni_pow4) ) {
	s << del;
	del = "|";
	TvFuncM::WordType tmp1 = (tmp & 0xFULL);
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
      if ( offset != 0 ) {
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
TvFuncM::dump(
  BinEnc& s
) const
{
  s.write_64(mInputNum);
  s.write_64(mOutputNum);
  for ( auto b: Range(mBlockNum) ) {
    s.write_64(mVector[b]);
  }
}

// @brief バイナリファイルの読み込み
// @param[in] s 入力元のストリーム
void
TvFuncM::restore(
  BinDec& s
)
{
  mInputNum = s.read_64();
  mOutputNum = s.read_64();
  mBlockNum1 = nblock(mInputNum);
  mBlockNum = mBlockNum1 * mOutputNum;
  mVector.resize(mBlockNum);
  for ( auto b: Range(mBlockNum) ) {
    mVector[b] = s.read_64();
  }
}

END_NAMESPACE_YM
