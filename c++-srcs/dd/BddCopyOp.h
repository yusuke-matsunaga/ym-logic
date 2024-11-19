#ifndef BDDCOPYOP_H
#define BDDCOPYOP_H

/// @file BddCopyOp.h
/// @brief BddCopyOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class BddCopyOp BddCopyOp.h "BddCopyOp.h"
/// @brief BDD のコピーを行う演算
///
/// 対象と自分のマネージャが異なるので変数順も異なる可能性がある．
//////////////////////////////////////////////////////////////////////
class BddCopyOp
{
public:

  /// @brief コンストラクタ
  BddCopyOp(
    BddMgrImpl& mgr,                   ///< [in] マネージャ
    const vector<BddVar>& var_list,    ///< [in] 変数リスト
    const vector<SizeType>& level_list ///< [in] レベルのリスト
  ) : mMgr{mgr},
      mVarList{var_list},
      mLevelList{level_list}
  {
  }

  /// @brief デストラクタ
  ~BddCopyOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  DdEdge
  copy_step(
    const Bdd& bdd,
    SizeType pos
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Bddマネージャ
  BddMgrImpl& mMgr;

  // 変数リスト
  vector<BddVar> mVarList;

  // レベルのリスト
  vector<SizeType> mLevelList;

  // 演算結果テーブル
  unordered_map<Bdd, DdEdge> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDCOPYOP_H
