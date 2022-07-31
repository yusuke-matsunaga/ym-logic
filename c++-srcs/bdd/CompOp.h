#ifndef COMPOP_H
#define COMPOP_H

/// @file CompOp.h
/// @brief CompOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CompOp CompOp.h "CompOp.h"
/// @brief compose 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class CompOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  CompOp(
    BddMgrImpl* mgr, ///< [in] マネージャ
    SizeType index,  ///< [in] 対象のインデックス
    BddEdge cedge    ///< [in] 置き換えるBDD
  ) : OpBase{mgr},
      mIndex{index},
      mCedge{cedge}
  {
  }

  /// @brief デストラクタ
  ~CompOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief compose 演算
  BddEdge
  comp_op(
    BddEdge edge
  )
  {
    return comp_step(edge);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief compose 演算
  BddEdge
  comp_step(
    BddEdge edge
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  SizeType mIndex;

  // 置き換えるBDD
  BddEdge mCedge;

  // 演算結果テーブル
  unordered_map<BddNode*, BddEdge> mTable;

};

END_NAMESPACE_YM_BDD

#endif // COMPOP_H
