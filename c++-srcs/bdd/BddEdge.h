#ifndef BDDEDGE_H
#define BDDEDGE_H

/// @file BddEdge.h
/// @brief BddEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

class BddNode;

//////////////////////////////////////////////////////////////////////
/// @class BddEdge BddEdge.h "BddEdge.h"
/// @brief BDD の枝を表すクラス
///
/// 実体はポインタの下位ビットにフラグを埋め込んでいる．
//////////////////////////////////////////////////////////////////////
class BddEdge
{
public:

  /// @brief コンストラクタ
  BddEdge(
    ympuint body = 0UL
  ) : mBody{body}
  {
  }

  /// @brief ノードと極性を指定したコンストラクタ
  BddEdge(
    BddNode* node,   ///< [in] ノード
    bool inv = false ///< [in] 反転フラグ
  ) : mBody{reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv)}
  {
  }

  /// @brief 定数0を表す枝を返す．
  static
  BddEdge
  zero()
  {
    return BddEdge{0UL};
  }

  /// @brief 定数1を表す枝を返す．
  static
  BddEdge
  one()
  {
    return BddEdge{1UL};
  }

  /// @brief 不正値を表す枝を返す．
  static
  BddEdge
  invalid()
  {
    return BddEdge{4UL};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const
  {
    return mBody == 0UL;
  }

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const
  {
    return mBody == 1UL;
  }

  /// @brief 定数の時 true を返す．
  bool
  is_const() const
  {
    return (mBody & ~1UL) == 0UL;
  }

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const
  {
    return mBody == 4UL;
  }

  /// @brief ノードを取り出す．
  BddNode*
  node() const
  {
    return reinterpret_cast<BddNode*>(mBody & ~1UL);
  }

  /// @brief 反転フラグを得る．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1UL);
  }

  /// @brief 本体の値を返す．
  ympuint
  body() const
  {
    return mBody;
  }

  /// @brief ハッシュ値を得る．
  SizeType
  hash() const
  {
    return static_cast<SizeType>((mBody * mBody) >> 20);
  }

  /// @brief 極性を反転させた枝を返す．
  BddEdge
  operator~() const
  {
    return BddEdge{mBody ^ 1UL};
  }

  /// @brief 正極性の枝を返す．
  BddEdge
  posi_edge() const
  {
    return BddEdge{mBody & ~1UL};
  }

  /// @brief 負極性の枝を返す．
  BddEdge
  nega_edge() const
  {
    return BddEdge{mBody | 1UL};
  }

  /// @brief 自身を正極性にする．
  /// @return 自身の参照を返す．
  BddEdge&
  make_positive()
  {
    mBody &= ~1UL;
    return *this;
  }

  /// @brief 自身を負極性にする．
  /// @return 自身の参照を返す．
  BddEdge&
  make_negative()
  {
    mBody |= 1UL;
    return *this;
  }

  /// @brief 極性と掛け合わせる．
  BddEdge
  operator^(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return BddEdge{mBody ^ static_cast<ympuint>(inv)};
  }

  /// @brief 極性と掛け合わせて代入する．
  BddEdge&
  operator^=(
    bool inv ///< [in] 反転フラグ
  )
  {
    mBody ^= static_cast<ympuint>(inv);
    return *this;
  }

  /// @brief 等価比較
  bool
  operator==(
    BddEdge right
  ) const
  {
    return mBody == right.mBody;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  ympuint mBody{0UL};

};

inline
bool
operator!=(
  BddEdge left,
  BddEdge right
)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_BDD

#endif // BDDEDGE_H
