﻿
/// @file NpnRawSig.cc
/// @brief NpnRawSig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/TvFunc.h"

#include "NpnRawSig.h"
#include "NpnConf.h"


const int debug_normalize = 0x0001;
const int debug_none      = 0x0000;
const int debug_all       = 0xffff;


#ifndef DEBUG_FLAG
#define DEBUG_FLAG debug_none
#endif


BEGIN_NAMESPACE_YM_NPN

int debug = DEBUG_FLAG;

//////////////////////////////////////////////////////////////////////
// NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param[in] func 対象の関数
NpnRawSig::NpnRawSig(const TvFunc& func) :
  mFunc(func)
{
  mInputNum = mFunc.input_num();

  // Walsh の0次と1次の係数を計算する．
  // 2次の係数はオンデマンドで計算する．
  mW0 = mFunc.walsh_01(mW1);
  for (ymuint i = 0; i < mInputNum; ++ i) {
    for (ymuint j = 0; j < mInputNum; ++ j) {
      mW2flag[i * mInputNum + j] = 0;
    }
  }
}

// デストラクタ
NpnRawSig::~NpnRawSig()
{
}

// @brief W0/W1 を用いて正規化する．
void
NpnRawSig::normalize(NpnConf& conf)
{
  if ( debug & debug_normalize ) {
    cout << "Before normalize" << endl;
    cout << mFunc << endl;
    dump_walsh(cout);
  }

  // mW0 が非負になるように出力極性の調整を行う．
  mOpol = 1;
  if ( walsh_0() < 0 ) {
    mOpol = -1;
    invert_all();
  }
  else if ( walsh_0() == 0 ) {
    mOpol = 0;
  }

  // Walsh の1次係数を計算し，極性の調整を行う．
  // 同時に等価入力クラスをつくる．
  mNc = 0;
  mIndepNum = 0;
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mIcNum[i] = 2;
    mIcLink[i] = static_cast<ymuint>(-1);

    VarId var(i);

    // i の1次係数を求める．
    int w1 = walsh_1(i);

    // w1 が非負になるように調整する．
    // w1 が 0 の時には実際のサポートかどうかも調べる．
    int ipol = 1;
    if ( w1 < 0 ) {
      w1 = -w1;
      ipol = -1;
      invert_input(i);
    }
    else if ( w1 == 0 ) {
      ipol = 0;
      bool stat = mFunc.check_sup(var);
      if ( !stat ) {
	// この入力はサポートではなかった
	add_indep(i);
	// 意味はないけど極性は決まったものとみなす．
	mIpols[i] = 1;
	continue;
      }
    }
    mIpols[i] = ipol;

    // 等価な入力があるかどうか調べる．
    bool found = false;
    for (ymuint p = 0; p < mNc; ++ p) {
      ymuint pos1 = mIcRep[p];
      if ( w1 != walsh_1(pos1) ) {
	continue;
      }
      VarId var1(pos1);
      // 1次係数が等しい場合
      // 対称性のチェックを行う．
      bool inv = (mIpols[pos1] * ipol == -1);
      bool stat = mFunc.check_sym(var, var1, inv);
      if ( stat ) {
	// 対称だった
	found = true;
	if ( w1 == 0 && ic_num(pos1) == 1 ) {
	  // bi-symmetry かどうかチェックする
	  bool stat = mFunc.check_sym(var, var1, !inv);
	  if ( stat ) {
	    set_bisym(pos1);
	  }
	}
	add_elem(pos1, i);
	break;
      }
      if ( w1 == 0 ) {
	// w1 == 0 の時には逆相での対称性もチェックする．
	// この場合，最初の要素の極性は常に false のはず
	bool stat = mFunc.check_sym(var, var1, true);
	if ( stat ) {
	  // 逆相で対称だった．
	  found = true;
	  add_elem(pos1, i);
	  mIpols[i] = -1;
	  invert_input(i);
	  break;
	}
      }
    }
    if ( !found ) {
      // 対称な入力が見つからなかった時には新たな入力クラスをつくる．
      mIcRep[mNc] = i;
      ++ mNc;
    }
  }

  if ( mOpol == 0 ) {
    // もしも入力の極性がすべて決まっていれば
    // w2 の最大値と最小値の絶対値の大きい方の出力極性を選ぶ．
    // 等しいときには総和を正にする．
    int min = walsh_2(0, 0);
    int max = min;
    int sum = 0;
    bool valid = true;
    for (ymuint i = 0; i < mInputNum; ++ i) {
      if ( walsh_1(i) == 0 ) {
	valid = false;
	break;
      }
      for (ymuint j = 0; j < mInputNum; ++ j) {
	int w2 = walsh_2(i, j);
	if ( min > w2 ) {
	  min = w2;
	}
	if ( max < w2 ) {
	  max = w2;
	}
	sum += w2;
      }
    }
    if ( valid ) {
      if ( -min > max ) {
	mOpol = -1;
	// w1 を正にするには入力をすべて反転する必要がある．
	for (ymuint i = 0; i < mInputNum; ++ i) {
	  if ( walsh_1(i) != 0 ) {
	    mIpols[i] *= -1;
	  }
	}
	// と同時に w2 も反転させる．
	invert_all_w2();
      }
      else if ( -min < max ) {
	mOpol = 1;
      }
      else { // -min == max
	if ( sum < 0 ) {
	  mOpol = -1;
	  // w1 を正にするには入力をすべて反転する必要がある．
	  for (ymuint i = 0; i < mInputNum; ++ i) {
	    if ( walsh_1(i) != 0 ) {
	      mIpols[i] *= -1;
	    }
	  }
	  // と同時に w2 も反転させる．
	  invert_all_w2();
	}
	else if ( sum > 0 ) {
	  mOpol = 1;
	}
      }
    }
  }

  if ( debug & debug_normalize ) {
    cout << "After normalize" << endl;
    dump_walsh(cout);
    dump_pols(cout);
  }

#if 0
  if ( mNc > 0 ) {
    for (ymuint i = 0; i < mNc; ++ i) {
      ymuint pos = mIcRep[i];
      int ipol1 = (walsh_1(pos) == 0) ? 0 : 1;
      conf.add_ic_rep(pos, ipol1);
    }
    // 最初はひとかたまりのグループにしておく．
    conf.add_ig(0);
  }
  if ( mOpol != 0 ) {
    conf.set_opol(1);
  }
  conf.set_sig(this);
#endif

}

// @brief すべてのシグネチャを反転させる．
void
NpnRawSig::invert_all()
{
  mW0 *= -1;
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mW1[i] *= -1;
  }
  invert_all_w2();
}

// @brief 一つの入力に関するシグネチャを反転する．
void
NpnRawSig::invert_input(ymuint pos)
{
  mW1[pos] *= -1;
  for (ymuint i = 0; i < mInputNum; ++ i) {
    invert_w2(pos, i);
  }
}

// @brief 2次の係数のみを反転する．
void
NpnRawSig::invert_all_w2()
{
  for (ymuint i = 0; i < mInputNum; ++ i) {
    for (ymuint j = i + 1; j < mInputNum; ++ j) {
      invert_w2(i, j);
    }
  }
}

// @brief w2 の要素を反転する．
void
NpnRawSig::invert_w2(ymuint pos1,
		     ymuint pos2)
{
  if ( pos2 > pos1 ) {
    int tmp = pos1;
    pos1 = pos2;
    pos2 = tmp;
  }
  ymuint base = pos1 * input_num() + pos2;
  if ( mW2flag[base] & 1 ) {
    mW2[base] *= -1;
  }
  else {
    mW2flag[base] ^= 2;
  }
}

// @brief 重み別 Walsh の 0次係数を得る．
int
NpnRawSig::walsh_w0(ymuint w,
		    bool opol,
		    bool ipol[]) const
{
  if ( mOpol == -1 ) {
    opol = !opol;
  }
  ymuint32 ibits = 0UL;
  for (ymuint i = 0; i < input_num(); ++ i) {
    bool ip = ipol[i];
    if ( mIpols[i] == -1 ) {
      ip = !ip;
    }
    if ( ip ) {
      ibits |= (1UL << i);
    }
  }
  return mFunc.walsh_w0(w, opol, ibits);
}

// @brief Walsh 係数を出力する．
void
NpnRawSig::dump_walsh(ostream& s) const
{
  s << "W0: " << walsh_0() << endl
    << "w1:";
  for (ymuint i = 0; i < input_num(); ++ i) {
    s << " " << walsh_1(i);
  }
  s << endl;
  s << "W2:" << endl;
  for (ymuint i = 0; i < input_num(); ++ i) {
    s << "   ";
    for (ymuint j = 0; j < input_num(); ++ j) {
      s << " " << setw(4) << walsh_2(i, j);
    }
    s << endl;
  }
  s << endl;
}

// @brief 極性情報を出力する．
void
NpnRawSig::dump_pols(ostream& s) const
{
  s << "opol: ";
  if ( mOpol == -1 ) {
    s << "N";
  }
  else if ( mOpol == 1 ) {
    s << "P";
  }
  else {
    s << "-";
  }
  s << endl
    << "ipol:";
  for (ymuint i = 0; i < input_num(); ++ i) {
    s << " ";
    if ( mIpols[i] == -1 ) {
      s << "N";
    }
    else if ( mIpols[i] == 1 ) {
      s << "P";
    }
    else {
      s << "-";
    }
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
