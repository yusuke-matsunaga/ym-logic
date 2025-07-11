#ifndef BDDMULTICOMPOP_H
#define BDDMULTICOMPOP_H

/// @file BddMultiCompOp.h
/// @brief BddMultiCompOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddOpBase.h"
#include "BddIteOp.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddMultiCompOp BddMultiCompOp.h "BddMultiCompOp.h"
/// @brief 複数のcompose演算を行うクラス
//////////////////////////////////////////////////////////////////////
class BddMultiCompOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddMultiCompOp(
    BddMgrImpl* mgr,                                     ///< [in] マネージャ
    const std::unordered_map<SizeType, DdEdge>& comp_map ///< [in] 変換マップ
  );

  /// @brief デストラクタ
  ~BddMultiCompOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のcompose演算を行う．
  DdEdge
  mcomp_op(
    DdEdge edge
  )
  {
    return mcomp_step(edge, 0);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のcompose演算を行う．
  DdEdge
  mcomp_step(
    DdEdge edge,
    SizeType pos
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変換リスト
  std::vector<std::pair<SizeType, DdEdge>> mCompList;

  // 演算結果テーブル
  std::unordered_map<const DdNode*, DdEdge> mTable;

  // ITE演算
  BddIteOp mIteOp;

};

END_NAMESPACE_YM_DD

#endif // BDDMULTICOMPOP_H
