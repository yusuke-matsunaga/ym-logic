#ifndef SUPOP_H
#define SUPOP_H

/// @file SupOp.h
/// @brief SupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "OpBase.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class SupOp SupOp.h "SupOp.h"
/// @brief サポートを表すBDDを求める演算
//////////////////////////////////////////////////////////////////////
class SupOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  SupOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~SupOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サポートを表すBDDを返す．
  BddEdge
  get_step(
    BddEdge edge ///< [in] 枝
  );

  /// @brief サポートのユニオンを求める．
  BddEdge
  cup_step(
    BddEdge edge0,
    BddEdge edge1
  );

  /// @brief サポートのインターセクションを求める．
  BddEdge
  cap_step(
    BddEdge edge0,
    BddEdge edge1
  );

  /// @brief サポートの差を求める．
  BddEdge
  diff_step(
    BddEdge edge0,
    BddEdge edge1
  );

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<BddNode*, BddEdge> mTable;

};

END_NAMESPACE_YM_BDD

#endif // SUPOP_H
