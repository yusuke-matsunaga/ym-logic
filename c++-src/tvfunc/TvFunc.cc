﻿
/// @file TvFunc.cc
/// @brief TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
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
vec_mask(int ni)
{
  TvFunc::WordType mask = ~0ULL;
  if ( ni < NIPW ) {
    int ni_exp = 1 << ni;
    mask >>= (64 - ni_exp);
  }
  return mask;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

// 入力数のみ指定したコンストラクタ
// 中身は恒偽関数
TvFunc::TvFunc(int ni) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new TvFunc::WordType[mBlockNum])
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] = 0UL;
  }
}

// 恒真関数を作るコンストラクタ
// 2番目の引数はダミー
TvFunc::TvFunc(int ni,
	       int dummy) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new TvFunc::WordType[mBlockNum])
{
  TvFunc::WordType mask = vec_mask(ni);
  for ( int b: Range(mBlockNum) ) {
    mVector[b] = mask;
  }
}

// リテラル関数を作るコンストラクタ
TvFunc::TvFunc(int ni,
	       VarId varid,
	       bool inv) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new TvFunc::WordType[mBlockNum])
{
  int idx = varid.val();
  ASSERT_COND( idx < ni );

  for ( int b: Range(mBlockNum) ) {
    TvFunc::WordType pat = lit_pat(idx, b);
    if ( inv ) {
      pat = ~pat;
    }
    mVector[b] = pat;
  }
  mVector[0] &= vec_mask(ni);
}

// 入力数と真理値を指定したコンストラクタ
TvFunc::TvFunc(int ni,
	       const vector<int>& values) :
  mInputNum(ni),
  mBlockNum(nblock(ni)),
  mVector(new TvFunc::WordType[mBlockNum])
{
  int ni_pow = 1 << ni;
  ASSERT_COND( values.size() == ni_pow );
  int base = 0;
  TvFunc::WordType bitpat = 0ULL;
  TvFunc::WordType bitmask = 1ULL;
  for ( int p: Range(ni_pow) ) {
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

// コピーコンストラクタ
TvFunc::TvFunc(const TvFunc& src) :
  mInputNum(src.mInputNum),
  mBlockNum(src.mBlockNum),
  mVector(new TvFunc::WordType[mBlockNum])
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] = src.mVector[b];
  }
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元のソースオブジェクト
TvFunc::TvFunc(TvFunc&& src) :
  mInputNum(src.mInputNum),
  mBlockNum(src.mBlockNum),
  mVector(src.mVector)
{
  src.mInputNum = 0;
  src.mBlockNum = 0;
  src.mVector = nullptr;
}

// コピー代入演算子
TvFunc&
TvFunc::operator=(const TvFunc& src)
{
  if ( mBlockNum != src.mBlockNum ) {
    delete [] mVector;
    mBlockNum = src.mBlockNum;
    mVector = new TvFunc::WordType[mBlockNum];
  }
  mInputNum = src.mInputNum;

  for ( int b: Range(mBlockNum) ) {
    mVector[b] = src.mVector[b];
  }

  return *this;
}

// @brief ムーブ代入演算子
// @param[in] src コピー元のソースオブジェクト
// @return 自分自身への参照を返す．
TvFunc&
TvFunc::operator=(TvFunc&& src)
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
  for ( int b: Range(mBlockNum) ) {
    mVector[b] ^= neg_mask;
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
TvFunc&
TvFunc::and_int(const TvFunc& src1)
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] &= src1.mVector[b];
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
TvFunc&
TvFunc::or_int(const TvFunc& src1)
{
  for ( int b: Range(mBlockNum) ) {
    mVector[b] |= src1.mVector[b];
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
TvFunc&
TvFunc::xor_int(const TvFunc& src1)
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
TvFunc&
TvFunc::cofactor_int(VarId varid,
		     bool inv)
{
  int pos = varid.val();
  if ( pos < NIPW ) {
    TvFunc::WordType mask = c_masks[pos];
    if ( inv ) {
      mask = ~mask;
    }
    int shift = 1 << pos;
    for ( int b: Range(mBlockNum) ) {
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
    int bit = 1U << pos;
    for ( int b: Range(mBlockNum) ) {
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

// 変数がサポートの時 true を返す．
bool
TvFunc::check_sup(VarId var) const
{
  int i = var.val();
  if ( i < NIPW ) {
    // ブロックごとにチェック
    int dist = 1 << i;
    TvFunc::WordType mask = c_masks[i];
    for ( int b: Range(mBlockNum) ) {
      TvFunc::WordType word = mVector[b];
      if ( (word ^ (word << dist)) & mask ) {
	return true;
      }
    }
  }
  else {
    // ブロック単位でチェック
    int i5 = i - NIPW;
    int check = 1 << i5;
    for ( int b: Range(mBlockNum) ) {
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
  int i = var1.val();
  int j = var2.val();

  // i と j を正規化する．
  if ( i < j ) {
    int tmp = i;
    i = j;
    j = tmp;
  }
  // ここ以降では必ず i > j が成り立つ．

  bool ans = true;
  if ( j >= NIPW ) {
    // i >= NIPW (実際には i > NIPW)
    // j >= NIPW
    // ブロック単位で比較する．
    int mask_i = (1 << (i - NIPW));
    int mask_j = (1 << (j - NIPW));
    int mask_all = mask_i | mask_j;
    int cond;
    if ( inv ) {
      cond = 0UL;
    }
    else {
      cond = mask_j;
    }
    for ( int b: Range(mBlockNum) ) {
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
    int mask_i = (1 << (i - NIPW));
    int cond;
    if ( inv ) {
      cond = 0UL;
    }
    else {
      cond = mask_i;
    }
    TvFunc::WordType mask2 = ~c_masks[j];
    int s = 1 << j;
    for ( int b: Range(mBlockNum) ) {
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
      int s = (1 << i) + (1 << j);
      for ( int b: Range(mBlockNum) ) {
	TvFunc::WordType word = mVector[b];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
    else {
      TvFunc::WordType mask = sym_masks2[(i * (i - 1)) / 2 + j];
      int s = (1 << i) - (1 << j);
      for ( int b: Range(mBlockNum) ) {
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
TvFunc::xform(const NpnMap& npnmap) const
{
#if defined(DEBUG)
  cout << "xform" << endl
       << *this << endl
       << npnmap << endl;
#endif

  int new_ni = npnmap.input_num2();
  int imask = 0;
  int ipat[kMaxNi];
  for ( int i: Range(new_ni) ) {
    ipat[i] = 0;
  }
  for ( int i: Range(mInputNum) ) {
    VarId src_var(i);
    NpnVmap imap = npnmap.imap(src_var);
    if ( imap.is_invalid() ) {
      continue;
    }
    if ( imap.inv() ) {
      imask |= (1 << i);
    }
    VarId dst_var = imap.var();
    int j = dst_var.val();
    ipat[j] = 1 << i;
  }
  TvFunc::WordType omask = npnmap.oinv() ? 1ULL : 0ULL;

  TvFunc ans(new_ni);
  int ni_pow = 1 << new_ni;
  for ( int b: Range(ni_pow) ) {
    int orig_b = 0;
    int tmp = b;
    for ( int i: Range(new_ni) ) {
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
  ymuint varmap = 0U;
  int dst_ni = 0;
  for ( int i: Range(mInputNum) ) {
    if ( !check_sup(VarId(i)) ) {
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
  int j = 0;
  int rmap[kMaxNi];
  for ( int i: Range(mInputNum) ) {
    if ( (varmap & (1U << i)) == 0U ) {
      ans.set(VarId(i), VarId(j), false);
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
// @param[out] map_list 変換を格納するリスト
void
TvFunc::npn_cannonical_all_map(vector<NpnMap>& map_list) const
{
  nsLogic::NpnMgr npn_mgr;
  npn_mgr.cannonical(*this);
  npn_mgr.all_cmap(map_list);
}

// ハッシュ値を返す．
SizeType
TvFunc::hash() const
{
  SizeType ans = 0;
  for ( int b: Range(mBlockNum) ) {
    TvFunc::WordType tmp = mVector[b];
    TvFunc::WordType tmp_l = (tmp >>  0) & 0xFFFFFFFFULL;
    TvFunc::WordType tmp_h = (tmp >> 32) & 0xFFFFFFFFULL;
    ans ^= tmp_l ^ tmp_h;
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
  int n = func1.mBlockNum;
  for ( int b: Range(n) ) {
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
operator&&(const TvFunc& func1,
	   const TvFunc& func2)
{
  if ( func1.mInputNum != func2.mInputNum ) {
    return false;
  }

  int n = func1.mBlockNum;
  for ( int b: Range(n) ) {
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
TvFunc::print(ostream& s,
	      int mode) const
{
  int ni_pow = 1 << mInputNum;
  const int wordsize = sizeof(TvFunc::WordType) * 8;
  if ( mode == 2 ) {
    TvFunc::WordType* bp = mVector;
    int offset = 0;
    TvFunc::WordType tmp = *bp;
    for ( int p: Range(ni_pow) ) {
      s << (tmp & 1LL);
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
    int ni_pow4 = ni_pow / 4;
    TvFunc::WordType* bp = mVector;
    int offset = 0;
    TvFunc::WordType tmp = *bp;
    for ( int p: Range(ni_pow4) ) {
      TvFunc::WordType tmp1 = (tmp & 0xF);
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
TvFunc::dump(ostream& s) const
{
  s.write(reinterpret_cast<const char*>(&mInputNum), sizeof(mInputNum));
  s.write(reinterpret_cast<const char*>(mVector), sizeof(WordType) * mBlockNum);
}

// @brief バイナリファイルの読み込み
// @param[in] s 入力元のストリーム
void
TvFunc::restore(istream& s)
{
  s.read(reinterpret_cast<char*>(&mInputNum), sizeof(mInputNum));
  int nblk = nblock(mInputNum);
  if ( mBlockNum != nblk ) {
    delete [] mVector;
    mBlockNum = nblk;
    mVector = new TvFunc::WordType[mBlockNum];
  }
  s.read(reinterpret_cast<char*>(mVector), sizeof(WordType) * mBlockNum);
}

// @brief コファクターマスクを得る．
// @param[in] pos 入力番号 ( 0 <= pos <= 5 )
TvFunc::WordType
TvFunc::c_mask(int pos)
{
  return c_masks[pos];
}

// @brief リテラル関数のビットパタンを得る
// @param[in] var_id 変数番号
// @param[in] block_id ブロック番号
TvFunc::WordType
TvFunc::lit_pat(int var_id,
		int block_id)
{
  if ( var_id < NIPW ) {
    // block_id に無関係
    return c_masks[var_id];
  }
  else {
    // ちょっとトリッキーなコード
    // block_id の var_id1 ビットめが1の時 0 - 1 = ~0が
    // 0 の 0 が返される．
    int var_id1 = var_id - NIPW;
    return 0ULL - ((block_id >> var_id1) & 1ULL);
  }
}

END_NAMESPACE_YM
