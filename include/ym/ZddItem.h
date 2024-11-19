#ifndef ZDDITEM_H
#define ZDDITEM_H

/// @file ZddItem.h
/// @brief ZddItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Zdd.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class ZddItem ZddItem.h "ZddItem.h"
/// @brief ZDD の要素を表すクラス
///
/// - 実際にはシングルトンを表す ZDD
//////////////////////////////////////////////////////////////////////
class ZddItem :
  public Zdd
{
  friend class ZddMgrImpl;
  friend class Zdd;
private:

  /// @brief 内容を指定したコンストラクタ
  ///
  /// root はシングルトンでなければならない．
  ZddItem(
    ZddMgrImpl* mgr, ///< [in] マネージャ
    DdEdge root      ///< [in] 根の枝
  );

  /// @brief Zdd からのコピーコンストラクタ
  ///
  /// src はシングルトンでなければならない．
  ZddItem(
    const Zdd& src ///< [in] コピー元のオブジェクト
  ) : Zdd{src}
  {
    if ( !src.is_singleton() ) {
      throw std::invalid_argument{"src is not a singleton"};
    }
  }


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  ZddItem() = default;

  // コピーコンストラクタとコピー代入演算子はデフォルト実装で OK

  /// @brief デストラクタ
  ~ZddItem() = default;

  /// @brief 不正値を作るクラスメソッド
  static
  ZddItem
  invalid()
  {
    return ZddItem{};
  }

  /// @breif Zdd からの変換関数
  ///
  /// - zdd.is_singleton() == true の時のみ成功する．
  /// - zdd.is_singleton() == false の時は ZddItem::invalid() が返される．
  static
  ZddItem
  from_zdd(
    const Zdd& zdd ///< [in] コピー元のオブジェクト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素番号を返す．
  SizeType
  id() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const ZddItem& right
  ) const
  {
    return Zdd::operator==(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ZddItem& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルを返す．
  SizeType
  level() const;

};


/// @brief ストリーム出力演算子
inline
ostream&
operator<<(
  ostream& s,
  const ZddItem& item
)
{
  s << "I" << item.id();
  return s;
}

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// ZddItem をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::ZddItem>
{
  SizeType
  operator()(
    const YM_NAMESPACE::ZddItem& item
  ) const
  {
    return item.hash();
  }
};

END_NAMESPACE_STD

#endif // ZDDITEM_H
