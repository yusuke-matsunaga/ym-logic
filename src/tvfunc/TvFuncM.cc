
/// @file TvFuncM.cc
/// @brief TvFuncM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/TvFuncM.h"
#include "ym/NpnMapM.h"


// 1 ワード当たりの入力数
#define NIPW 6

//#define DEBUG

BEGIN_NONAMESPACE

// コファクターマスク
ymuint64 c_masks[] = {
  0xAAAAAAAAAAAAAAAAULL,
  0xCCCCCCCCCCCCCCCCULL,
  0xF0F0F0F0F0F0F0F0ULL,
  0xFF00FF00FF00FF00ULL,
  0xFFFF0000FFFF0000ULL,
  0xFFFFFFFF00000000ULL
};

// 対称性を調べるためのテーブルその1
// 同位相の時に用いる．
ymuint64 sym_masks2[] = {
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
ymuint64 sym_masks3[] = {
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

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

// 入力数と出力数のみ指定したコンストラクタ
// 中身は恒偽関数
TvFuncM::TvFuncM(int ni,
		 int no) :
  mInputNum(ni),
  mOutputNum(no),
  mBlockNum1(nblock(ni)),
  mBlockNum(mBlockNum1 * no),
  mVector(new ymuint64[mBlockNum])
{
  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] = 0ULL;
  }
}

// @brief TvFunc を用いたコンストラクタ
// @param[in] src_list 各出力の論理関数
// @note src_list の関数の入力数は等しくなければならない．
TvFuncM::TvFuncM(const vector<TvFunc>& src_list)
{
  ASSERT_COND( !src_list.empty() );
  const TvFunc& first = src_list.front();
  int ni = first.input_num();
  int no = src_list.size();
  for ( int i = 1; i < no; ++ i ) {
    ASSERT_COND( src_list[i].input_num() == ni );
  }

  mInputNum = ni;
  mOutputNum = no;
  mBlockNum1 = nblock(ni);
  mBlockNum = mBlockNum1 * no;
  mVector = new ymuint64[mBlockNum];
  for ( int i = 0; i < no; ++ i ) {
    int offset = i * mBlockNum1;
    const TvFunc& src1 = src_list[i];
    for ( int j = 0; j < mBlockNum1; ++ j ) {
      mVector[offset + j] = src1.raw_data(j);
    }
  }
}

// コピーコンストラクタ
TvFuncM::TvFuncM(const TvFuncM& src) :
  mInputNum(src.mInputNum),
  mOutputNum(src.mOutputNum),
  mBlockNum1(src.mBlockNum1),
  mBlockNum(src.mBlockNum),
  mVector(new ymuint64[mBlockNum])
{
  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] = src.mVector[i];
  }
}

// @brief TvFunc からの変換用コンストラクタ
TvFuncM::TvFuncM(const TvFunc& src)
{
  int ni = src.input_num();
  mInputNum = ni;
  mOutputNum = 1;
  mBlockNum1 = nblock(ni);
  mBlockNum = mBlockNum1;
  mVector = new ymuint64[mBlockNum];
  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] = src.raw_data(i);
  }
}

// 代入演算子
const TvFuncM&
TvFuncM::operator=(const TvFuncM& src)
{
  if ( mBlockNum != src.mBlockNum ) {
    delete [] mVector;
    mBlockNum1 = src.mBlockNum1;
    mBlockNum = src.mBlockNum;
    mVector = new ymuint64[mBlockNum];
  }
  mInputNum = src.mInputNum;
  mOutputNum = src.mOutputNum;

  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] = src.mVector[i];
  }

  return *this;
}

// デストラクタ
TvFuncM::~TvFuncM()
{
  delete [] mVector;
}

// 自分自身を否定する．
const TvFuncM&
TvFuncM::negate()
{
  switch ( mInputNum ) {
  case 0:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0x1ULL;
    }
    break;

  case 1:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0x3ULL;
    }
    break;

  case 2:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0xFULL;
    }
    break;

  case 3:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0xFFULL;
    }
    break;

  case 4:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0xFFFFULL;
    }
    break;

  case 5:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0xFFFFFFFFULL;
    }
    break;

  case 6:
    for ( int i = 0; i < mOutputNum; ++ i ) {
      mVector[i] ^= 0xFFFFFFFFFFFFFFFFULL;
    }
    break;

  default:
    {
      ymuint64* endp = mVector + mBlockNum;
      for ( ymuint64* bp = mVector; bp != endp; ++ bp ) {
	*bp ^= ~(0ULL);
      }
    }
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
const TvFuncM&
TvFuncM::operator&=(const TvFuncM& src1)
{
  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] &= src1.mVector[i];
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
const TvFuncM&
TvFuncM::operator|=(const TvFuncM& src1)
{
  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] |= src1.mVector[i];
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
const TvFuncM&
TvFuncM::operator^=(const TvFuncM& src1)
{
  for ( int i = 0; i < mBlockNum; ++ i ) {
    mVector[i] ^= src1.mVector[i];
  }
  return *this;
}

// @brief コファクターを計算し自分に代入する．
// @param[in] varid 変数番号
// @param[in] pol 極性
// @return 自身への参照を返す．
const TvFuncM&
TvFuncM::set_cofactor(VarId varid,
		      bool inv)
{
  int pos = varid.val();
  if ( pos < NIPW ) {
    ymuint64 mask = c_masks[pos];
    if ( inv ) {
      mask = ~mask;
    }
    int shift = 1 << pos;
    for ( int i = 0; i < mBlockNum; ++ i ) {
      ymuint64 pat = mVector[i] & mask;
      if ( inv ) {
	pat |= (pat << shift);
      }
      else {
	pat |= (pat >> shift);
      }
      mVector[i] = pat;
    }
  }
  else {
    pos -= NIPW;
    int bit = 1U << pos;
    for ( int j = 0; j < mOutputNum; ++ j ) {
      ymuint offset = j * mBlockNum1;
      for ( int i = 0; i < mBlockNum1; ++ i ) {
	if ( inv ) {
	  if ( (i & bit) == bit ) {
	    mVector[i + offset] = mVector[(i ^ bit) + offset];
	  }
	}
	else {
	  if ( (i & bit) == 0U ) {
	    mVector[i + offset] = mVector[(i ^ bit) + offset];
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
TvFuncM::output(VarId ovar) const
{
#warning "効率が悪い仮のコード"

  int np = 1U << input_num();
  vector<int> tmp(np);
  for ( int i = 0; i < np; ++ i ) {
    tmp[i] = value(ovar, i);
  }
  return TvFunc(input_num(), tmp);
}

// var がサポートの時 true を返す．
bool
TvFuncM::check_sup(VarId var) const
{
  int i = var.val();
  if ( i < NIPW ) {
    // ブロックごとにチェック
    int dist = 1U << i;
    ymuint64 mask = c_masks[i];
    for ( int i = 0; i < mBlockNum; ++ i ) {
      ymuint64 word = mVector[i];
      if ( (word ^ (word << dist)) & mask ) {
	return true;
      }
    }
  }
  else {
    // ブロック単位でチェック
    int i5 = i - NIPW;
    int check = 1 << i5;
    for ( int i = 0; i < mOutputNum; ++ i ) {
      int offset = i * mBlockNum1;
      for ( int b = 0; b < mBlockNum1; ++ b ) {
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
TvFuncM::check_sym(VarId var1,
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
    int cond = inv ? 0 : mask_j;
    for ( int i = 0; i < mOutputNum; ++ i ) {
      int offset = i * mBlockNum1;
      for ( int v = 0; v < mBlockNum1; ++ v ) {
	if ( (v & mask_all) == cond &&
	     mVector[v + offset] != mVector[(v ^ mask_all) + offset] ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  else if ( i >= NIPW ) {
    // i >= NIPW
    // j < NIPW
    int mask_i = (1U << (i - NIPW));
    int cond = inv ? 0UL : mask_i;
    ymuint64 mask2 = ~c_masks[j];
    int s = 1 << j;
    for ( int i = 0; i < mOutputNum; ++ i ) {
      int offset = i * mBlockNum1;
      for ( int v = 0; v < mBlockNum1; ++ v ) {
	if ( (v & mask_i) == cond &&
	     (mVector[v + offset] ^ (mVector[(v ^ mask_i) + offset] >> s)) & mask2 ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  else {
    // i < NIPW
    // j < NIPW
    if ( inv ) {
      ymuint64 mask = sym_masks3[(i * (i - 1)) / 2 + j];
      int s = (1U << i) + (1U << j);
      for ( int i = 0; i < mBlockNum; ++ i ) {
	ymuint64 word = mVector[i];
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
    else {
      ymuint64 mask = sym_masks2[(i * (i - 1)) / 2 + j];
      int s = (1U << i) - (1U << j);
      for ( int i = 0; i < mBlockNum; ++ i ) {
	ymuint64 word = mVector[i];
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
TvFuncM::xform(const NpnMapM& npnmap) const
{
  ymuint ni_pow = 1UL << mInputNum;

#if defined(DEBUG)
  cout << "xform" << endl
       << *this << endl
       << npnmap << endl;
#endif

  int imask = 0;
  int ipat[kMaxNi];
  for ( int i = 0; i < mInputNum; ++ i ) {
    VarId src_var(i);
    NpnVmap imap = npnmap.imap(src_var);
    if ( imap.inv() ) {
      imask |= (1UL << i);
    }
    VarId dst_var = imap.var();
    int j = dst_var.val();
    ipat[i] = 1 << j;
  }

  TvFuncM ans(mInputNum, mOutputNum);

  for ( int o = 0; o < mOutputNum; ++ o ) {
    VarId src_var(o);
    NpnVmap omap = npnmap.omap(src_var);
    VarId dst_var = omap.var();
    int dst_pos = dst_var.val();
    ymuint64 omask = omap.inv() ? 1ULL : 0ULL;
    for ( int i = 0; i < ni_pow; ++ i ) {
      int new_i = 0;
      int tmp = i;
      for ( int b = 0; b < mInputNum; ++ b, tmp >>= 1 ) {
	if ( tmp & 1 ) {
	  new_i |= ipat[b];
	}
      }
      ymuint64 pat = (value(VarId(o), i ^ imask) ^ omask);
      ans.mVector[block(new_i) + dst_pos * mBlockNum1] |= pat << shift(new_i);
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
  for ( int i = 0; i < mBlockNum; ++ i ) {
    ymuint64 tmp = mVector[i];
    ymuint64 tmp_l = (tmp >> 0) & 0xFFFFFFFFULL;
    ymuint64 tmp_h = (tmp >> 32) & 0xFFFFFFFFULL;
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
compare(const TvFuncM& left,
	const TvFuncM& right)
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

  int n = left.mBlockNum;
  for ( int i = 0; i < n; ++ i ) {
    ymuint64 w1 = left.mVector[i];
    ymuint64 w2 = right.mVector[i];
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
operator&&(const TvFuncM& func1,
	   const TvFuncM& func2)
{
  if ( func1.mInputNum != func2.mInputNum ||
       func1.mOutputNum != func2.mOutputNum ) {
    return false;
  }

  int n = func1.mBlockNum;
  for ( int i = 0; i < n; ++ i ) {
    ymuint64 w1 = func1.mVector[i];
    ymuint64 w2 = func2.mVector[i];
    if ( (w1 & w2) != 0U ) {
      return true;
    }
  }
  return false;
}

// 内容の出力
// mode は 2 か 16
void
TvFuncM::print(ostream& s,
	       int mode) const
{
  int ni_pow = 1UL << mInputNum;
  const int wordsize = sizeof(ymuint64) * 8;
  if ( mode == 2 ) {
    ymuint64* bp = mVector;
    ymuint64 tmp = *bp;
    const char* del = "";
    for ( int j = 0; j < mOutputNum; ++ j ) {
      s << del;
      del = "|";
      int offset = 0;
      for ( int i = 0; i < ni_pow; ++ i ) {
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
    int ni_pow4 = ni_pow / 4;
    ymuint64* bp = mVector;
    ymuint64 tmp = *bp;
    for ( int j = 0; j < mOutputNum; ++ j ) {
      const char* del = "";
      int offset = 0;
      for ( int i = 0; i < ni_pow4; ++ i ) {
	s << del;
	del = "|";
	ymuint64 tmp1 = (tmp & 0xFULL);
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
TvFuncM::dump(ODO& s) const
{
  s << mInputNum
    << mOutputNum
    << mBlockNum1
    << mBlockNum;

  for ( int i = 0; i < mBlockNum; ++ i ) {
    s << mVector[i];
  }
}

// @brief バイナリファイルの読み込み
// @param[in] s 入力元のストリーム
void
TvFuncM::restore(IDO& s)
{
  int nblk;
  s >> mInputNum
    >> mOutputNum
    >> mBlockNum1
    >> nblk;

  if ( mBlockNum != nblk ) {
    delete [] mVector;
    mBlockNum = nblk;
    mVector = new ymuint64[mBlockNum];
  }
  for ( int i = 0; i < mBlockNum; ++ i ) {
    s >> mVector[i];
  }
}

END_NAMESPACE_YM
