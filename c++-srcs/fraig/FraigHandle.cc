
/// @file FraigHandle.cc
/// @brief FraigHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigHandle.h"
#include "FraigNode.h"


BEGIN_NAMESPACE_EQUIV

//////////////////////////////////////////////////////////////////////
// FraigHandle
//////////////////////////////////////////////////////////////////////

// @relates FraigHandle
// @brief 内容を出力する関数
ostream&
operator<<(
  ostream& s,
  FraigHandle src
)
{
  if ( src.is_zero() ) {
    s << "ZERO";
  }
  else if ( src.is_one() ) {
    s << "ONE";
  }
  else {
    if ( src.inv() ) {
      s << "~";
    }
    if ( src.is_input() ) {
      s << "I" << src.input_id();
    }
    else {
      s << "A" << src.node()->id();
    }
  }
  return s;
}

END_NAMESPACE_EQUIV
