#ifndef ALG_H
#define ALG_H

/// @file Alg.h
/// @brief Alg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"


BEGIN_NAMESPACE_YM_LOGIC

/// @brief 極性を表す列挙型
enum class AlgPol {
  X = 0, //@< なし
  P = 1, //@< 正極性
  N = 2  //@< 負極性
};

using AlgBitVect = ymuint64;

class AlgBlock;
class AlgLitSet;
class AlgMgr;

END_NAMESPACE_YM_LOGIC

#endif // ALG_H
