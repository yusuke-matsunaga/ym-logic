
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
  SizeType ni
) const
{
  NpnMap map(ni);
  map.set_oinv(oinv());
  for ( SizeType i = 0; i < ni; ++ i ) {
    map.set_imap(i, i, iinv(i));
  }
  return map;
}

void
print_polconf(
  std::ostream& s,
  const PolConf& polconf,
  SizeType ni
)
{
  auto inv_bits = polconf.iinv_bits();
  for ( SizeType i = 0; i < ni; ++ i ) {
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
  s << std::endl;
}

void
print_polconf_list(
  std::ostream& s,
  const std::vector<PolConf>& polconf_list,
  SizeType ni
)
{
  for ( auto p = polconf_list.begin();
	p != polconf_list.end(); ++ p ) {
    print_polconf(s, *p, ni);
  }
}

END_NAMESPACE_YM_LOGIC
