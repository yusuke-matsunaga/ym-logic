#ifndef BNET2BDD_H
#define BNET2BDD_H

/// @file Bnet2Bdd.h
/// @brief Bnet2Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNetwork.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Bnet2Bdd Bnet2Bdd.h "Bnet2Bdd.h"
/// @brief BnNetwork の global function を表す BDD を作るクラス
//////////////////////////////////////////////////////////////////////
class Bnet2Bdd
{
public:

  /// @brief コンストラクタ
  Bnet2Bdd(
    BddMgr& mgr,             ///< [in] BDDマネージャ
    const BnNetwork& network ///< [in] ネットワーク
  );

  /// @brief デストラクタ
  ~Bnet2Bdd() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNetwork の global function を作る．
  vector<Bdd>
  make_global_func();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode に対応するBDDを作る．
  Bdd
  make_node_func(
    SizeType node_id ///< [in] ノード番号
  );

  /// @brief 論理積を計算する．
  Bdd
  make_and(
    const vector<Bdd>& fanin_list
  );

  /// @brief 論理和を計算する．
  Bdd
  make_or(
    const vector<Bdd>& fanin_list
  );

  /// @brief 排他的論理和を計算する．
  Bdd
  make_xor(
    const vector<Bdd>& fanin_list
  );

  /// @brief 論理式を計算する．
  Bdd
  make_expr(
    const Expr& expr,
    const vector<Bdd>& fanin_list
  );

  /// @brief 真理値表を計算する．
  Bdd
  make_func(
    const TvFunc& func,
    const vector<Bdd>& fanin_list
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  BddMgr& mMgr;

  // ネットワーク
  const BnNetwork& mNetwork;

  // ノード番号をキーにして BDD を格納する辞書
  unordered_map<SizeType, Bdd> mBddMap;

};

END_NAMESPACE_YM

#endif // BNET2BDD_H
