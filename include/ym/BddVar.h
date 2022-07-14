#ifndef BDDVAR_H
#define BDDVAR_H

/// @file BddVar.h
/// @brief BddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM

class BddMgr;

//////////////////////////////////////////////////////////////////////
/// @class BddVar BddVar.h "ym/BddVar.h"
/// @brief BDd の変数を表すクラス
///
/// 中身はただの整数だが，BddMgr で変数を割り当てないと使えない．
//////////////////////////////////////////////////////////////////////
class BddVar
{
  friend class BddMgrImpl;

private:

  /// @brief コンストラクタ
  BddVar(
    SizeType index,      ///< [in] インデックス
    const string& symbol ///< [in] 名前
  ) : mIndex{index},
      mSymbol{symbol}
  {
  }


public:

  /// @brief コピーコンストラクタ
  BddVar(
    const BddVar& src
  ) = default;

  /// @brief 代入演算子
  BddVar&
  operator=(
    const BddVar& src
  ) = default;

  /// @brief 不正値を返す．
  static
  BddVar
  make_invalid()
  {
    return BddVar{static_cast<SizeType>(-1), string{}};
  }

  /// @brief デストラクタ
  ~BddVar() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正な値を持っている時に true を返す．
  bool
  is_invalid() const
  {
    return mIndex == static_cast<SizeType>(-1);
  }

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return !is_invalid();
  }

  /// @brief インデックスを返す．
  SizeType
  index() const
  {
    return mIndex;
  }

  /// @brief 名前を返す．
  string
  symbol() const
  {
    return mSymbol;
  }

  /// @brief 等価比較
  bool
  operator==(
    const BddVar& right
  ) const
  {
    return mIndex == right.mIndex;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  SizeType mIndex;

  // 名前
  string mSymbol;

};

/// @brief 非等価比較
inline
bool
operator!=(
  const BddVar& left,
  const BddVar& right
)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM

#endif // BDDVAR_H
