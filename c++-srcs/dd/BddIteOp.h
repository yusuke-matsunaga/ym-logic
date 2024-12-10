#ifndef BDDITEOP_H
#define BDDITEOP_H

/// @file BddIteOp.h
/// @brief BddIteOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"
#include "Apply3Key.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddIteOp BddIteOp.h "BddIteOp.h"
/// @brief ITE演算(AND, XOR)を行うクラス
//////////////////////////////////////////////////////////////////////
class BddIteOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddIteOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddIteOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief AND 演算を行う．
  DdEdge
  and_step(
    DdEdge left,
    DdEdge right
  );

  /// @brief OR 演算を行う．
  DdEdge
  or_step(
    DdEdge left,
    DdEdge right
  )
  {
    // DeMorgan の法則
    return ~and_step(~left, ~right);
  }

  /// @brief XOR 演算を行う．
  DdEdge
  xor_step(
    DdEdge left,
    DdEdge right
  );

  /// @brief ITE 演算を行う．
  DdEdge
  ite_step(
    DdEdge e0,
    DdEdge e1,
    DdEdge e2
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ITE用のテーブル
  unordered_map<Apply3Key, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDITEOP_H
