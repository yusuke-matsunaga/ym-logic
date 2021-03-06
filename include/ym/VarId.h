﻿#ifndef YM_VARID_H
#define YM_VARID_H

/// @file ym/VarId.h
/// @brief VarId の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2017, 2018, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


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
/// 不正値として -1 を仮定しているが，実際には負数はすべて不正値となる．
//////////////////////////////////////////////////////////////////////
class VarId
{
public:

  /// @brief 不正な値を返す．
  static
  VarId
  illegal();

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値( = VarId::illegal())になる．
  VarId() = default;

  /// @brief 値を指定したコンストラクタ
  /// @param[in] val 変数番号
  ///
  /// val は数値だが同じ数値の変数が同じ変数を表す
  /// という以外に意味はない．
  explicit
  VarId(int val);

  /// @brief デストラクタ
  ~VarId() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外から使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正しい値の時に true を返す．
  bool
  is_valid() const;

  /// @brief 値を取り出す．
  int
  val() const;

  /// @brief 比較関数
  /// @param[in] left, right 比較対象の変数
  /// @retval -1 left < right
  /// @retval  0 left = right
  /// @retval  1 left > right
  friend
  int
  compare(const VarId& left,
	  const VarId& right);


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルに出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  /// @brief バイナリファイルを読み込む．
  void
  restore(istream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の値
  int mVal{-1};

};

/// @brief 等価比較
/// @param[in] left, right 比較対象の変数
bool
operator==(const VarId& left,
	   const VarId& right);

/// @brief 非等価比較
/// @param[in] left, right 比較対象の変数
bool
operator!=(const VarId& left,
	   const VarId& right);

/// @brief 小なり比較
/// @param[in] left, right 比較対象の変数
bool
operator<(const VarId& left,
	  const VarId& right);

/// @brief 大なり比較
/// @param[in] left, right 比較対象の変数
bool
operator>(const VarId& left,
	  const VarId& right);

/// @brief 小なりイコール比較
/// @param[in] left, right 比較対象の変数
bool
operator<=(const VarId& left,
	   const VarId& right);

/// @brief 大なりイコール比較
/// @param[in] left, right 比較対象の変数
bool
operator>=(const VarId& left,
	   const VarId& right);

/// @relates VarId
/// @brief ストリームに対する出力
/// @param[in] s 出力先のストリーム
/// @param[in] varid 対象の変数
ostream&
operator<<(ostream& s,
	   const VarId& varid);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 不正な値を返す．
inline
VarId
VarId::illegal()
{
  return VarId();
}

// @brief 値を指定したコンストラクタ
inline
VarId::VarId(int val) :
  mVal{val}
{
  // 負数は不正値に直しておく．
  if ( mVal < 0 ) {
    mVal = -1;
  }
}

// @brief 正しい値の時に true を返す．
inline
bool
VarId::is_valid() const
{
  return mVal >= 0;
}

// @brief 値を取り出す．
inline
int
VarId::val() const
{
  return mVal;
}

// @brief 比較関数
// @param[in] left, right 比較対象の変数
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
inline
int
compare(const VarId& left,
	const VarId& right)
{
  if ( left.mVal < right.mVal ) {
    return -1;
  }
  if ( left.mVal > right.mVal ) {
    return 1;
  }
  return 0;
}

// @brief 等価比較
// @param[in] left, right 比較対象の変数
inline
bool
operator==(const VarId& left,
	   const VarId& right)
{
  return compare(left, right) == 0;
}

// @brief 非等価比較
// @param[in] left, right 比較対象の変数
inline
bool
operator!=(const VarId& left,
	   const VarId& right)
{
  return !operator==(left, right);
}

// @brief 小なり比較
// @param[in] left, right 比較対象の変数
inline
bool
operator<(const VarId& left,
	  const VarId& right)
{
  return compare(left, right) == -1;
}

// @brief 大なり比較
// @param[in] left, right 比較対象の変数
inline
bool
operator>(const VarId& left,
	  const VarId& right)
{
  return operator<(right, left);
}

// @brief 小なりイコール比較
// @param[in] left, right 比較対象の変数
inline
bool
operator<=(const VarId& left,
	   const VarId& right)
{
  return !operator<(right, left);
}

// @brief 大なりイコール比較
// @param[in] left, right 比較対象の変数
inline
bool
operator>=(const VarId& left,
	   const VarId& right)
{
  return !operator<(left, right);
}

// @brief バイナリファイルに出力する．
// @param[in] s 出力先のストリーム
inline
void
VarId::dump(ostream& s) const
{
  s.write(reinterpret_cast<const char*>(&mVal), sizeof(mVal));
}

// @brief バイナリファイルを読み込む．
inline
void
VarId::restore(istream& s)
{
  s.read(reinterpret_cast<char*>(&mVal), sizeof(mVal));
}

// @relates VarId
// @brief ストリームに対する出力
// @param[in] s 出力先のストリーム
// @param[in] varid 対象の変数
inline
ostream&
operator<<(ostream& s,
	   const VarId& varid)
{
  if ( varid.is_valid() ) {
    s << "V_" << varid.val();
  }
  else {
    s << "---";
  }
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// VarId の等価比較オブジェクト
template <>
struct equal_to<YM_NAMESPACE::VarId>
{
  bool
  operator()(YM_NAMESPACE::VarId n1,
	     YM_NAMESPACE::VarId n2) const
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
  operator()(YM_NAMESPACE::VarId key) const {
    return static_cast<SizeType>(key.val());
  }
};

END_NAMESPACE_STD

#endif // YM_VARID_H
