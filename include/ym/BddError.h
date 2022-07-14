#ifndef BDDERROR_H
#define BDDERROR_H

/// @file BddError.h
/// @brief BddError のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BddError BddError.h "BddError.h"
/// @brief BDD 関係のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class BddError :
  public exception
{
public:

  /// @brief コンストラクタ
  BddError(
    const string& msg ///< [in] エラーメッセージ
  ) : mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~BddError() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表す文字列を返す．
  const string&
  str() const
  {
    return mMsg;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージ
  string mMsg;

};

END_NAMESPACE_YM

#endif // BDDERROR_H
