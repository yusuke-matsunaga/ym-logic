
/// @file FuncAnalyzer.cc
/// @brief FuncAnalyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief 与えられた論理式が組み込み型かどうか判定する．
//
// 組み込み型でない場合には PrimType::None が返される．
PrimType
Expr::analyze() const
{
  // 単純な場合のチェック
  if ( is_zero() ) {
    return PrimType::C0;
  }
  if ( is_one() ) {
    return PrimType::C1;
  }
  if ( is_positive_literal() ) {
    return PrimType::Buff;
  }
  if ( is_negative_literal() ) {
    return PrimType::Not;
  }
  // 全てのオペランドが同じ曲性のリテラル
  // かどうか調べる．
  int phase = 0;
  int parity = 0;
  bool ok = true;
  for ( auto expr1: operand_list() ) {
    if ( !expr1.is_literal() ) {
      ok = false;
      break;
    }
    if ( expr1.is_positive_literal() ) {
      ++ phase;
    }
    else { // is_negative_literal()
      -- phase;
      parity ^= 1;
    }
  }
  if ( ok ) {
    SizeType n = operand_num();
    if ( is_and() ) {
      if ( phase == n ) {
	return PrimType::And;
      }
      if ( phase == -n ) {
	return PrimType::Nor;
      }
    }
    else if ( is_or() ) {
      if ( phase == n ) {
	return PrimType::Or;
      }
      else if ( phase == -n ) {
	return PrimType::Nand;
      }
    }
    else if ( is_xor() ) {
      if ( parity == 0 ) {
	return PrimType::Xor;
      }
      return PrimType::Xnor;
    }
  }

  // ここまで来たら複雑な形の式
  auto input_num = input_size();
  if ( input_num <= 10 ) {
    // 10入力以下の場合は一旦 TvFunc に変換する．
    auto tv = tvfunc(input_num);
    return tv.analyze();
  }

  return PrimType::None;
}

END_NAMESPACE_YM_LOGIC
