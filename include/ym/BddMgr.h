#ifndef BDDMGR_H
#define BDDMGR_H

/// @file BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM

class Bdd;
class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class BddMgr BddMgr.h "ym/BddMgr.h"
/// @brief BDD を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class BddMgr
{
public:

  /// @brief コンストラクタ
  BddMgr();

  /// @brief デストラクタ
  ~BddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を割り当てる．
  /// @return 変数を返す．
  BddVar
  new_variable(
    const string& name = string{} ///< [in] 変数名(オプショナル)
  );

  /// @brief 恒儀関数を作る．
  Bdd
  make_zero();

  /// @brief 恒新関数を作る．
  Bdd
  make_one();

  /// @brief リテラル関数を作る．
  Bdd
  make_literal(
    BddVar var,      ///< [in] 変数
    bool inv = false ///< [in] 反転フラグ
  );

  /// @brief 肯定のリテラル関数を作る．
  Bdd
  make_posi_literal(
    BddVar var  ///< [in] 変数
  );

  /// @brief 否定のリテラル関数を作る．
  Bdd
  make_nega_literal(
    BddVar var  ///< [in] 変数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  BddMgrImpl* mImpl;

};

END_NAMESPACE_YM

#endif // BDDMGR_H
