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
/// @class SopBlockRef SopBlock.h "SopBlock.h"
/// @brief SopCover/SopCube の中身
///
/// ただし入力数の情報は持たない．
//////////////////////////////////////////////////////////////////////
struct SopBlockRef
{

  // キューブ数
  SizeType cube_num;

  // ビットベクタの本体
  const SopBitVect& body;

};


//////////////////////////////////////////////////////////////////////
/// @class SopBlock SopBlock.h "SopBlock.h"
/// @brief SopCover/SopCube の中身
///
/// ただし入力数の情報は持たない．
//////////////////////////////////////////////////////////////////////
struct SopBlock
{

  // 空のコンストラクタ
  SopBlock() = default;

  // コンストラクタ
  SopBlock(
    SizeType _cube_num,
    SizeType _cube_size
  ) : cube_num{_cube_num},
      body(_cube_num * _cube_size)
  {
  }

  // SopBlockRef からの疑似コピーコンストラクタ
  SopBlock(
    const SopBlockRef& src
  ) : cube_num{src.cube_num},
      body{src.body}
  {
  }

  // キューブ数
  SizeType cube_num{0};

  // ビットベクタの本体
  SopBitVect body;

};

END_NAMESPACE_YM_SOP

#endif // SOPBLOCK_H
