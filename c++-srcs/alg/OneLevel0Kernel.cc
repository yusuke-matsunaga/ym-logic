
/// @file OneLevel0Kernel.cc
/// @brief OneLevel0Kernel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "OneLevel0Kernel.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス OneLevel0Kernel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OneLevel0Kernel::OneLevel0Kernel()
{
}

// @brief デストラクタ
OneLevel0Kernel::~OneLevel0Kernel()
{
}

BEGIN_NONAMESPACE

// f 中で２回以上現れるリテラルを求める．
// なければ Literal::x() を返す．
inline
Literal
find_literal(
  const AlgCover& f
)
{
  int nv = f.variable_num();
  for ( int var = 0; var < nv; ++ var ) {
    for ( auto lit: {Literal(var, false), Literal(var, true)} ) {
      int n = f.literal_num(lit);
      if ( n >= 2 ) {
	return lit;
      }
    }
  }
  return Literal::x();
}

END_NONAMESPACE

// @brief 除数を求める．
AlgCover
OneLevel0Kernel::operator()(
  const AlgCover& f
) const
{
  if ( f.cube_num() < 2 ) {
    // f をこれ以上割ることはできない．
    // 空の論理式を返す．
    return AlgCover{f.variable_num()};
  }

  // f に2回以上現れるリテラルを求める．
  auto lit = find_literal(f);
  if ( lit == Literal::x() ) {
    // f をこれ以上割ることはできない．
    // 空の論理式を返す．
    return AlgCover{f.variable_num()};
  }

  auto f1 = f;
  do {
    f1 /= lit;
    auto cc = f1.common_cube();
    f1 /= cc;
    lit = find_literal(f1);
  } while ( lit != Literal::x() );
  return f1;
}

END_NAMESPACE_YM_ALG
