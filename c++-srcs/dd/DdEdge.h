#ifndef DDEDGE_H
#define DDEDGE_H

/// @file DdEdge.h
/// @brief DdEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

class DdNode;

//////////////////////////////////////////////////////////////////////
/// @class DdEdge DdEdge.h "DdEdge.h"
/// @brief BDD の枝を表すクラス
///
/// 実体はポインタの下位ビットにフラグを埋め込んでいる．
//////////////////////////////////////////////////////////////////////
class DdEdge
{
public:

  /// @brief コンストラクタ
  explicit
  DdEdge(
    PtrIntType body = 0UL
  ) : mBody{body}
  {
  }

  /// @brief ノードと極性を指定したコンストラクタ
  explicit
  DdEdge(
    const DdNode* node, ///< [in] ノード
    bool inv = false    ///< [in] 反転フラグ
  ) : mBody{reinterpret_cast<PtrIntType>(node) | static_cast<PtrIntType>(inv)}
  {
  }

  /// @brief 定数0を表す枝を返す．
  static
  DdEdge
  zero()
  {
    return DdEdge{0UL};
  }

  /// @brief 定数1を表す枝を返す．
  static
  DdEdge
  one()
  {
    return DdEdge{1UL};
  }

  /// @brief 不正値を表す枝を返す．
  static
  DdEdge
  invalid()
  {
    return DdEdge{4UL};
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
  const DdNode*
  node() const
  {
    return reinterpret_cast<const DdNode*>(mBody & ~1UL);
  }

  /// @brief 反転フラグを得る．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1UL);
  }

  /// @brief 本体の値を返す．
  PtrIntType
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
  DdEdge
  operator~() const
  {
    return DdEdge{mBody ^ 1UL};
  }

  /// @brief 正極性の枝を返す．
  DdEdge
  posi_edge() const
  {
    return DdEdge{mBody & ~1UL};
  }

  /// @brief 負極性の枝を返す．
  DdEdge
  nega_edge() const
  {
    return DdEdge{mBody | 1UL};
  }

  /// @brief 自身を正極性にする．
  /// @return 自身の参照を返す．
  DdEdge&
  make_positive()
  {
    mBody &= ~1UL;
    return *this;
  }

  /// @brief 自身を負極性にする．
  /// @return 自身の参照を返す．
  DdEdge&
  make_negative()
  {
    mBody |= 1UL;
    return *this;
  }

  /// @brief 極性と掛け合わせる．
  DdEdge
  operator^(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return DdEdge{mBody ^ static_cast<PtrIntType>(inv)};
  }

  /// @brief 極性と掛け合わせて代入する．
  DdEdge&
  operator^=(
    bool inv ///< [in] 反転フラグ
  )
  {
    mBody ^= static_cast<PtrIntType>(inv);
    return *this;
  }

  /// @brief 等価比較
  bool
  operator==(
    DdEdge right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    DdEdge right
  )
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  PtrIntType mBody{0UL};

};

/// @brief root を根とするDDの内容を出力する(デバッグ用)．
extern
void
print(
  ostream& s, ///< [in] 出力ストリーム
  DdEdge root ///< [in] 根の枝
);

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

/// @brief ハッシュ用の関数オブジェクト
template<>
struct hash<nsYm::nsDd::DdEdge>
{
  SizeType
  operator()(
    const nsYm::nsDd::DdEdge& key
  ) const
  {
    return key.hash();
  }
};

END_NAMESPACE_STD

#endif // DDEDGE_H
