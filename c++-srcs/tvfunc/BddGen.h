#ifndef BDDGEN_H
#define BDDGEN_H

/// @file BddGen.h
/// @brief BddGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BddGen BddGen.h "BddGen.h"
/// @brief Tv2Bdd の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class BddGen
{
public:

  /// @brief コンストラクタ
  BddGen(
    const TvFunc& func,                 ///< [in] 対象の関数
    BddMgr& mgr,                        ///< [in] BDD マネージャ
    const std::vector<BddVar>& var_list ///< [in] 変数リスト
  ) : mFunc{func},
      mMgr{mgr},
      mVarList{var_list}
  {
  }

  /// @brief デストラクタ
  ~BddGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD を作る．
  Bdd
  run();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief run() の下請け関数
  Bdd
  decomp_step(
    SizeType var,   ///< [in] トップの変数
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の関数
  const TvFunc& mFunc;

  // BDD マネージャ
  BddMgr& mMgr;

  // 変数のリスト
  std::vector<BddVar> mVarList;

};

END_NAMESPACE_YM

#endif // BDDGEN_H
