#ifndef SOP_H
#define SOP_H

/// @file Sop.h
/// @brief Sop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"


BEGIN_NAMESPACE_YM_LOGIC

/// @brief パタンを表す列挙型
enum class SopPat {
  _X = 0, //@< なし
  _1 = 1, //@< 正極性
  _0 = 2  //@< 負極性
};

/// @brief SopPat をパックしたビットベクタ型
using SopBitVect = ymuint64;

// クラス名の前方参照用宣言
class SopBlock;
class SopLitSet;
class SopMgr;

END_NAMESPACE_YM_LOGIC

#endif // SOP_H
