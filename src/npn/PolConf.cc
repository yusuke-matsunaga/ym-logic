
/// @file PolConf.cc
/// @brief PolConf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "PolConf.h"

BEGIN_NAMESPACE_YM_LOGIC

void
print_polconf(ostream& s,
	      const PolConf& polconf,
	      ymuint ni)
{
  ymuint inv_bits = polconf.iinv_bits();
  for (ymuint i = 0; i < ni; ++ i) {
    if ( inv_bits & (1U << i) ) {
      s << "N";
    }
    else {
      s << "P";
    }
  }
  s << "|";
  if ( polconf.oinv() ) {
    s << "N";
  }
  else {
    s << "P";
  }
  s << endl;
}

void
print_polconf_list(ostream& s,
		   const vector<PolConf>& polconf_list,
		   ymuint ni)
{
  for (vector<PolConf>::const_iterator p = polconf_list.begin();
       p != polconf_list.end(); ++ p) {
    print_polconf(s, *p, ni);
  }
}

END_NAMESPACE_YM_LOGIC
