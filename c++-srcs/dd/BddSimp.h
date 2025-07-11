#ifndef BDDSIMP_H
#define BDDSIMP_H

/// @file BddSimp.h
/// @brief BddSimp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddSimp BddSimp.h "BddSimp.h"
/// @brief ITE演算(AND, XOR)を行うクラス
//////////////////////////////////////////////////////////////////////
class BddSimp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddSimp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddSimp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 簡単化を行う．
  DdEdge
  simp_step(
    DdEdge left,
    DdEdge right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // simplify用のテーブル
  std::unordered_map<Apply2Key, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDSIMP_H
