#ifndef BDDVARSET_H
#define BDDVARSET_H

/// @file BddVarSet.h
/// @brief BddVarSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddVarSet BddVarSet.h "BddVarSet.h"
/// @brief Bdd を用いて変数集合を表すクラス
//////////////////////////////////////////////////////////////////////
class BddVarSet :
  private Bdd
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  BddVarSet() = default;

  /// @brief 要素のリストを指定したコンストラクタ
  ///
  /// var_set を省略した場合には空集合となる．
  BddVarSet(
    BddMgr& mgr,                    ///< [in] BDDマネージャ
    const vector<SizeType>& var_set ///< [in] 変数のリスト
    = vector<SizeType>{}
  );

  /// @brief デストラクタ
  ~BddVarSet() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 和集合を求める．
  BddVarSet
  operator+(
    const BddVarSet& right ///< [in] オペランド
  ) const;

  /// @brief 和集合を求め代入する．
  BddVarSet&
  operator+=(
    const BddVarSet& right ///< [in] オペランド
  );

  /// @brief 差集合を求める．
  BddVarSet
  operator-(
    const BddVarSet& right ///< [in] オペランド
  ) const;

  /// @brief 差集合を求め代入する．
  BddVarSet&
  operator-=(
    const BddVarSet& right ///< [in] オペランド
  );

  /// @brief 共通集合を求める．
  BddVarSet
  operator&(
    const BddVarSet& right //< [in] オペランド
  ) const;

  /// @brief 共通集合を求め代入する．
  BddVarSet&
  operator&=(
    const BddVarSet& right //< [in] オペランド
  );

  /// @brief 共通部分を持つかチェックする．
  bool
  operator&&(
    const BddVarSet& right //< [in] オペランド
  ) const;

  /// @brief 等価比較
  bool
  operator==(
    const BddVarSet& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較
  bool
  operator!=(
    const BddVarSet& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }

  /// @brief サイズを返す．
  SizeType
  size() const;

  /// @brief 変数のリストに変換する．
  ///
  /// 変換できない時は例外を送出する．
  vector<SizeType>
  to_varlist() const;

  /// @brief 先頭の変数を返す．
  SizeType
  top_var() const;

  /// @brief 内容を表すBDDを返す．
  Bdd
  bdd() const
  {
    return *this;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メンバを指定したコンストラクタ
  BddVarSet(
    const Bdd& bdd
  ) : Bdd{bdd}
  {
  }

};

END_NAMESPACE_YM_DD

#endif // BDDVARSET_H
