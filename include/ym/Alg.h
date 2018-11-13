#ifndef ALG_H
#define ALG_H

/// @file Alg.h
/// @brief Alg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"


/// @brief Algサブパッケージ用の名前空間の開始
#define BEGIN_NAMESPACE_YM_ALG \
BEGIN_NAMESPACE_YM_LOGIC \
BEGIN_NAMESPACE(nsAlg)

/// @brief Algサブパッケージ用の名前空間の終了
#define END_NAMESPACE_YM_ALG \
END_NAMESPACE(nsAlg) \
END_NAMESPACE_YM_LOGIC


BEGIN_NAMESPACE_YM_ALG

class AlgCover;
class AlgCube;
class AlgLitSet;
class AlgMgr;

using AlgBitVect = ymuint64;

END_NAMESPACE_YM_ALG

BEGIN_NAMESPACE_YM

/// @brief 極性を表す列挙型
enum class AlgPol {
  X = 0, //@< なし
  P = 1, //@< 正極性
  N = 2  //@< 負極性
};

using nsLogic::nsAlg::AlgCover;
using nsLogic::nsAlg::AlgCube;
using nsLogic::nsAlg::AlgLitSet;
using nsLogic::nsAlg::AlgMgr;

END_NAMESPACE_YM

#endif // ALG_H
