
/// @file NpnMap.cc
/// @brief NpnMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
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

// 空のコンストラクタ
NpnMap::NpnMap() :
  mNiPol(0UL)
{
}

// 入力数を指定したコンストラクタ
// 恒等変換になる．
NpnMap::NpnMap(ymuint ni) :
  mNiPol(0)
{
  set_ni(ni, ni);
  for (ymuint i = 0; i < ni; ++ i) {
    mImap[i] = NpnVmap(VarId(i), false);
  }
}

// @brief 写像前と後の入力数を指定したコンストラクタ
// @param[in] ni 写像前の入力数
// @param[in] ni2 写像後の入力数
//
// 内容は不定
NpnMap::NpnMap(ymuint ni,
	       ymuint ni2) :
  mNiPol(0)
{
  set_ni(ni, ni2);
}

// コピーコンストラクタ
NpnMap::NpnMap(const NpnMap& src) :
  mNiPol(src.mNiPol)
{
  for (ymuint i = 0; i < input_num(); ++ i) {
    mImap[i] = src.mImap[i];
  }
}

// 代入演算子
const NpnMap&
NpnMap::operator=(const NpnMap& src)
{
  mNiPol = src.mNiPol;
  for (ymuint i = 0; i < input_num(); ++ i) {
    mImap[i] = src.mImap[i];
  }
  return *this;
}

// デストラクタ
NpnMap::~NpnMap()
{
}

// 内容をクリアする．
// 入力の変換内容は kImapBad になる．
// 出力の極性は kPolPosi
void
NpnMap::clear()
{
  mNiPol &= ~(1UL);
  for (ymuint i = 0; i < input_num(); ++ i) {
    mImap[i] = NpnVmap::invalid();
  }
}

// 入力数を再設定して内容をクリアする．
void
NpnMap::resize(ymuint new_ni)
{
  set_ni(new_ni, new_ni);
  for (ymuint i = 0; i < new_ni; ++ i) {
    mImap[i] = NpnVmap::invalid();
  }
}

// @brief 入力数を再設定する．
// @param[in] ni 写像前の入力数
// @param[in] ni2 写像後の入力数
//
// 以前の内容はクリアされる．
void
NpnMap::resize(ymuint ni,
	       ymuint ni2)
{
  set_ni(ni, ni2);
  for (ymuint i = 0; i < ni; ++ i) {
    mImap[i] = NpnVmap::invalid();
  }
}

// 恒等変換を表すように設定する．
void
NpnMap::set_identity(ymuint new_ni)
{
  set_ni(new_ni, new_ni);
  for (ymuint i = 0; i < new_ni; ++ i) {
    mImap[i] = NpnVmap(VarId(i), false);
  }
}

// @brief 入力の変換内容の設定
// @param[in] src_var 入力変数
// @param[in] dst_var 変換先の入力変数
// @param[in] pol 極性
void
NpnMap::set(VarId var,
	    NpnVmap imap)
{
  ymuint src_pos = var.val();
  if ( src_pos < input_num() ) {
    mImap[src_pos] = imap;
  }
}

// 出力極性を設定する．
void
NpnMap::set_oinv(bool inv)
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
NpnMap::operator==(const NpnMap& src) const
{
  if ( mNiPol != src.mNiPol ) {
    return false;
  }
  for (ymuint i = 0; i < input_num(); ++ i) {
    if ( mImap[i] != src.mImap[i] ) {
      return false;
    }
  }
  return true;
}

// 逆写像を求める．1対1写像でなければ答えは不定
NpnMap
inverse(const NpnMap& src)
{
  if ( debug_npn_map ) {
    cerr << "inverse :" << endl
	 << src
	 << endl;
  }

  ymuint src_ni = src.input_num();
  ymuint dst_ni = src.input_num2();
  NpnMap dst_map(dst_ni, src_ni);
  for (ymuint i = 0; i < src_ni; ++ i) {
    VarId src_var(i);
    NpnVmap imap = src.imap(src_var);
    if ( !imap.is_invalid() ) {
      VarId dst_var = imap.var();
      if ( dst_var.val() >= dst_ni ) {
	if ( debug_npn_map ) {
	  cerr << "inverse(src): srcの値域と定義域が一致しません．";
	}
	return NpnMap(src_ni);
      }
      bool inv = imap.inv();
      dst_map.set(dst_var, src_var, inv);
    }
  }
  dst_map.set_oinv(src.oinv());

  if ( debug_npn_map ) {
    cerr << "--->" << endl
	 << dst_map
	 << endl << endl;
  }

  return dst_map;
}

// 合成を求める．src1の値域とsrc2の定義域は一致していなければならない．
NpnMap
operator*(const NpnMap& src1,
	  const NpnMap& src2)
{
  if ( debug_npn_map ) {
    cerr << "compose :"
	 << endl
	 << src1
	 << endl
	 << "with"
	 << endl
	 << src2
	 << endl;
  }

  ymuint ni1 = src1.input_num();
  ymuint ni1_2 = src1.input_num2();
  ymuint ni2 = src2.input_num();
  ymuint ni2_2 = src2.input_num2();
  if ( ni1_2 != ni2 ) {
    if ( debug_npn_map ) {
      cerr << "src1 * src2: src1の値域とsrc2の定義域が一致しません．";
    }
    return NpnMap();
  }

  NpnMap dst_map(ni1, ni2_2);
  dst_map.set_oinv(src1.oinv() ^ src2.oinv());
  for (ymuint i1 = 0; i1 < ni1; ++ i1) {
    VarId var1(i1);
    NpnVmap imap1 = src1.imap(var1);
    if ( imap1.is_invalid() ) {
      dst_map.set(var1, NpnVmap::invalid());
    }
    else {
      VarId var2 = imap1.var();
      bool inv2 = imap1.inv();
      NpnVmap imap2 = src2.imap(var2);
      if ( imap2.is_invalid() ) {
	if ( debug_npn_map ) {
	  cerr << "src1 * src2: src1の値域とsrc2の定義域が一致しません．";
	}
	return NpnMap();
      }
      else {
	VarId var3 = imap2.var();
	bool inv3 = imap2.inv();
	dst_map.set(var1, var3, inv2 ^ inv3);
      }
    }
  }

  if ( debug_npn_map ) {
    cerr << "--->" << endl
	 << dst_map
	 << endl;
  }

  return dst_map;
}

// ストリーム出力演算子
ostream&
operator<<(ostream& s,
	   const NpnMap& map)
{
  const char* comma = "";
  s << "MAP: " << map.input_num() << " -> " << map.input_num2() << ", "
    << "INPUT(";
  for (ymuint i = 0; i < map.input_num(); ++ i) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    VarId var(i);
    NpnVmap imap = map.imap(var);
    if ( imap.is_invalid() ) {
      s << "---";
    }
    else {
      VarId dst_var = imap.var();
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
ODO&
operator<<(ODO& bos,
	   const NpnMap& map)
{
  ymuint32 ni = map.input_num();
  ymuint32 ni2 = map.input_num2();
  bos << ni << ni2;
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap vmap = map.imap(VarId(i));
    bos << vmap;
  }
  bos << map.oinv();

  return bos;
}

// バイナリ入力
IDO&
operator>>(IDO& bis,
	   NpnMap& map)
{
  ymuint32 ni;
  ymuint32 ni2;
  bis >> ni >> ni2;
  map.resize(ni, ni2);
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap vmap;
    bis >> vmap;
    map.set(VarId(i), vmap);
  }
  bool inv;
  bis >> inv;
  map.set_oinv(inv);

  return bis;
}

END_NAMESPACE_YM_LOGIC
