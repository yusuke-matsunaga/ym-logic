#ifndef YM_VARID_H
#define YM_VARID_H

/// @file ym/VarId.h
/// @brief VarId の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2017, 2018, 2019, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class VarId VarId.h "ym/VarId.h"
/// @ingroup LogicGroup
/// @brief 変数番号を表す型
///
/// 基本的にはただの符号なし整数だが，意味のある演算がほとんどないので
/// あえてクラスの形にしている．
/// 例えば変数どうしの四則演算に直接的な意味はない．
///
/// 不正値として -1 を符号なしに変換した値を仮定している．
//////////////////////////////////////////////////////////////////////
class VarId
{
public:

  /// @brief 不正な値を返す．
  static
  VarId
  illegal()
  {
    return VarId();
  }


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値( = VarId::illegal())になる．
  VarId() = default;

  /// @brief 値を指定したコンストラクタ
  ///
  /// val は数値だが同じ数値の変数が同じ変数を表す
  /// という以外に意味はない．
  explicit
  VarId(
    SizeType val ///< [in] 変数番号
  ) : mVal{val}
  {
  }

  /// @brief デストラクタ
  ~VarId() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外から使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正しい値の時に true を返す．
  bool
  is_valid() const
  {
    return mVal != static_cast<SizeType>(-1);
  }

  /// @brief 値を取り出す．
  SizeType
  val() const
  {
    return mVal;
  }

  /// @brief 比較関数
  /// @retval -1 left < right
  /// @retval  0 left = right
  /// @retval  1 left > right
  friend
  int
  compare(
    const VarId& left, ///< [in] 第1オペランド
    const VarId& right ///< [in] 第2オペランド
  )
  {
    if ( left.mVal < right.mVal ) {
      return -1;
    }
    if ( left.mVal > right.mVal ) {
      return 1;
    }
    return 0;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルに出力する．
  void
  dump(
    BinEnc& s ///< [in] 出力先のストリーム
  ) const
  {
    s.write_vint(mVal);
  }

  /// @brief バイナリファイルを読み込む．
  void
  restore(
    BinDec& s ///< [in] 入力ストリーム
  )
  {
    mVal = s.read_vint();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の値
  SizeType mVal{static_cast<SizeType>(-1)};

};

/// @brief 等価比較
inline
bool
operator==(
  const VarId& left, ///< [in] 第1オペランド
  const VarId& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == 0;
}

/// @brief 非等価比較
inline
bool
operator!=(
  const VarId& left, ///< [in] 第1オペランド
  const VarId& right ///< [in] 第2オペランド
)
{
  return !operator==(left, right);
}

/// @brief 小なり比較
inline
bool
operator<(
  const VarId& left, ///< [in] 第1オペランド
  const VarId& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == -1;
}

/// @brief 大なり比較
inline
bool
operator>(
  const VarId& left, ///< [in] 第1オペランド
  const VarId& right ///< [in] 第2オペランド
)
{
  return operator<(right, left);
}

/// @brief 小なりイコール比較
inline
bool
operator<=(
  const VarId& left, ///< [in] 第1オペランド
  const VarId& right ///< [in] 第2オペランド
)
{
  return !operator<(right, left);
}

/// @brief 大なりイコール比較
inline
bool
operator>=(
  const VarId& left, ///< [in] 第1オペランド
  const VarId& right ///< [in] 第2オペランド
)
{
  return !operator<(left, right);
}

/// @relates VarId
/// @brief ストリームに対する出力
inline
ostream&
operator<<(
  ostream& s,        ///< [in] 出力先のストリーム
  const VarId& varid ///< [in] 対象の変数
)
{
  if ( varid.is_valid() ) {
    s << "V_" << varid.val();
  }
  else {
    s << "---";
  }
  return s;
}

/// @relates VarId
/// @brief バイナリ出力
inline
BinEnc&
operator<<(
  BinEnc& s,        ///< [in] 出力先のストリーム
  const VarId& varid ///< [in] 対象の変数
)
{
  varid.dump(s);
  return s;
}

/// @relates VarId
/// @brief バイナリ入力
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 出力先のストリーム
  VarId& varid ///< [out] 対象の変数
)
{
  varid.restore(s);
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// VarId の等価比較オブジェクト
template <>
struct equal_to<YM_NAMESPACE::VarId>
{
  bool
  operator()(
    YM_NAMESPACE::VarId n1,
    YM_NAMESPACE::VarId n2
  ) const
  {
    return n1 == n2;
  }
};


//////////////////////////////////////////////////////////////////////
// hash<VarId> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
hash<YM_NAMESPACE::VarId>
{
  SizeType
  operator()(
    YM_NAMESPACE::VarId key
  ) const {
    return static_cast<SizeType>(key.val());
  }
};

END_NAMESPACE_STD

#endif // YM_VARID_H
