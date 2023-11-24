#ifndef BDDSUPOP_H
#define BDDSUPOP_H

/// @file BddSupOp.h
/// @brief BddSupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddSupOp BddSupOp.h "BddSupOp.h"
/// @brief サポートを表すBDDを求める演算
//////////////////////////////////////////////////////////////////////
class BddSupOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddSupOp(
    BddMgrImpl& mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddSupOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サポートを表すBDDを返す．
  DdEdge
  get_step(
    DdEdge edge ///< [in] 枝
  );

  /// @brief サポートのユニオンを求める．
  DdEdge
  cup_step(
    DdEdge edge0,
    DdEdge edge1
  );

  /// @brief サポートのインターセクションを求める．
  DdEdge
  cap_step(
    DdEdge edge0,
    DdEdge edge1
  );

  /// @brief サポートの差を求める．
  DdEdge
  diff_step(
    DdEdge edge0,
    DdEdge edge1
  );

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<const DdNode*, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDSUPOP_H
