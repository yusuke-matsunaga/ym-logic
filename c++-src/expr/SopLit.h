#ifndef SOPLIT_H
#define SOPLIT_H

/// @file SopLit.h
/// @brief SopLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// リテラル数見積り用のクラス
/// 具体的には積項数とリテラル数のペア
//////////////////////////////////////////////////////////////////////
class SopLit
{
public:
  /// コンストラクタ
  SopLit(int np = 0,
	 int nl = 0);

  /// 積項数を得る．
  int
  np() const;

  /// リテラル数を得る．
  int
  nl() const;

  /// 和を取って代入する．
  const SopLit&
  operator+=(const SopLit& src);

  /// 積を取って代入する．
  const SopLit&
  operator*=(const SopLit& src);

  /// 2つの積項の和
  friend
  SopLit
  operator+(const SopLit& src1,
	    const SopLit& src2);

  /// 2つの積項の積
  friend
  SopLit
  operator*(const SopLit& src1,
	    const SopLit& src2);

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 積項数
  int mNp;

  // リテラル数
  int mNl;
};



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
SopLit::SopLit(int np,
	       int nl) :
  mNp(np),
  mNl(nl)
{
}

inline
int
SopLit::np() const
{
  return mNp;
}

inline
int
SopLit::nl() const
{
  return mNl;
}

inline
const SopLit&
SopLit::operator+=(const SopLit& src)
{
  mNp += src.mNp;
  mNl += src.mNl;
  return *this;
}

inline
const SopLit&
SopLit::operator*=(const SopLit& src)
{
  int old_np = mNp;
  mNp *= src.mNp;
  mNl = mNl * src.mNp + src.mNl * old_np;
  return *this;
}

inline
SopLit
operator+(const SopLit& src1,
	  const SopLit& src2)
{
  return SopLit(src1.mNp + src2.mNp, src1.mNl + src2.mNl);
}

inline
SopLit
operator*(const SopLit& src1,
	  const SopLit& src2)
{
  return SopLit(src1.mNp * src2.mNp,
		src1.mNl * src2.mNp + src2.mNl * src1.mNp);
}

END_NAMESPACE_YM_LOGIC

#endif // SOPLIT_H
