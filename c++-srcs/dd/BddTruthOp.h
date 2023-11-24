#ifndef BDDTRUTHOP_H
#define BDDTRUTHOP_H

/// @file BddTruthOp.h
/// @brief BddTruthOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddOpBase.h"
#include "DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddTruthOp BddTruthOp.h "BddTruthOp.h"
/// @brief 真理値表の文字列からBDDを作る演算
//////////////////////////////////////////////////////////////////////
class BddTruthOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddTruthOp(
    BddMgrImpl& mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddTruthOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 真理値表の文字列からBDDを作る．
  DdEdge
  op_step(
    const string& str, ///< [in] 文字列
    SizeType index     ///< [in] 先頭の変数インデックス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<string, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDTRUTHOP_H
