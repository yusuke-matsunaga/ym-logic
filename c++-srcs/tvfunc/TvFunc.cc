
/// @file TvFunc.cc
/// @brief TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017, 2022-2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "ym/Range.h"
#include "NpnMgr.h"


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

// @brief 不正値を作るコンストラクタ
TvFunc::TvFunc(
) : mInputNum{0},
    mBlockNum{0},
    mVector{nullptr}
{
}

// 入力数のみ指定したコンストラクタ
// 中身は恒偽関数
TvFunc::TvFunc(
  SizeType ni
) : mInputNum{ni},
    mBlockNum{nblock(ni)},
    mVector{new TvFunc::WordType[mBlockNum]}
{
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] = 0UL;
  }
}

// 恒真関数を作るコンストラクタ
// 2番目の引数はダミー
TvFunc::TvFunc(
  SizeType ni,
  int dummy
) : mInputNum{ni},
    mBlockNum{nblock(ni)},
    mVector{new TvFunc::WordType[mBlockNum]}
{
  TvFunc::WordType mask = vec_mask(ni);
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] = mask;
  }
}

// リテラル関数を作るコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  SizeType varid,
  bool inv
) : mInputNum{ni},
    mBlockNum{nblock(ni)},
    mVector{new TvFunc::WordType[mBlockNum]}
{
  ASSERT_COND( varid != BAD_VARID );
  ASSERT_COND( varid < ni );

  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType pat = lit_pat(varid, b);
    if ( inv ) {
      pat = ~pat;
    }
    mVector[b] = pat;
  }
  mVector[0] &= vec_mask(ni);
}

// 入力数と真理値を指定したコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  const vector<int>& values
) : mInputNum{ni},
    mBlockNum{nblock(ni)},
    mVector{new TvFunc::WordType[mBlockNum]}
{
  SizeType ni_pow = 1 << ni;
  ASSERT_COND( values.size() == ni_pow );
  SizeType base = 0;
  TvFunc::WordType bitpat = 0ULL;
  TvFunc::WordType bitmask = 1ULL;
  for ( SizeType p: Range(ni_pow) ) {
    if ( values[p] ) {
      bitpat |= bitmask;
    }
    bitmask <<= 1;
    if ( bitmask == 0UL ) {
      mVector[base] = bitpat;
      ++ base;
      bitmask = 1ULL;
      bitpat = 0ULL;
    }
  }
  if ( bitmask != 1ULL ) {
    mVector[base] = bitpat;
    ++ base;
  }
}

// @brief 文字列からの変換コンストラクタ
TvFunc::TvFunc(
  const string& str
)
{
  SizeType ni;
  SizeType n = str.size();
  ni = 0;
  while ( (1 << ni) < n ) {
    ++ ni;
  }
  ASSERT_COND( (1 << ni) == n );

  mInputNum = ni;
  mBlockNum = nblock(ni);
  mVector = new TvFunc::WordType[mBlockNum];
  SizeType base = 0;
  TvFunc::WordType bitpat = 0ULL;
  TvFunc::WordType bitmask = 1ULL;
  for ( SizeType i = 0; i < n; ++ i ) {
    char c = str[n - i - 1];
    if ( c == '0' ) {
      ;
    }
    else if ( c == '1' ) {
      bitpat |= bitmask;
    }
    else {
      ASSERT_NOT_REACHED;
    }
    bitmask <<= 1;
    if ( bitmask == 0UL ) {
      mVector[base] = bitpat;
      ++ base;
      bitmask = 1ULL;
      bitpat = 0ULL;
    }
  }
  if ( bitmask != 1ULL ) {
    mVector[base] = bitpat;
    ++ base;
  }

}

// コピーコンストラクタ
TvFunc::TvFunc(
  const TvFunc& src
) : mInputNum{src.mInputNum},
    mBlockNum{src.mBlockNum},
    mVector{new TvFunc::WordType[mBlockNum]}
{
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] = src.mVector[b];
  }
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元のソースオブジェクト
TvFunc::TvFunc(
  TvFunc&& src
) : mInputNum{src.mInputNum},
    mBlockNum{src.mBlockNum},
    mVector{src.mVector}
{
  src.mInputNum = 0;
  src.mBlockNum = 0;
  src.mVector = nullptr;
}

// コピー代入演算子
TvFunc&
TvFunc::operator=(
  const TvFunc& src
)
{
  if ( mBlockNum != src.mBlockNum ) {
    delete [] mVector;
    mBlockNum = src.mBlockNum;
    mVector = new TvFunc::WordType[mBlockNum];
  }
  mInputNum = src.mInputNum;

  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] = src.mVector[b];
  }

  return *this;
}

// @brief ムーブ代入演算子
// @return 自分自身への参照を返す．
TvFunc&
TvFunc::operator=(
  TvFunc&& src
)
{
  if ( mVector != nullptr ) {
    delete [] mVector;
  }

  mInputNum = src.mInputNum;
  mBlockNum = src.mBlockNum;
  mVector = src.mVector;

  src.mInputNum = 0;
  src.mBlockNum = 0;
  src.mVector = nullptr;

  return *this;
}

// デストラクタ
TvFunc::~TvFunc()
{
  delete [] mVector;
}

// 自分自身を否定する．
TvFunc&
TvFunc::invert_int()
{
  TvFunc::WordType neg_mask = vec_mask(mInputNum);
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] ^= neg_mask;
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
TvFunc&
TvFunc::and_int(
  const TvFunc& src1
)
{
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] &= src1.mVector[b];
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
TvFunc&
TvFunc::or_int(
  const TvFunc& src1
)
{
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] |= src1.mVector[b];
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
TvFunc&
TvFunc::xor_int(
  const TvFunc& src1
)
{
  for ( SizeType b: Range(mBlockNum) ) {
    mVector[b] ^= src1.mVector[b];
  }
  return *this;
}

// @brief コファクターを計算し自分に代入する．
TvFunc&
TvFunc::cofactor_int(
  SizeType varid,
  bool inv
)
{
  ASSERT_COND( varid != BAD_VARID );
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
  return *this;
}

// @brief 定数0関数の時に true を返す．
bool
TvFunc::is_zero() const
{
  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType word = mVector[b];
    if ( word != 0UL ) {
      return false;
    }
  }
  return true;
}

// @brief 定数1関数の時に true を返す．
bool
TvFunc::is_one() const
{
  TvFunc::WordType mask = vec_mask(mInputNum);
  if ( mInputNum < NIPW ) {
    return (mVector[0] & mask) == mask;
  }
  else {
    for ( SizeType b: Range(mBlockNum) ) {
      TvFunc::WordType word = mVector[b];
      if ( word != mask ) {
	return false;
      }
    }
    return true;
  }
}

// 変数がサポートの時 true を返す．
bool
TvFunc::check_sup(
  SizeType var
) const
{
  ASSERT_COND( var != BAD_VARID );
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
  ASSERT_COND( varid != BAD_VARID );
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
  ASSERT_COND( var1 != BAD_VARID );
  ASSERT_COND( var2 != BAD_VARID );
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

// npnmap に従った変換を行う．
TvFunc
TvFunc::xform(
  const NpnMap& npnmap
) const
{
  if ( is_invalid() ) {
    // 不正値は変換しても不正値
    return TvFunc::make_invalid();
  }

#if defined(DEBUG)
  cout << "xform" << endl
       << *this << endl
       << npnmap << endl;
#endif

  SizeType new_ni = npnmap.input_num2();
  SizeType imask = 0;
  SizeType ipat[kMaxNi];
  for ( SizeType i: Range(new_ni) ) {
    ipat[i] = 0;
  }
  for ( SizeType src_var: Range(mInputNum) ) {
    auto imap = npnmap.imap(src_var);
    if ( imap.is_invalid() ) {
      continue;
    }
    if ( imap.inv() ) {
      imask |= (1 << src_var);
    }
    auto dst_var = imap.var();
    ipat[dst_var] = 1 << src_var;
  }
  TvFunc::WordType omask = npnmap.oinv() ? 1ULL : 0ULL;

  TvFunc ans{new_ni};
  SizeType ni_pow = 1 << new_ni;
  for ( SizeType b: Range(ni_pow) ) {
    SizeType orig_b = 0;
    SizeType tmp = b;
    for ( SizeType i: Range(new_ni) ) {
      if ( tmp & 1 ) {
	orig_b |= ipat[i];
      }
      tmp >>= 1;
    }
    TvFunc::WordType pat = (value(orig_b ^ imask) ^ omask);
    ans.mVector[block(b)] |= pat << shift(b);
  }

#if defined(DEBUG)
  cout << ans << endl;
#endif

  return ans;
}

// @brief 独立な変数を取り除く変換を返す．
NpnMap
TvFunc::shrink_map() const
{
  // まず独立な変数を求める．
  SizeType varmap = 0U;
  SizeType dst_ni = 0;
  for ( SizeType i: Range(mInputNum) ) {
    if ( !check_sup(i) ) {
      varmap |= (1U << i);
    }
    else {
      ++ dst_ni;
    }
  }
  // varmap に 1 のビットが立っているところが独立な変数
  if ( varmap == 0U ) {
    // 独立な変数はなかった．
    // 恒等変換を返す．
    return NpnMap(mInputNum);
  }

  // npnmap を設定する．
  NpnMap ans(mInputNum, dst_ni);
  SizeType j = 0;
  SizeType rmap[kMaxNi];
  for ( SizeType i: Range(mInputNum) ) {
    if ( (varmap & (1U << i)) == 0U ) {
      ans.set(i, j, false);
      rmap[j] = i;
      ++ j;
    }
  }
  return ans;
}

// @brief npn 変換の正規変換を求める．
NpnMap
TvFunc::npn_cannonical_map() const
{
  nsLogic::NpnMgr npn_mgr;
  npn_mgr.cannonical(*this);
  vector<NpnMap> cmap_list;
  npn_mgr.all_cmap(cmap_list);
  ASSERT_COND( !cmap_list.empty() );
  return cmap_list.front();
}

// @brief npn 変換の正規変換をすべて求める．
vector<NpnMap>
TvFunc::npn_cannonical_all_map() const
{
  nsLogic::NpnMgr npn_mgr;
  npn_mgr.cannonical(*this);
  vector<NpnMap> map_list;
  npn_mgr.all_cmap(map_list);
  return map_list;
}

// @brief 内容を表す文字列を返す．
string
TvFunc::str(
  int mode
) const
{
  ostringstream buf;
  print(buf, mode);
  return buf.str();
}

// ハッシュ値を返す．
SizeType
TvFunc::hash() const
{
  SizeType ans = 0;
  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType tmp = mVector[b];
    TvFunc::WordType tmp_l = (tmp >>  0) & 0xFFFFFFFFULL;
    TvFunc::WordType tmp_h = (tmp >> 32) & 0xFFFFFFFFULL;
    ans ^= tmp_l ^ tmp_h;
  }
  return ans + mInputNum;
}

// 比較関数
int
compare(
  const TvFunc& func1,
  const TvFunc& func2
)
{
  // まず入力数を比較する．
  if ( func1.mInputNum < func2.mInputNum ) {
    return -1;
  }
  if ( func1.mInputNum > func2.mInputNum ) {
    return 1;
  }

  // 以降は入力数が等しい場合
  SizeType n = func1.mBlockNum;
  for ( SizeType b: Range(n) ) {
    TvFunc::WordType w1 = func1.mVector[n - b - 1];
    TvFunc::WordType w2 = func2.mVector[n - b - 1];
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
operator&&(
  const TvFunc& func1,
  const TvFunc& func2
)
{
  if ( func1.mInputNum != func2.mInputNum ) {
    return false;
  }

  SizeType n = func1.mBlockNum;
  for ( SizeType b: Range(n) ) {
    TvFunc::WordType w1 = func1.mVector[n - b - 1];
    TvFunc::WordType w2 = func2.mVector[n - b - 1];
    if ( (w1 & w2) != 0U ) {
      return true;
    }
  }
  return false;
}

// 内容の出力
// mode は 2 か 16
void
TvFunc::print(
  ostream& s,
  int mode
) const
{
  if ( is_invalid() ) {
    // 空
    return;
  }

  // value(0) は LSB なので右端となる．
  // 出力するのは左端からなので反転させる必要がある．
  SizeType ni_pow = 1 << mInputNum;
  const SizeType wordsize = sizeof(TvFunc::WordType) * 8;
  if ( mode == 2 ) {
    for ( SizeType p: Range(ni_pow) ) {
      s << value(ni_pow - p - 1);
    }
  }
  else if ( mode == 16 ) {
    switch ( mInputNum ) {
    case 0:
      if ( value(0) ) {
	s << "1";
      }
      else {
	s << "0";
      }
      break;
    case 1:
      {
	int v = value(0) + value(1) * 2;
	s << v;
      }
      break;
    default:
      for ( SizeType p: Range_<SizeType, 4>(ni_pow) ) {
	SizeType pos = ni_pow - p - 4;
	int v = value(pos + 0);
	v += value(pos + 1) * 2;
	v += value(pos + 2) * 4;
	v += value(pos + 3) * 8;
	s << hex << v << dec;
      }
      break;
    }
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief バイナリファイルの書き出し
void
TvFunc::dump(
  BinEnc& s
) const
{
  if ( is_invalid() ) {
    // 不正値
    s << static_cast<SizeType>(-1);
  }
  else {
    s << mInputNum;
    s.write_block(reinterpret_cast<const ymuint8*>(mVector), mBlockNum * sizeof(WordType));
  }
}

// @brief バイナリファイルの読み込み
void
TvFunc::restore(
  BinDec& s
)
{
  s >> mInputNum;
  if ( mInputNum == - 1 ) {
    // 不正値
    mInputNum = 0;
    mBlockNum = 0;
    if ( mVector != nullptr ) {
      delete [] mVector;
    }
    mVector = nullptr;
  }
  else {
    SizeType nblk = nblock(mInputNum);
    if ( mBlockNum != nblk ) {
      delete [] mVector;
      mBlockNum = nblk;
      mVector = new TvFunc::WordType[mBlockNum];
    }
    s.read_block(reinterpret_cast<ymuint8*>(mVector), mBlockNum * sizeof(WordType));
  }
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
