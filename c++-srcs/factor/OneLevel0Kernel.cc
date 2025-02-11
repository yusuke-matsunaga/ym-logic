
/// @file OneLevel0Kernel.cc
/// @brief OneLevel0Kernel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "OneLevel0Kernel.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス OneLevel0Kernel
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// f 中で２回以上現れるリテラルを求める．
// なければ Literal::x() を返す．
inline
Literal
find_literal(
  const SopCover& f
)
{
  for ( auto& lits: f.literal_list() ) {
    for ( auto lit: lits ) {
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
SopCover
OneLevel0Kernel::divisor(
  const SopCover& f
)
{
  if ( f.cube_num() < 2 ) {
    // f をこれ以上割ることはできない．
    // 空の論理式を返す．
    return SopCover{f.variable_num()};
  }

  // f に2回以上現れるリテラルを求める．
  auto lit = find_literal(f);
  if ( lit == Literal::x() ) {
    // f をこれ以上割ることはできない．
    // 空の論理式を返す．
    return SopCover{f.variable_num()};
  }

  auto f1 = f;
  do {
    f1.algdiv_int(lit);
    auto cc = f1.common_cube();
    f1.algdiv_int(cc);
    lit = find_literal(f1);
  } while ( lit != Literal::x() );
  return f1;
}

END_NAMESPACE_YM_SOP
