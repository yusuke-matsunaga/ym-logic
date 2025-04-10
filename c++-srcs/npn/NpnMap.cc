
/// @file NpnMap.cc
/// @brief NpnMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2019, 2021, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/NpnMap.h"


//#define DEBUG_NPN_MAP


BEGIN_NAMESPACE_YM_LOGIC

// デバッグ用のフラグ変数
#if defined(DEBUG) || defined(DEBUG_NPN_MAP)
const int debug_npn_map = 1;
#else
const int debug_npn_map = 0;
#endif

// 入力数を指定したコンストラクタ
// 恒等変換になる．
NpnMap::NpnMap(
  SizeType ni
)
{
  set_ni(ni, ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    mImap[i] = NpnVmap(i, false);
  }
}

// @brief 写像前と後の入力数を指定したコンストラクタ
NpnMap::NpnMap(
  SizeType ni,
  SizeType ni2
)
{
  set_ni(ni, ni2);
}

// コピーコンストラクタ
NpnMap::NpnMap(
  const NpnMap& src
) : mNiPol{src.mNiPol}
{
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    mImap[i] = src.mImap[i];
  }
}

// 代入演算子
NpnMap&
NpnMap::operator=(
  const NpnMap& src
)
{
  mNiPol = src.mNiPol;
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    mImap[i] = src.mImap[i];
  }
  return *this;
}

// 内容をクリアする．
// 入力の変換内容は kImapBad になる．
// 出力の極性は kPolPosi
void
NpnMap::clear()
{
  mNiPol &= ~(1UL);
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    mImap[i] = NpnVmap::invalid();
  }
}

// 入力数を再設定して内容をクリアする．
void
NpnMap::resize(
  SizeType new_ni
)
{
  set_ni(new_ni, new_ni);
  for ( SizeType i = 0; i < new_ni; ++ i ) {
    mImap[i] = NpnVmap::invalid();
  }
}

// @brief 入力数を再設定する．
void
NpnMap::resize(
  SizeType ni,
  SizeType ni2
)
{
  set_ni(ni, ni2);
  for ( SizeType i = 0; i < ni; ++ i ) {
    mImap[i] = NpnVmap::invalid();
  }
}

// 恒等変換を表すように設定する．
void
NpnMap::set_identity(
  SizeType new_ni,
  bool oinv
)
{
  set_ni(new_ni, new_ni);
  for ( SizeType i = 0; i < new_ni; ++ i ) {
    mImap[i] = NpnVmap(i, false);
  }
  set_oinv(oinv);
}

// @brief 入力の変換内容の設定
void
NpnMap::set_imap(
  SizeType var,
  NpnVmap imap
)
{
  if ( var < input_num() ) {
    mImap[var] = imap;
  }
}

// 出力極性を設定する．
void
NpnMap::set_oinv(
  bool inv
)
{
  if ( inv ) {
    mNiPol |= 1UL;
  }
  else {
    mNiPol &= ~(1UL);
  }
}

// 内容が等しいか調べる．
bool
NpnMap::operator==(
  const NpnMap& src
) const
{
  if ( mNiPol != src.mNiPol ) {
    return false;
  }
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    if ( mImap[i] != src.mImap[i] ) {
      return false;
    }
  }
  return true;
}

// 逆写像を求める．1対1写像でなければ答えは不定
NpnMap
NpnMap::inverse() const
{
  if ( debug_npn_map ) {
    cerr << "inverse :" << endl
	 << *this
	 << endl;
  }

  SizeType src_ni = input_num();
  SizeType dst_ni = input_num2();
  NpnMap dst_map(dst_ni, src_ni);
  for ( SizeType src_var = 0; src_var < src_ni; ++ src_var ) {
    NpnVmap imap1 = imap(src_var);
    if ( !imap1.is_invalid() ) {
      auto dst_var = imap1.var();
      if ( dst_var >= dst_ni ) {
	if ( debug_npn_map ) {
	  cerr << "inverse(src): srcの値域と定義域が一致しません．";
	}
	return NpnMap{};
      }
      bool inv = imap1.inv();
      dst_map.set_imap(dst_var, src_var, inv);
    }
  }
  dst_map.set_oinv(oinv());

  if ( debug_npn_map ) {
    cerr << "--->" << endl
	 << dst_map
	 << endl << endl;
  }

  return dst_map;
}

// 合成を求める．src1の値域とsrc2の定義域は一致していなければならない．
NpnMap
NpnMap::operator*(
  const NpnMap& src2
) const
{
  if ( debug_npn_map ) {
    cerr << "compose :"	 << endl
	 << *this	 << endl
	 << "with"	 << endl
	 << src2	 << endl;
  }

  SizeType ni1 = input_num();
  SizeType ni1_2 = input_num2();
  SizeType ni2 = src2.input_num();
  SizeType ni2_2 = src2.input_num2();
  if ( ni1_2 != ni2 ) {
    if ( debug_npn_map ) {
      cerr << "src1 * src2: src1の値域とsrc2の定義域が一致しません．";
    }
    return NpnMap();
  }

  NpnMap dst_map(ni1, ni2_2);
  dst_map.set_oinv(oinv() ^ src2.oinv());
  for ( SizeType var1 = 0; var1 < ni1; ++ var1 ) {
    auto imap1 = imap(var1);
    if ( imap1.is_invalid() ) {
      dst_map.set_imap(var1, NpnVmap::invalid());
    }
    else {
      auto var2 = imap1.var();
      bool inv2 = imap1.inv();
      auto imap2 = src2.imap(var2);
      if ( imap2.is_invalid() ) {
	if ( debug_npn_map ) {
	  cerr << "src1 * src2: src1の値域とsrc2の定義域が一致しません．";
	}
	return NpnMap();
      }
      else {
	auto var3 = imap2.var();
	bool inv3 = imap2.inv();
	dst_map.set_imap(var1, var3, inv2 ^ inv3);
      }
    }
  }

  if ( debug_npn_map ) {
    cerr << "--->" << endl
	 << dst_map << endl;
  }

  return dst_map;
}

// @brief 合成を求める．
NpnMap&
NpnMap::operator*=(
  const NpnMap& src2
)
{
  *this = *this * src2;
  return *this;
}

// ストリーム出力演算子
ostream&
operator<<(
  ostream& s,
  const NpnMap& map
)
{
  const char* comma = "";
  s << "MAP: " << map.input_num() << " -> " << map.input_num2() << ", "
    << "INPUT(";
  for ( SizeType i = 0; i < map.input_num(); ++ i ) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    NpnVmap imap = map.imap(i);
    if ( imap.is_invalid() ) {
      s << "---";
    }
    else {
      auto dst_var = imap.var();
      bool inv = imap.inv();
      if ( inv ) {
	s << '~';
      }
      s << dst_var;
    }
  }
  s << ")";
  s << " OUTPUT(0 ==> ";
  if ( map.oinv() ) {
    s << '~';
  }
  s << "0)";

  return s;
}

// バイナリ出力
void
NpnMap::dump(
  ostream& bos
) const
{
  SizeType ni = input_num();
  SizeType ni2 = input_num2();
  bos << ni << ni2;
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto vmap = imap(i);
    vmap.dump(bos);
  }
  bos << oinv();
}

// バイナリ入力
void
NpnMap::restore(
  istream& bis
)
{
  SizeType ni;
  SizeType ni2;
  bis >> ni >> ni2;
  resize(ni, ni2);
  for ( SizeType i = 0; i < ni; ++ i ) {
    NpnVmap vmap;
    vmap.restore(bis);
    set_imap(i, vmap);
  }
  bool inv;
  bis >> inv;
  set_oinv(inv);
}

END_NAMESPACE_YM_LOGIC
