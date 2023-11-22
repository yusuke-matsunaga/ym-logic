#ifndef ZDDEDGE_H
#define ZDDEDGE_H

/// @file ZddEdge.h
/// @brief ZddEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddNode;

//////////////////////////////////////////////////////////////////////
/// @class ZddEdge ZddEdge.h "ZddEdge.h"
/// @brief ZDD の枝を表すクラス
///
/// 実体はポインタの下位ビットにフラグを埋め込んでいる．
//////////////////////////////////////////////////////////////////////
class ZddEdge
{
public:

  /// @brief コンストラクタ
  explicit
  ZddEdge(
    PtrIntType body = 0UL
  ) : mBody{body}
  {
  }

  /// @brief ノード性を指定したコンストラクタ
  explicit
  ZddEdge(
    ZddNode* node ///< [in] ノード
  ) : mBody{reinterpret_cast<PtrIntType>(node)}
  {
  }

  /// @brief 定数0を表す枝を返す．
  static
  ZddEdge
  zero()
  {
    return ZddEdge{0UL};
  }

  /// @brief 定数1を表す枝を返す．
  static
  ZddEdge
  one()
  {
    return ZddEdge{1UL};
  }

  /// @brief 不正値を表す枝を返す．
  static
  ZddEdge
  invalid()
  {
    return ZddEdge{4UL};
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
  ZddNode*
  node() const
  {
    return reinterpret_cast<ZddNode*>(mBody & ~1UL);
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

  /// @brief 等価比較
  bool
  operator==(
    ZddEdge right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    ZddEdge right
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

END_NAMESPACE_YM_ZDD

BEGIN_NAMESPACE_STD

/// @brief ハッシュ用の関数オブジェクト
template<>
struct hash<nsYm::nsZdd::ZddEdge>
{
  SizeType
  operator()(
    const nsYm::nsZdd::ZddEdge& key
  ) const
  {
    return key.hash();
  }
};

END_NAMESPACE_STD

#endif // ZDDEDGE_H
