#ifndef BDDLIT_H
#define BDDLIT_H

/// @file BddLit.h
/// @brief BddLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

inline
Bdd
var_to_lit(
  const BddVar& var,
  bool inv
)
{
  auto bdd = static_cast<Bdd>(var);
  if ( inv ) {
    bdd = ~bdd;
  }
  return bdd;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @class BddLit BddLit.h "BddLit.h"
/// @brief BDD のリテラルを表すクラス
///
/// - ブール論理の世界においてはリテラルとは変数とその否定のみ
/// - 実際にはリテラル関数を表すBDDを用いている．
//////////////////////////////////////////////////////////////////////
class BddLit :
  public Bdd
{
#if 0
  friend class BddMgrImpl;
private:

  /// @brief コンストラクタ
  BddLit(
    BddMgrImpl* mgr, ///< [in] BDDマネージャ
    DdEdge root      ///< [in] 根の枝
  ) : Bdd{mgr, root}
  {
  }
#endif

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddLit() = default;

  /// @brief BddVar と極性を引数にとるコンストラクタ
  explicit
  BddLit(
    const BddVar& var, ///< [in] 変数
    bool inv = false   ///< [in] 極性
                       ///<      - false: 肯定(反転なし)
                       ///<      - true:  否定(反転あり)
  ) : Bdd{var_to_lit(var, inv)}
  {
  }

  /// @brief コピーコンストラクタ
  BddLit(
    const BddLit& src ///< [in] コピー元のオブジェクト
  ) : Bdd{src}
  {
  }

  /// @brief デストラクタ
  ~BddLit() = default;

  /// @brief 不正な値を作るクラスメソッド
  static
  BddLit
  invalid()
  {
    return BddLit{};
  }

  /// @brief 肯定のリテラルを作るクラスメソッド
  static
  BddLit
  positive_literal(
    const BddVar& var ///< [in] 変数
  )
  {
    return BddLit{var, false};
  }

  /// @brief 否定のリテラルを作るクラスメソッド
  static
  BddLit
  negative_literal(
    const BddVar& var ///< [in] 変数
  )
  {
    return BddLit{var, true};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を取り出す．
  BddVar
  var() const
  {
    if ( is_positive() ) {
      return BddVar{static_cast<Bdd>(*this)};
    }
    else {
      return BddVar{~static_cast<Bdd>(*this)};
    }
  }

  /// @brief 同じ変数の肯定のリテラルを返す．
  BddLit
  make_positive() const
  {
    return BddLit{var(), false};
  }

  /// @brief 同じ変数の否定のリテラルを返す．
  BddLit
  make_negative() const
  {
    return BddLit{var(), true};
  }

  /// @brief 極性を反転したリテラルを返す．
  BddLit
  complement() const
  {
    return BddLit{var(), is_positive()};
  }

  /// @brief 肯定のリテラルのとき true を返す．
  bool
  is_positive() const
  {
    return root_inv();
  }

  /// @brief 否定のリテラルのとき true を返す．
  bool
  is_negative() const
  {
    return !is_positive();
  }

  /// @brief 反転演算子(complementの別名)
  BddLit
  operator~() const
  {
    return complement();
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddLit& right
  ) const
  {
    return Bdd::operator==(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddLit& right
  ) const
  {
    return !operator==(right);
  }

};

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// BddLit をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::BddLit>
{
  SizeType
  operator()(
    const YM_NAMESPACE::BddLit& lit
  ) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_STD

#endif // BDDLIT_H
