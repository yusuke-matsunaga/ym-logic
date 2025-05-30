﻿
/// @file walsh_01_normalize.cc
/// @brief walsh_01_normalize の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include "InputInfo.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief Walsh の0次/1次係数を用いた正規化を行う．
bool
walsh_01_normalize(
  const TvFunc& func,
  NpnMap& xmap,
  InputInfo& input_info
)
{
  // Walsh の0次と1次の係数を計算する．
  int w0;
  int w1[TvFunc::kMaxNi];
  w0 = func.walsh_01(w1);

  SizeType ni = func.input_num();

  xmap.clear();
  xmap.resize(ni);

  bool opol_fixed = false;

  // w0 が非負になるように出力極性を決める．
  if ( w0 < 0 ) {
    xmap.set_oinv(true);
    opol_fixed = true;
    w0 = -w0;
    // w1 も反転させる．
    for (int i = 0; i < ni; ++ i) {
      w1[i] = -w1[i];
    }
  }
  else if ( w0 > 0 ) {
    // そのままの極性で固定する．
    xmap.set_oinv(false);
    opol_fixed = true;
  }
  else {
    // とりあえずそのままの極性で固定する．
    xmap.set_oinv(false);
  }

  // w1 に従って入力極性の調整を行う．
  for ( SizeType var = 0; var < ni; ++ var ) {
    if ( w1[var] < 0 ) {
      // 反転させる．
      xmap.set_imap(var, var, true);
      w1[var] = -w1[var];
    }
    else if ( w1[var] > 0 ) {
      // そのままの極性で固定する．
      xmap.set_imap(var, var, false);
    }
    else { // w1[var] == 0
      // 独立な変数かどうか調べる．
      if ( func.check_sup(var) ) {
	// とりあえずそのままの極性で固定する．
	xmap.set_imap(var, var, false);
      }
      else {
	// 独立だった．
	;
      }
    }
  }

  // xmap に従って関数を変換する．
  auto func0 = func.xform(xmap);

  // 等価な入力グループを探す．
  // 副産物として入力の極性が決まる場合がある．
  input_info.clear();
  input_info.set_input_num(ni);
  for ( SizeType var = 0; var < ni; ++ var ) {
    bool found = false;
    for ( SizeType gid = 0; gid < input_info.group_num(); ++ gid ) {
      if ( w1[var] != input_info.w1(gid) ) {
	// w1 の値が異なる．
	continue;
      }

      // 対称性のチェックを行う．
      SizeType var1 = input_info.elem(gid, 0);
      bool stat1 = func0.check_sym(var, var1, false);
      if ( stat1 ) {
	// 対称だった．
	found = true;
	if ( w1[var1] == 0 && input_info.elem_num(gid) == 1 ) {
	  // 係数が0で最初の等価対の場合には bi-simmetry のチェックを行う．
	  bool stat2 = func0.check_sym(var, var1, true);
	  if ( stat2 ) {
	    // bi-symmetry の印を付けておく．
	    input_info.set_bisym(gid);
	  }
	}
	// 要素を追加しておく．
	input_info.add_elem(gid, var);
	break;
      }
      else if ( w1[var1] == 0 ) {
	// 逆極性で対称の場合もあるのでチェックしておく．
	bool stat3 = func0.check_sym(var, var1, true);
	if ( stat3 ) {
	  // 逆相で対称だった．
	  found = true;
	  input_info.add_elem(gid, var);
	  xmap.set_imap(var, var, true);
	  break;
	}
      }
    }
    if ( !found ) {
      // 新しい等価グループを作る．
      input_info.new_group(var, w1[var]);
    }
  }

  return opol_fixed;
}

END_NAMESPACE_YM_LOGIC
