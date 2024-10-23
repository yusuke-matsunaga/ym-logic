
/// @file NpnMgr.cc
/// @brief NpnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
/// All rights reserved.

#include "npn/NpnMgr.h"
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
//int debug = debug_all;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_LOGIC

// @brief 重み別 w0 を用いて極性を確定させる．
// @param[in] func 対象の関数
// @param[in] polconf_list 極性割当候補のリスト
void
walsh_w0_refine(
  const TvFunc& func,
  vector<PolConf>& polconf_list
)
{
  SizeType ni = func.input_num();

  if ( debug ) {
    cout << "before walsh_w0_refine()" << endl;
    print_polconf_list(cout, polconf_list, ni);
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  int w = 0;
  for ( w = 0; w <= ni && polconf_list.size() > 1; ++ w ) {
    bool first = true;
    int max_d0 = 0;
    SizeType wpos = 0;
    for ( SizeType i = 0; i < polconf_list.size(); ++ i ) {
      PolConf polconf = polconf_list[i];
      int d0 = func.walsh_w0(w, polconf.oinv(), polconf.iinv_bits());

      int stat = -1;
      if ( first ) {
	first = false;
      }
      else {
	stat = max_d0 - d0;
      }
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
    cout << "after walsh_w0_refine()" << endl;
    print_polconf_list(cout, polconf_list, ni);
  }
}

END_NAMESPACE_YM_LOGIC
