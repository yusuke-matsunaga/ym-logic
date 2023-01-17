
/// @file PolConf.cc
/// @brief PolConf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "PolConf.h"
#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief NpnMap に変換する．
// @param[in] ni 入力数
NpnMap
PolConf::to_npnmap(
  int ni
) const
{
  NpnMap map(ni);
  map.set_oinv(oinv());
  for (int i = 0; i < ni; ++ i) {
    map.set(i, i, iinv(i));
  }
  return map;
}

void
print_polconf(
  ostream& s,
  const PolConf& polconf,
  int ni
)
{
  ymuint32 inv_bits = polconf.iinv_bits();
  for (int i = 0; i < ni; ++ i) {
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
print_polconf_list(
  ostream& s,
  const vector<PolConf>& polconf_list,
  int ni
)
{
  for (vector<PolConf>::const_iterator p = polconf_list.begin();
       p != polconf_list.end(); ++ p) {
    print_polconf(s, *p, ni);
  }
}

END_NAMESPACE_YM_LOGIC
