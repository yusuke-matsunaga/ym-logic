
/// @file FuncAnalyzer.cc
/// @brief FuncAnalyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM

// @brief 与えられた真理値表が組み込み型かどうか判定する．
//
// 組み込み型でない場合には PrimType::None が返される．
PrimType
TvFunc::analyze() const
{
  auto map = shrink_map();
  auto func1 = xform(map);

  if ( func1 == TvFunc::zero(0) ) {
    return PrimType::C0;
  }
  else if ( func1 == TvFunc::one(0) ) {
    return PrimType::C1;
  }
  else if ( func1 == TvFunc::posi_literal(1, 0) ) {
    return PrimType::Buff;
  }
  else if ( func1 == TvFunc::nega_literal(1, 0) ) {
    return PrimType::Not;
  }
  else {
    auto input_num = func1.input_num();
    auto np = 1UL << input_num;
    int val_0;
    int val_1;
    bool has_0 = false;
    bool has_1 = false;
    bool xor_match = true;
    bool xnor_match = true;
    for ( auto p = 0; p < np; ++ p ) {
      int val = func1.value(p);
      if ( p == 0UL ) {
	// 00...00 の時の値
	val_0 = val;
      }
      else if ( p == (np - 1) ) {
	// 11...11 の時の値
	val_1 = val;
      }
      else {
	if ( val == 0 ) {
	  // 少なくとも1つは0になった．
	  has_0 = true;
	}
	else {
	  // 少なくとも1つは1になった．
	  has_1 = true;
	}
      }

      // p のパリティを計算する．
      bool parity = false;
      for ( auto i = 0; i < input_num; ++ i ) {
	if ( (1UL << i) & p ) {
	  parity = !parity;
	}
      }
      if ( parity ) {
	if ( val == 1 ) {
	  xnor_match = false;
	}
	else {
	  xor_match = false;
	}
      }
      else {
	if ( val == 1 ) {
	  xor_match = false;
	}
	else {
	  xnor_match = false;
	}
      }
    }
    if ( val_0 == 0 && val_1 == 1 ) {
      if ( !has_0 ) {
	// 00...00 だけ 0 で残りが 1
	return PrimType::Or;
      }
      else if ( !has_1 ) {
	// 11...11 だけ 1 で残りが 0
	return PrimType::And;
      }
    }
    if ( val_0 == 1 && val_1 == 0 ) {
      if ( !has_0 ) {
	// 11...11 だけ 0 で残りが 1
	return PrimType::Nand;
      }
      else if ( !has_1 ) {
	// 00...00 だけ 1 で残りが 0
	return PrimType::Nor;
      }
    }
    if ( xor_match ) {
      return PrimType::Xor;
    }
    else if ( xnor_match ) {
      return PrimType::Xnor;
    }
  }

  return PrimType::None;
}

END_NAMESPACE_YM
