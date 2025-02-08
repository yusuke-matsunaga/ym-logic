#ifndef TV2BDD_H
#define TV2BDD_H

/// @file Tv2Bdd.h
/// @brief Tv2Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Tv2Bdd Tv2Bdd.h "Tv2Bdd.h"
/// @brief TvFunc を Bdd に変換するクラス
///
/// 実際にはただの関数群
//////////////////////////////////////////////////////////////////////
class Tv2Bdd
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief TvFunc を BDD に変換する．
  static
  Bdd
  gen_bdd(
    const TvFunc& f, ///< [in] 対象の関数
    BddMgr& mgr      ///< [in] BDDマネージャ
  );

  /// @brief BDD に変換する．
  ///
  /// - f の i 番目の変数を var_list[i] に対応させる．
  /// - var_list.size() >= input_num() でなければ
  ///   std::invalid_argument 例外を送出する．
  static
  Bdd
  gen_bdd(
    const TvFunc& f,               ///< [in] 対象の関数
    BddMgr& mgr,                   ///< [in] BDDマネージャ
    const vector<BddVar>& var_list ///< [in] 変数リスト
  );

  /// @brief 複数の TvFunc を BDD のリストに変換する．
  static
  vector<Bdd>
  gen_bdd(
    const vector<TvFunc>& f_list, ///< [in] 対象の関数のリスト
    BddMgr& mgr                   ///< [in] BDDマネージャ
  );

  /// @brief 複数の TvFunc を BDD のリストに変換する．
  ///
  /// - f の i 番目の変数を var_list[i] に対応させる．
  /// - var_list.size() >= input_num() でなければ
  ///   std::invalid_argument 例外を送出する．
  static
  vector<Bdd>
  gen_bdd(
    const vector<TvFunc>& f_list,  ///< [in] 対象の関数のリスト
    BddMgr& mgr,                   ///< [in] BDDマネージャ
    const vector<BddVar>& var_list ///< [in] 変数リスト
  );

};

END_NAMESPACE_YM

#endif // TV2BDD_H
