#ifndef BDDLIT_H
#define BDDLIT_H

/// @file BddLit.h
/// @brief BddLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BddObj.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddLit BddLit.h "BddLit.h"
/// @brief BDD のリテラルを表すクラス
///
/// - ブール論理の世界においてはリテラルとは変数とその否定のみ
/// - 本当はオーバーフローの危険があるが，下位1ビットを極性にする．
//////////////////////////////////////////////////////////////////////
class BddLit :
  public BddObj
{
  friend class BddMgrImpl;
private:

  /// @brief コンストラクタ
  BddLit(
    BddMgrImpl* mgr, ///< [in] BDDマネージャ
    SizeType val     ///< [in] 変数番号と極性をパックしたもの
  ) : BddObj{mgr},
      mVal{val}
  {
  }


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddLit() = default;

  /// @brief 肯定のリテラルを作るクラスメソッド
  static
  BddLit
  posi_literal(
    const BddVar& var ///< [in] 変数
  )
  {
    auto val = var.mVarId << 1;
    return BddLit{var._mgr(), val};
  }

  /// @brief 否定のリテラルを作るクラスメソッド
  static
  BddLit
  nega_literal(
    const BddVar& var ///< [in] 変数
  )
  {
    auto val = (var.mVarId << 1) | 1UL;
    return BddLit{var._mgr(), val};
  }

  /// @brief コピーコンストラクタ
  BddLit(
    const BddLit& src ///< [in] コピー元のオブジェクト
  ) : BddObj{src},
      mVal{src.mVal}
  {
  }

  /// @brief デストラクタ
  ~BddLit() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を取り出す．
  BddVar
  var() const
  {
    return BddVar{_mgr(), mVal >> 1};
  }

  /// @brief 同じ変数の肯定のリテラルを返す．
  BddLit
  make_positive() const
  {
    return BddLit{_mgr(), mVal & ~1UL};
  }

  /// @brief 同じ変数の否定のリテラルを返す．
  BddLit
  make_negative() const
  {
    return BddLit{_mgr(), mVal | 1UL};
  }

  /// @brief 極性を反転したリテラルを返す．
  BddLit
  complement() const
  {
    return BddLit{_mgr(), mVal ^ 1UL};
  }

  /// @brief 肯定のリテラルのとき true を返す．
  bool
  is_positive() const
  {
    return static_cast<bool>((mVal & 1) == 0);
  }

  /// @brief 否定のリテラルのとき true を返す．
  bool
  is_negative() const
  {
    return static_cast<bool>((mVal & 1) == 1);
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
    if ( mVal == right.mVal && _mgr() == right._mgr() ) {
      return true;
    }
    return false;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(q
    const BddLit& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief ハッシュ関数
  SizeType
  hash() const
  {
    return mVal;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号を極性をパックしたもの
  SizeType mVal;

};

END_NAMESPACE_YM_DD

#endif // BDDLIT_H
