﻿
/// @file walsh_w1_refine.cc
/// @brief walsh_w1_refine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "IgPartition.h"
#include "PolConf.h"


BEGIN_NONAMESPACE

#ifdef YM_DEBUG
#define DEBUG_FLAG true
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG false
#endif

int debug = DEBUG_FLAG;
//int debug = true;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_LOGIC

// @brief 重み別 w1 を用いて極性を確定させる．
void
walsh_w1_refine(
  const TvFunc& func,
  SizeType var,
  vector<PolConf>& polconf_list
)
{
  SizeType ni = func.input_num();

  if ( debug ) {
    cout << "before walsh_w1_refine" << endl;
    print_polconf_list(cout, polconf_list, ni);
    cout << endl;
  }

  // 重み別 w1 係数を用いて極性の決定を行う．
  for ( SizeType w = 0; w <= ni; ++ w ) {
    bool first = true;
    int max_d0 = 0;
    SizeType wpos = 0;
    for ( SizeType i = 0; i < polconf_list.size(); ++ i ) {
      PolConf polconf = polconf_list[i];
      int d0 = func.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());
      if ( debug ) {
	print_polconf(cout, polconf, ni);
	cout << " walsh_w1(" << var << ", " << w << ") = " << d0 << endl;
      }
      int stat = first ? -1 : max_d0 - d0;
      first = false;
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  max_d0 = d0;
	}
	polconf_list[wpos] = polconf;
	++ wpos;
      }
    }
    if ( wpos < polconf_list.size() ) {
      polconf_list.erase(polconf_list.begin() + wpos, polconf_list.end());
    }
  }

  if ( debug ) {
    cout << "after walsh_w1_refine" << endl;
    print_polconf_list(cout, polconf_list, ni);
    cout << endl;
  }
}

BEGIN_NONAMESPACE

class W1Cmp
{
public:

  W1Cmp(
    SizeType n
  ): mW1Array(n)
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief w1 を設定する．
  void
  set_w1(
    SizeType gid,
    int w1
  )
  {
    mW1Array[gid] = w1;
  }

  /// @brief 等価比較演算子
  bool
  eq(
    SizeType gid1,
    SizeType gid2
  ) const
  {
    return mW1Array[gid1] == mW1Array[gid2];
  }

  /// @brief 大小比較演算子
  bool
  gt(
    SizeType gid1,
    SizeType gid2
  ) const
  {
    return mW1Array[gid1] > mW1Array[gid2];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<int> mW1Array;

};

END_NONAMESPACE

// @brief 重み別 w1 を用いて変数順序を確定させる．
// @param[in] var_list 対象の変数リスト
void
walsh_w1_refine(
  const TvFunc& func,
  IgPartition& igpart
)
{
  SizeType ni = func.input_num();

  if ( debug ) {
    cout << "before walsh_w1_refine" << endl;
    cout << igpart << endl;
  }

  // 重み別 w1 係数を用いて極性の決定を行う．
  for ( SizeType w = 0; w <= ni; ++ w ) {
    for ( SizeType pid = 0; pid < igpart.partition_num(); ++ pid ) {
      if ( igpart.is_resolved(pid) ) {
	continue;
      }
      W1Cmp w1_cmp(igpart.group_num());
      for ( SizeType pos = igpart.partition_begin(pid);
	    pos < igpart.partition_end(pid); ++ pos ) {
	auto gid = igpart.group_id(pos);
	auto iid = igpart.input_id(gid);
	int ww1 = func.walsh_w1(iid, w, false, 0U);
	w1_cmp.set_w1(gid, ww1);
      }
      auto delta = igpart.refine(pid, w1_cmp);
      pid += delta;
    }
  }

  // 確定した分割を前に持ってくる．
  igpart.reorder();

  if ( debug ) {
    cout << "after walsh_w1_refine" << endl
	 << igpart << endl;
  }
}

END_NAMESPACE_YM_LOGIC
