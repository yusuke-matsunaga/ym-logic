#ifndef TRUTHOP_H
#define TRUTHOP_H

/// @file TruthOp.h
/// @brief TruthOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class TruthOp TruthOp.h "TruthOp.h"
/// @brief 真理値表の文字列からBDDを作る演算
//////////////////////////////////////////////////////////////////////
class TruthOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  TruthOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : OpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~TruthOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 真理値表の文字列からBDDを作る．
  BddEdge
  op_step(
    const string& str, ///< [in] 文字列
    SizeType index     ///< [in] 先頭の変数インデックス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  unordered_map<string, BddEdge> mTable;

};

END_NAMESPACE_YM_BDD

#endif // TRUTHOP_H
