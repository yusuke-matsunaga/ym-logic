
/// @file Literal.cc
/// @brief Literal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Literal.h"
#include "ym/VarId.h"


BEGIN_NAMESPACE_YM

// ostream に対する書出し
ostream&
operator<<(ostream& s,
	   const Literal& lit)
{
  if ( lit.is_valid() ) {
    s << lit.varid();
    if ( lit.is_negative() ) {
      s << "'";
    }
  }
  else {
    s << "-X-";
  }
  return s;
}

END_NAMESPACE_YM
