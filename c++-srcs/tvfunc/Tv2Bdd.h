#ifndef TV2BDD_H
#define TV2BDD_H

/// @file Tv2Bdd.h
/// @brief Tv2Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Tv2Bdd Tv2Bdd.h "Tv2Bdd.h"
/// @brief TvFunc を Bdd に変換するためのクラス
//////////////////////////////////////////////////////////////////////
class Tv2Bdd
{
public:

  /// @brief コンストラクタ
  Tv2Bdd(
    BddMgr& mgr,       ///< [in] BDD マネージャ
    const TvFunc& func ///< [in] 対象の関数
  ) : mMgr{mgr},
      mFunc{func},
      mOrder(func.input_num())
  {
  }

  /// @brief デストラクタ
  ~Tv2Bdd() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD を作る．
  Bdd
  run(
    const vector<SizeType>& order ///< [in] 変数順
  );


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

  // BDD マネージャ
  BddMgr& mMgr;

  // 対象の関数
  const TvFunc& mFunc;

  // 変数順
  vector<SizeType> mOrder;

};

END_NAMESPACE_YM

#endif // TV2BDD_H
