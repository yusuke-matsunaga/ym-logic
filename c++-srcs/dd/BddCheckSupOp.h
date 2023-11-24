#ifndef BDDCHECKSUPOP_H
#define BDDCHECKSUPOP_H

/// @file BddCheckSupOp.h
/// @brief BddCheckSupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddCheckSupOp BddCheckSupOp.h "BddCheckSupOp.h"
/// @brief サポートチェックを行う
//////////////////////////////////////////////////////////////////////
class BddCheckSupOp
{
public:

  /// @brief コンストラクタ
  BddCheckSupOp(
    SizeType index   ///< [in] インデックス
  ) : mIndex{index}
  {
  }

  /// @brief デストラクタ
  ~BddCheckSupOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サポートチェックを行う．
  bool
  op_step(
    DdEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  SizeType mIndex;

  // 演算結果テーブル
  unordered_map<const DdNode*, bool> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDCHECKSUPOP_H
