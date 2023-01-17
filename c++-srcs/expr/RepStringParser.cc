
/// @file RepStringParser.cc
/// @brief RepStringParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "RepStringParser.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief 一文字読み出す．
char
RepStringParser::read_char()
{
  ASSERT_COND( mPos < mString.size() );
  auto c = mString[mPos];
  ++ mPos;
  return c;
}

// @brief 整数を読み出す．
int
RepStringParser::read_int()
{
  int ans = 0;
  while ( mPos < mString.size() ) {
    char c = read_char();
    if ( !isdigit(c) ) {
      -- mPos;
      break;
    }
    ans = ans * 10 + static_cast<int>(c) - static_cast<int>('0');
  }
  return ans;
}

END_NAMESPACE_YM_LOGIC
