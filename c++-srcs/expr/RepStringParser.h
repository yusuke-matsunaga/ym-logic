#ifndef REPSTRINGPARSER_H
#define REPSTRINGPARSER_H

/// @file RepStringParser.h
/// @brief RepStringParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class RepStringParser RepStringParser.h "RepStringParser.h"
/// @brief rep_string を読み込むパーサー
//////////////////////////////////////////////////////////////////////
class RepStringParser
{
public:

  /// @brief コンストラクタ
  RepStringParser(
    const string& rep_string ///< [in] 文字列
  ) : mString{rep_string},
      mPos{0}
  {
  }

  /// @brief デストラクタ
  ~RepStringParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 一文字読み出す．
  char
  read_char()
  {
    ASSERT_COND( mPos < mString.size() );
    auto c = mString[mPos];
    ++ mPos;
    return c;
  }

  /// @brief 整数を読み出す．
  int
  read_int()
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列
  string mString;

  // 読み出し位置
  SizeType mPos;

};

END_NAMESPACE_YM_LOGIC

#endif // REPSTRINGPARSER_H
