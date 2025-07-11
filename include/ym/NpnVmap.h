#ifndef YM_NPNVMAP_H
#define YM_NPNVMAP_H

/// @file ym/NpnVmap.h
/// @brief NpnVmap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2017, 2019, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class NpnVmap NpnVmap.h "ym/NpnVmap.h"
/// @ingroup LogicGroup
/// @brief 変数の変換を表すクラス
///
/// @sa NpnMap, NpnMapM
//////////////////////////////////////////////////////////////////////
class NpnVmap
{
public:

  /// @brief 空のコンストラクタ．
  ///
  /// 内容は不定
  NpnVmap() : mPosPol{0xFF}
  {
  }

  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] var
  /// @param[in] inv
  explicit
  NpnVmap(
    SizeType var,    ///< [in] 変数番号
    bool inv = false ///< [in] 反転属性
                     ///        - false: 反転なし (正極性)
                     ///        - true:  反転あり (負極性)
  ) : mPosPol((var << 1) | static_cast<std::uint8_t>(inv))
  {
  }

  /// @brief 不正な値を返すクラス関数
  static
  NpnVmap
  invalid()
  {
    // 実はデフォルトコンストラクタを呼ぶだけ．
    // おもに読みやすさのために用意した関数
    return NpnVmap{};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変換先の変数番号を得る．
  SizeType
  var() const
  {
    if ( is_invalid() ) {
      return BAD_VARID;
    }
    else {
      return mPosPol >> 1;
    }
  }

  /// @brief 反転属性を取り出す．
  bool
  inv() const
  {
    return static_cast<bool>(mPosPol & 1U);
  }

  /// @brief 不正な値の時に true を返す．
  bool
  is_invalid() const
  {
    return mPosPol == 0xFF;
  }

  /// @brief 等価比較演算
  bool
  operator==(
    const NpnVmap& right
  ) const
  {
    return mPosPol == right.mPosPol;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    const NpnVmap& right
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ/リストア関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const
  {
    s << mPosPol;
  }

  /// @brief バイナリ入力
  void
  restore(
    std::istream& s ///< [in] 入力ストリーム
  )
  {
    s >> mPosPol;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号と極性をパックしたもの
  std::uint8_t mPosPol;

};

END_NAMESPACE_YM_LOGIC

#endif // YM_NPNVMAP_H
