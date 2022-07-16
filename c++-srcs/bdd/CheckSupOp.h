#ifndef CHECKSUPOP_H
#define CHECKSUPOP_H

/// @file CheckSupOp.h
/// @brief CheckSupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CheckSupOp CheckSupOp.h "CheckSupOp.h"
/// @brief サポートチェックを行う
//////////////////////////////////////////////////////////////////////
class CheckSupOp
{
public:

  /// @brief コンストラクタ
  CheckSupOp(
    SizeType index   ///< [in] インデックス
  ) : mIndex{index}
  {
  }

  /// @brief デストラクタ
  ~CheckSupOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サポートチェックを行う．
  bool
  op_step(
    BddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  SizeType mIndex;

  // 演算結果テーブル
  unordered_map<BddNode*, bool> mTable;

};

END_NAMESPACE_YM_BDD

#endif // CHECKSUPOP_H
