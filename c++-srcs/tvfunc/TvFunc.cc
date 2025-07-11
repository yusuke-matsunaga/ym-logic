
/// @file TvFunc.cc
/// @brief TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "ym/SopCube.h"
#include "ym/Range.h"
#include "npn/NpnMgr.h"


// 1 ワード当たりの入力数
#define NIPW 6

//#define DEBUG

BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

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
    mVector(0)
{
}

// 入力数のみ指定したコンストラクタ
// 中身は恒偽関数
TvFunc::TvFunc(
  SizeType ni
) : mInputNum{ni},
    mBlockNum{nblock(ni)},
    mVector(mBlockNum, 0ULL)
{
}

// 恒真関数を作るコンストラクタ
// 2番目の引数はダミー
TvFunc::TvFunc(
  SizeType ni,
  int dummy
) : mInputNum{ni},
    mBlockNum{nblock(ni)},
    mVector(mBlockNum, vec_mask(ni))
{
}

// リテラル関数を作るコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  SizeType varid,
  bool inv
) : TvFunc(ni)
{
  _check_varid(varid);

  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType pat = lit_pat(varid, b);
    if ( inv ) {
      pat = ~pat;
    }
    mVector[b] = pat;
  }
  mVector[0] &= vec_mask(ni);
}

// @brief カバーに対応した関数を作るコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  const std::vector<std::vector<Literal>>& cube_list
) : TvFunc(ni)
{
  for ( auto& cube: cube_list ) {
    for ( auto lit: cube ) {
      _check_varid(lit.varid());
    }
  }

  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType pat0 = 0x0000000000000000ULL;
    for ( auto& cube: cube_list ) {
      TvFunc::WordType pat1 = 0xFFFFFFFFFFFFFFFFULL;
      for ( auto lit: cube ) {
	auto varid = lit.varid();
	auto inv = lit.is_negative();
	auto pat2 = lit_pat(varid, b);
	if ( inv ) {
	  pat2 = ~pat2;
	}
	pat1 &= pat2;
      }
      pat0 |= pat1;
    }
    mVector[b] = pat0;
  }
  mVector[0] &= vec_mask(ni);
}

// @brief カバー関数を作るコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  const std::vector<SopCube>& cube_list
) : TvFunc(ni)
{
  for ( auto& cube: cube_list ) {
    if ( cube.variable_num() != ni ) {
      throw std::invalid_argument{"input_num mismatch"};
    }
  }

  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType pat0 = 0x0000000000000000ULL;
    for ( auto& cube: cube_list ) {
      TvFunc::WordType pat1 = 0xFFFFFFFFFFFFFFFFULL;
      for ( auto lit: cube.literal_list() ) {
	auto varid = lit.varid();
	auto inv = lit.is_negative();
	auto pat2 = lit_pat(varid, b);
	if ( inv ) {
	  pat2 = ~pat2;
	}
	pat1 &= pat2;
      }
      pat0 |= pat1;
    }
    mVector[b] = pat0;
  }
  mVector[0] &= vec_mask(ni);
}

// @brief キューブに対応した関数を作るコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  const std::vector<Literal>& lit_list
) : TvFunc(ni)
{
  for ( auto lit: lit_list ) {
    _check_varid(lit.varid());
  }

  for ( SizeType b: Range(mBlockNum) ) {
    TvFunc::WordType pat = 0xFFFFFFFFFFFFFFFFULL;
    for ( auto lit: lit_list ) {
      auto varid = lit.varid();
      auto inv = lit.is_negative();
      auto pat1 = lit_pat(varid, b);
      if ( inv ) {
	pat1 = ~pat1;
      }
      pat &= pat1;
    }
    mVector[b] = pat;
  }
  mVector[0] &= vec_mask(ni);
}

// 入力数と真理値を指定したコンストラクタ
TvFunc::TvFunc(
  SizeType ni,
  const std::vector<int>& values
) : TvFunc(ni)
{
  SizeType ni_pow = 1 << ni;
  if ( values.size() != ni_pow ) {
    auto emsg = "the size of 'values' should be 2 to the power of 'ni'";
    throw std::invalid_argument{emsg};
  }

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

BEGIN_NONAMESPACE

SizeType
get_ni(
  const std::string& str
)
{
  // str が適切な文字列か確認する．
  for ( char c: str ) {
    if ( c != '0' && c != '1' ) {
      throw std::invalid_argument("only '0' or '1' are expected");
    }
  }
  // 長さが2のべき乗になっているか確認する．
  SizeType ni;
  SizeType n = str.size();
  ni = 0;
  while ( (1 << ni) < n ) {
    ++ ni;
  }
  if ( (1 << ni) != n ) {
    auto emsg = "the data length is expected to be the power of 2";
    throw std::invalid_argument(emsg);
  }
  return ni;
}

END_NONAMESPACE

// @brief 文字列からの変換コンストラクタ
TvFunc::TvFunc(
  const std::string& str
) : TvFunc(get_ni(str))
{
  SizeType base = 0;
  TvFunc::WordType bitpat = 0ULL;
  TvFunc::WordType bitmask = 1ULL;
  // 文字列はLSBが右になるようになっている．
  SizeType n = str.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    char c = str[n - i - 1];
    if ( c == '1' ) {
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
  }
}

// コピーコンストラクタ
TvFunc::TvFunc(
  const TvFunc& src
) : mInputNum{src.mInputNum},
    mBlockNum{src.mBlockNum},
    mVector{src.mVector}
{
}

// @brief ムーブコンストラクタ
TvFunc::TvFunc(
  TvFunc&& src
) : mInputNum{src.mInputNum},
    mBlockNum{src.mBlockNum},
    mVector{std::move(src.mVector)}
{
  src.mInputNum = 0;
  src.mBlockNum = 0;
}

// コピー代入演算子
TvFunc&
TvFunc::operator=(
  const TvFunc& src
)
{
  if ( &src != this ) {
    mInputNum = src.mInputNum;
    mBlockNum = src.mBlockNum;
    mVector = src.mVector;
  }
  return *this;
}

// @brief ムーブ代入演算子
TvFunc&
TvFunc::operator=(
  TvFunc&& src
)
{
  mInputNum = src.mInputNum;
  mBlockNum = src.mBlockNum;
  std::swap(mVector, src.mVector);

  src.mInputNum = 0;
  src.mBlockNum = 0;

  return *this;
}

// デストラクタ
TvFunc::~TvFunc()
{
}

// 自分自身を否定する．
TvFunc&
TvFunc::invert_int()
{
  if ( is_valid() ) {
    TvFunc::WordType neg_mask = vec_mask(mInputNum);
    for ( SizeType b: Range(mBlockNum) ) {
      mVector[b] ^= neg_mask;
    }
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
TvFunc&
TvFunc::and_int(
  const TvFunc& src1
)
{
  if ( is_valid() ) {
    _check_size(src1);
    for ( SizeType b: Range(mBlockNum) ) {
      mVector[b] &= src1.mVector[b];
    }
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
TvFunc&
TvFunc::or_int(
  const TvFunc& src1
)
{
  if ( is_valid() ) {
    _check_size(src1);
    for ( SizeType b: Range(mBlockNum) ) {
      mVector[b] |= src1.mVector[b];
    }
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
TvFunc&
TvFunc::xor_int(
  const TvFunc& src1
)
{
  if ( is_valid() ) {
    _check_size(src1);
    for ( SizeType b: Range(mBlockNum) ) {
      mVector[b] ^= src1.mVector[b];
    }
  }
  return *this;
}

// @brief 定数0関数の時に true を返す．
bool
TvFunc::is_zero() const
{
  if ( is_invalid() ) {
    // 不正な関数なら定数ではない．
    return false;
  }

  for ( SizeType b: Range(mBlockNum) ) {
    auto word = mVector[b];
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
  if ( is_invalid() ) {
    // 不正な関数なら定数ではない．
    return false;
  }

  auto mask = vec_mask(mInputNum);
  if ( mInputNum < NIPW ) {
    return (mVector[0] & mask) == mask;
  }
  else {
    for ( SizeType b: Range(mBlockNum) ) {
      auto word = mVector[b];
      if ( word != mask ) {
	return false;
      }
    }
    return true;
  }
}

// npnmap に従った変換を行う．
TvFunc
TvFunc::xform(
  const NpnMap& npnmap
) const
{
  if ( is_invalid() ) {
    // 不正値は変換しても不正値
    return TvFunc::invalid();
  }

#if defined(DEBUG)
  std::cout << "xform" << std::endl
	    << *this << std::endl
	    << npnmap << std::endl;
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

  TvFunc ans(new_ni);
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
  std::cout << ans << std::endl;
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
      ans.set_imap(i, j, false);
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
  std::vector<NpnMap> cmap_list;
  npn_mgr.all_cmap(cmap_list);
  ASSERT_COND( !cmap_list.empty() );
  return cmap_list.front();
}

// @brief npn 変換の正規変換をすべて求める．
std::vector<NpnMap>
TvFunc::npn_cannonical_all_map() const
{
  nsLogic::NpnMgr npn_mgr;
  npn_mgr.cannonical(*this);
  std::vector<NpnMap> map_list;
  npn_mgr.all_cmap(map_list);
  return map_list;
}

// @brief 内容を表す文字列を返す．
std::string
TvFunc::str(
  int mode
) const
{
  std::ostringstream buf;
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
TvFunc::_compare(
  const TvFunc& func2
) const
{
  _check_size(func2);

  // 以降は入力数が等しい場合
  SizeType n = mBlockNum;
  for ( SizeType b: Range(n) ) {
    auto w1 = mVector[n - b - 1];
    auto w2 = func2.mVector[n - b - 1];
    if ( w1 < w2 ) {
      return -1;
    }
    if ( w1 > w2 ) {
      return 1;
    }
  }
  return 0;
}

// @brief 交差チェック
bool
TvFunc::check_intersect(
  const TvFunc& func2
) const
{
  _check_size(func2);

  SizeType n = mBlockNum;
  for ( SizeType b: Range(n) ) {
    auto w1 = mVector[n - b - 1];
    auto w2 = func2.mVector[n - b - 1];
    if ( (w1 & w2) != 0U ) {
      return true;
    }
  }
  return false;
}

// @brief 包含チェック
bool
TvFunc::check_containment(
  const TvFunc& func2
) const
{
  _check_size(func2);

  SizeType n = mBlockNum;
  for ( SizeType b: Range(n) ) {
    auto w1 = mVector[n - b - 1];
    auto w2 = func2.mVector[n - b - 1];
    if ( (w1 & ~w2) != 0U ) {
      return false;
    }
  }
  return true;
}

// 内容の出力
// mode は 2 か 16
void
TvFunc::print(
  std::ostream& s,
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
	s << std::hex << v << std::dec;
      }
      break;
    }
  }
  else {
    throw std::invalid_argument{"'mode' should be 2 or 16"};
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
    s.write_64(-1);
  }
  else {
    s.write_64(mInputNum);
    for ( auto b: Range(mBlockNum) ) {
      s.write_64(mVector[b]);
    }
  }
}

// @brief バイナリファイルの読み込み
void
TvFunc::restore(
  BinDec& s
)
{
  mInputNum = s.read_64();
  if ( mInputNum == - 1 ) {
    // 不正値
    mInputNum = 0;
    mBlockNum = 0;
    mVector.clear();
  }
  else {
    SizeType nblk = nblock(mInputNum);
    if ( mBlockNum != nblk ) {
      mBlockNum = nblk;
      mVector.resize(nblk);
    }
    for ( auto b: Range(mBlockNum) ) {
      mVector[b] = s.read_64();
    }
  }
}

END_NAMESPACE_YM
