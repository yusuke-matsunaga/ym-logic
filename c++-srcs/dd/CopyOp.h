#ifndef COPYOP_H
#define COPYOP_H

/// @file CopyOp.h
/// @brief CopyOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

class DdNodeMgr;
class DdEdge;
class DdNode;

//////////////////////////////////////////////////////////////////////
/// @class CopyOp CopyOp.h "CopyOp.h"
/// @brief DD のコピーを行う演算
///
/// これは DD の構造をコピーするので BDD と ZDD で共通の操作となる．
//////////////////////////////////////////////////////////////////////
class CopyOp
{
public:

  /// @brief コンストラクタ
  CopyOp(
    DdNodeMgr& mgr ///< [in] マネージャ
  ) : mNodeMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~CopyOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  DdEdge
  copy_step(
    DdEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードマネージャ
  DdNodeMgr& mNodeMgr;

  // 演算結果テーブル
  unordered_map<const DdNode*, const DdNode*> mTable;

};

END_NAMESPACE_YM_DD

#endif // COPYOP_H
