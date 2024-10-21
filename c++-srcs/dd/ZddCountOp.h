#ifndef ZDDCOUNTOP_H
#define ZDDCOUNTOP_H

/// @file ZddCountOp.h
/// @brief ZddCountOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddCountOp ZddCountOp.h "ZddCountOp.h"
/// @brief 要素数を数える
//////////////////////////////////////////////////////////////////////
class ZddCountOp
{
public:

  /// @brief コンストラクタ
  ZddCountOp() = default;

  /// @brief デストラクタ
  ~ZddCountOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  SizeType
  count_step(
    DdEdge edge
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<DdEdge, SizeType> mTable;

};

END_NAMESPACE_YM_DD

#endif // COUNTOP_H
