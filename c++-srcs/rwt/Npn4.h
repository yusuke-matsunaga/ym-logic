#ifndef NPN4_H
#define NPN4_H

/// @file Npn4.h
/// @brief Npn4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class Npn4 Npn4.h "Npn4.h"
/// @brief 4入力関数のNPN変換を表すクラス
///
/// - 出力の反転の有無
/// - 入力の反転の有無
/// - 入力変数の順列
/// を持つ．
///
/// 変換としてはまず，入出力の反転を行った後に順列の入れ替えを行うものとする．
///
/// 全部で 2^1 x 2^4 x 4! = 2 x 16 x 24 = 768 通りしかない．
/// 緩めのパッキングでも10ビットで表せる．
/// 演算は表引きで行う．
//////////////////////////////////////////////////////////////////////
class Npn4
{
public:

  /// @brief 4入力関数を表す真理値表のパタン
  using Tv4 = std::uint16_t;

public:

  /// @brief コンストラクタ
  ///
  /// 恒等変換となる．
  Npn4() = default;

  /// @brief 内容を指定したコンストラクタ
  ///
  /// iinv, iperm が不正な時は std::invalid_argument 例外を送出する．
  Npn4(
    bool oinv,                    ///< [in] 出力の反転属性
    const vector<bool>& iinv,     ///< [in] 入力の反転属性(4つ)
    const vector<SizeType>& iperm ///< [in] 入力の順列
  );

  /// @brief 恒等変換を返すクラスメソッド
  static
  Npn4
  identity()
  {
    return Npn4();
  }

  /// @brief デストラクタ
  ~Npn4() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の反転属性を返す．
  bool
  oinv() const
  {
    return static_cast<bool>((mChunk >> 9) & 1);
  }

  /// @brief 入力の反転属性を返す．
  bool
  iinv(
    SizeType pos ///< [in] 入力番号 (0 <= pos < 4)
  ) const
  {
    if ( pos < 0 || 4 <= pos ) {
      std::out_of_range{"pos is out of range"};
    }
    return static_cast<bool>((mChunk >> (pos + 5)) & 1);
  }

  /// @brief 入力の置換結果を返す．
  /// @return pos 番目の入力の置換先の番号を返す．
  SizeType
  iperm(
    SizeType pos ///< [in] 入力番号 (0 <= pos < 4)
  ) const;

  /// @brief 関数のNPN変換結果を求める．
  Tv4
  xform(
    Tv4 tv ///< [in] 関数の真理値表パタン
  ) const;

  /// @brief xform() の別名
  Tv4
  operator()(
    Tv4 tv ///< [in] 関数の真理値表パタン
  ) const
  {
    return xform(tv);
  }

  /// @brief 正規化を行う．
  static
  Tv4
  normalize(
    Tv4 tv,   ///< [in] 元の関数
    Npn4& npn ///< [out] 正規化用の変換
  );

  /// @brief サポートを求める．
  /// @return サポートを表すビットベクタを返す．
  ///
  /// iビット目が1の時，i番目の入力がサポートになっていることを示す．
  std::uint8_t
  get_support(
    Tv4 tv ///< [in] 対象の関数
  );

  /// @brief 逆変換を返す．
  Npn4
  operator~() const;

  /// @brief 合成を返す．
  Npn4
  operator*(
    const Npn4& right
  ) const;

  /// @brief 等価比較演算
  bool
  operator==(
    const Npn4& right
  ) const
  {
    return mChunk == right.mChunk;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    const Npn4& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接したコンストラクタ
  Npn4(
    std::uint16_t chunk
  ) : mChunk{chunk}
  {
  }

  /// @brief インデックスを返す．
  std::uint8_t
  _index() const
  {
    return static_cast<std::uint8_t>(mChunk & 31);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべての情報をパックしたもの
  std::uint16_t mChunk{0};

};

/// @brief ストリーム出力
inline
std::ostream&
operator<<(
  std::ostream& s,
  const Npn4& npn
)
{
  npn.print(s);
  return s;
}

END_NAMESPACE_YM_AIG

#endif // NPN4_H
