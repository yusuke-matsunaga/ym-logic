#ifndef SOPLIT_H
#define SOPLIT_H

/// @file SopLit.h
/// @brief SopLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  explicit
  SopLit(
    SizeType np = 0,
    SizeType nl = 0
  ) : mNp{np},
      mNl{nl}
  {
  }

  /// @brief ゼロのオブジェクトを作るクラスメソッド
  static
  SopLit
  zero()
  {
    return SopLit{0, 0};
  }

  /// 積項数を得る．
  SizeType
  np() const
  {
    return mNp;
  }

  /// リテラル数を得る．
  SizeType
  nl() const
  {
    return mNl;
  }

  /// 和を取って代入する．
  SopLit&
  operator+=(
    const SopLit& src
  )
  {
    mNp += src.mNp;
    mNl += src.mNl;
    return *this;
  }

  /// 積を取って代入する．
  SopLit&
  operator*=(
    const SopLit& src
  )
  {
    SizeType old_np = mNp;
    mNp *= src.mNp;
    mNl = mNl * src.mNp + src.mNl * old_np;
    return *this;
  }

  /// 2つの積項の和
  SopLit
  operator+(
    const SopLit& src2
  ) const
  {
    return SopLit{mNp + src2.mNp,
		  mNl + src2.mNl};
  }

  /// 2つの積項の積
  SopLit
  operator*(
    const SopLit& src2
  ) const
  {
    return SopLit{mNp * src2.mNp,
		  mNl * src2.mNp + src2.mNl * mNp};
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 積項数
  SizeType mNp;

  // リテラル数
  SizeType mNl;

};

END_NAMESPACE_YM_LOGIC

#endif // SOPLIT_H
