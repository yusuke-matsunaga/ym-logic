﻿#ifndef YM_LITERAL_H
#define YM_LITERAL_H

/// @file ym/Literal.h
/// @brief Literal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016-2017, 2022-2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @ingroup LogicGroup
/// @class Literal Literal.h "ym/Literal.h"
/// @brief リテラル(変数番号＋極性)を表すクラス
/// @sa VarId
//////////////////////////////////////////////////////////////////////
class Literal
{
public:

  /// @brief 不正値を返す．
  static
  Literal
  x()
  {
    return Literal{};
  }

  /// @brief デフォルトコンストラクタ
  ///
  /// 未定義リテラル( = Literal::x())になる．
  Literal() = default;

  /// @brief 変数番号と極性を指定したコンストラクタ
  explicit
  Literal(
    SizeType varid,  ///< [in] 変数番号
    bool inv = false ///< [in] 極性
                     ///   - false: 反転なし (正極性)
                     ///   - true:  反転あり (負極性)
  )
  {
    set(varid, inv);
  }

  /// @brief index からの変換関数
  static
  Literal
  index2literal(
    SizeType index ///< [in] 変数番号と極性をエンコードしたもの
  )
  {
    Literal lit;
    lit.mBody = index;
    return lit;
  }

  // コピーコンストラクタ,代入演算子,デストラクタはデフォルト
  // のものがそのまま使える．


public:
  //////////////////////////////////////////////////////////////////////
  // 内容をセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    SizeType varid,  ///< [in] 変数番号
    bool inv = false ///< [in] 極性
		     ///   - false: 反転なし (正極性)
		     ///   - true:  反転あり (負極性)
  )
  {
    if ( varid == BAD_VARID ) {
      mBody = 0;
    }
    else {
      mBody = (varid << 1) + static_cast<SizeType>(inv);
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mBody != (BAD_VARID << 1);
  }

  /// @brief 変数番号を得る．
  /// @return 変数番号
  SizeType
  varid() const
  {
    if ( is_valid() ) {
      return mBody >> 1;
    }
    else {
      return BAD_VARID;
    }
  }

  /// @brief 正極性のリテラルの時 true を返す．
  bool
  is_positive() const
  {
    return !is_negative();
  }

  /// @brief 負極性のリテラルの時 true を返す．
  bool
  is_negative() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 極性の反転
  /// @return 極性を反転させたリテラルを返す．
  Literal
  operator~() const
  {
    return invert();
  }

  /// @brief 極性の反転の別名
  Literal
  invert() const
  {
    if ( is_valid() ) {
      Literal lit;
      lit.mBody = mBody ^ 1U;
      return lit;
    }
    else {
      return *this;
    }
  }

  /// @brief 同じ変数の正極性リテラルを返す．
  Literal
  make_positive() const
  {
    if ( is_valid() ) {
      Literal lit;
      lit.mBody = mBody & ~1U;
      return lit;
    }
    else {
      return *this;
    }
  }

  /// @brief 同じ変数の負極性リテラルを返す．
  Literal
  make_negative() const
  {
    if ( is_valid() ) {
      Literal lit;
      lit.mBody = mBody | 1U;
      return lit;
    }
    else {
      return *this;
    }
  }

  /// @brief ハッシュ用の関数
  SizeType
  hash() const
  {
    return mBody;
  }

  /// @brief 配列のインデックスとして使用可能な数を返す．
  SizeType
  index() const
  {
    return mBody;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルに出力する．
  void
  dump(
    BinEnc& s ///< [in] 出力先のストリーム
  ) const
  {
    s.write_vint(mBody);
  }

  /// @brief バイナリファイルから読み込む．
  void
  restore(
    BinDec& s ///< [in] 入力元のストリーム
  )
  {
    mBody = s.read_vint();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号と極性をパックしたもの
  SizeType mBody{BAD_VARID << 1};

};

/// @relates Literal
/// @brief 比較関数
/// @retval -1 lit1 < lit2
/// @retval  0 lit1 = lit2
/// @retval  1 lit1 > lit2
inline
int
compare(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  if ( lit1.index() < lit2.index() ) {
    return -1;
  }
  if ( lit1.index() > lit2.index() ) {
    return 1;
  }
  return 0;
}

/// @relates Literal
/// @brief 等価比較
/// @return lit1 と lit2 が等しいリテラルの時 true を返す．
inline
bool
operator==(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  return compare(lit1, lit2) == 0;
}

/// @relates Literal
/// @brief 非等価比較
/// @return lit1 と lit2 が等しいリテラルでないとき true を返す．
inline
bool
operator!=(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  return !operator==(lit1, lit2);
}

/// @relates Literal
/// @brief 小なり比較
/// @return lit1 が lit2 より小さいとき true を返す．
inline
bool
operator<(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  return compare(lit1, lit2) == -1;
}

/// @relates Literal
/// @brief 大なり比較
/// @return lit1 が lit2 より大きいとき true を返す．
inline
bool
operator>(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  return operator<(lit2, lit1);
}

/// @relates Literal
/// @brief 小なりイコール比較
/// @return lit1 が lit2 より小さいか等しいとき true を返す．
inline
bool
operator<=(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  return !operator<(lit2, lit1);
}

/// @relates Literal
/// @brief 大なりイコール比較
/// @return lit1 が lit2 より大きいか等しいとき true を返す．
inline
bool
operator>=(
  Literal lit1, ///< [in] 第1オペランド
  Literal lit2  ///< [in] 第2オペランド
)
{
  return !operator<(lit1, lit2);
}

/// @relates Literal
/// @brief Literal の内容を ostream に出力する関数
/// @return s
inline
std::ostream&
operator<<(
  std::ostream& s,   ///< [in] 出力ストリーム
  const Literal& lit ///< [in] 出力対象のリテラル
)
{
  if ( lit.is_valid() ) {
    if ( lit.is_positive() ) {
      s << "P";
    }
    else {
      s << "N";
    }
    s << "<" << lit.varid() << ">";
  }
  else {
    s << "U<>";
  }
  return s;
}

/// @relates Literal
/// @brief Literal の内容をバイナリ出力する関数
/// @return s
inline
BinEnc&
operator<<(
  BinEnc& s,        ///< [in] 出力ストリーム
  const Literal& lit ///< [in] 象のリテラル
)
{
  lit.dump(s);
  return s;
}

/// @relates Literal
/// @brief Literal の内容をバイナリ入力する関数
/// @return s
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力ストリーム
  Literal& lit ///< [out] 対象のリテラル
)
{
  lit.restore(s);
  return s;
}

END_NAMESPACE_YM


BEGIN_NAMESPACE_STD

// Literal の等価比較オブジェクト
template <>
struct equal_to<YM_NAMESPACE::Literal>
{
  bool
  operator()(
    YM_NAMESPACE::Literal n1,
    YM_NAMESPACE::Literal n2
  ) const
  {
    return n1 == n2;
  }
};

// Literal をキーにしたハッシュ関数クラス
template <>
struct hash<YM_NAMESPACE::Literal>
{
  SizeType
  operator()(
    YM_NAMESPACE::Literal lit
  ) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_LITERAL_H
