#ifndef MULTICOMPOP_H
#define MULTICOMPOP_H

/// @file MultiCompOp.h
/// @brief MultiCompOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "OpBase.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class MultiCompOp MultiCompOp.h "MultiCompOp.h"
/// @brief 複数のcompose演算を行うクラス
//////////////////////////////////////////////////////////////////////
class MultiCompOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  MultiCompOp(
    BddMgrImpl* mgr,                                 ///< [in] マネージャ
    const unordered_map<SizeType, BddEdge>& comp_map ///< [in] 変換マップ
  );

  /// @brief デストラクタ
  ~MultiCompOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のcompose演算を行う．
  BddEdge
  mcomp_op(
    BddEdge edge
  )
  {
    return mcomp_step(edge, 0);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のcompose演算を行う．
  BddEdge
  mcomp_step(
    BddEdge edge,
    SizeType pos
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変換リスト
  vector<pair<SizeType, BddEdge>> mCompList;

  // 演算結果テーブル
  unordered_map<BddNode*, BddEdge> mTable;

};

END_NAMESPACE_YM_BDD

#endif // MULTICOMPOP_H
