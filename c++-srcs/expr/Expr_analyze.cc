
/// @file FuncAnalyzer.cc
/// @brief FuncAnalyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief 与えられた論理式が組み込み型かどうか判定する．
//
// 組み込み型でない場合には BuiltinType::None が返される．
BuiltinType
Expr::analyze() const
{
  // 単純な場合のチェック
  if ( is_zero() ) {
    return BuiltinType::C0;
  }
  if ( is_one() ) {
    return BuiltinType::C1;
  }
  if ( is_posi_literal() ) {
    return BuiltinType::Buff;
  }
  if ( is_nega_literal() ) {
    return BuiltinType::Not;
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
    if ( expr1.is_posi_literal() ) {
      ++ phase;
    }
    else { // is_nega_literal()
      -- phase;
      parity ^= 1;
    }
  }
  if ( ok ) {
    SizeType n = operand_num();
    if ( is_and() ) {
      if ( phase == n ) {
	return BuiltinType::And;
      }
      if ( phase == -n ) {
	return BuiltinType::Nor;
      }
    }
    else if ( is_or() ) {
      if ( phase == n ) {
	return BuiltinType::Or;
      }
      else if ( phase == -n ) {
	return BuiltinType::Nand;
      }
    }
    else if ( is_xor() ) {
      if ( parity == 0 ) {
	return BuiltinType::Xor;
      }
      return BuiltinType::Xnor;
    }
  }

  // ここまで来たら複雑な形の式
  auto input_num = input_size();
  if ( input_num <= 10 ) {
    // 10入力以下の場合は一旦 TvFunc に変換する．
    auto tv = make_tv(input_num);
    return tv.analyze();
  }

  return BuiltinType::None;
}

END_NAMESPACE_YM_LOGIC
