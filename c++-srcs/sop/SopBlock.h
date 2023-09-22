#ifndef SOPBLOCK_H
#define SOPBLOCK_H

/// @file SopBlock.h
/// @brief SopBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class SopBlock SopBlock.h "SopBlock.h"
/// @brief SopCover の中身
///
/// ただし入力数の情報は持たない．
//////////////////////////////////////////////////////////////////////
struct SopBlock
{

  // キューブ数
  SizeType cube_num;

  // 容量
  SizeType capacity;

  // ブロックの先頭アドレス
  SopBitVect* body;

};

END_NAMESPACE_YM_SOP

#endif // SOPBLOCK_H
