#ifndef BDDEXPROP_H
#define BDDEXPROP_H

/// @file BddExprOp.h
/// @brief BddExprOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "BddIteOp.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddExprOp BddExprOp.h "BddExprOp.h"
/// @brief Expr を Bdd に変換する
//////////////////////////////////////////////////////////////////////
class BddExprOp:
  public BddIteOp
{
public:

  /// @brief コンストラクタ
  BddExprOp(
    BddMgrImpl* mgr,                   ///< [in] マネージャ
    const vector<SizeType>& index_list ///< [in] インデックスリスト
  ) : BddIteOp{mgr},
      mLevelList{index_list}
  {
  }

  /// @brief デストラクタ
  ~BddExprOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Expr を BDD に変換する．
  DdEdge
  op_step(
    const Expr& expr ///< [in] 論理式
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスリスト
  vector<SizeType> mLevelList;

};

END_NAMESPACE_YM_DD

#endif // BDDEXPROP_H
