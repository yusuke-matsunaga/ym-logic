#ifndef BDDMGR_H
#define BDDMGR_H

/// @file BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "ym/VarId.h"


BEGIN_NAMESPACE_YM_BDD

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

  /// @brief 恒儀関数を作る．
  Bdd
  make_zero();

  /// @brief 恒新関数を作る．
  Bdd
  make_one();

  /// @brief リテラル関数を作る．
  Bdd
  make_literal(
    VarId var,       ///< [in] 変数
    bool inv = false ///< [in] 反転フラグ
  );

  /// @brief 肯定のリテラル関数を作る．
  Bdd
  make_posi_literal(
    VarId var ///< [in] 変数
  );

  /// @brief 否定のリテラル関数を作る．
  Bdd
  make_nega_literal(
    VarId var ///< [in] 変数
  );

  /// @brief 真理値表形式の文字列からBDDを作る．
  ///
  /// str の形式は以下の通り
  /// - big endian: 0文字目が(1, 1, 1, 1)に対応する
  Bdd
  from_truth(
    const string& str ///< [in] 01の文字列
  );

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief GC を起動するしきい値を返す．
  SizeType
  gc_limit() const;

  /// @brief GC を起動するしきい値を設定する．
  void
  set_gc_limit(
    SizeType limit
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  BddMgrImpl* mImpl;

};

END_NAMESPACE_YM_BDD

#endif // BDDMGR_H
