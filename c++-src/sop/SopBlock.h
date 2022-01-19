#ifndef SOPBLOCK_H
#define SOPBLOCK_H

/// @file SopBlock.h
/// @brief SopBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Sop.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class SopBlock SopBlock.h "SopBlock.h"
/// @brief SopCover/SopCube に共通なデータ構造
//////////////////////////////////////////////////////////////////////
class SopBlock
{
public:

  /// @brief コンストラクタ
  SopBlock(
    SizeType cube_num,        ///< [in] キューブ数
    const SopBitVect* bitvect ///< [in] ビットベクタの先頭アドレス
  ) : mCubeNum{cube_num},
      mBitVect{bitvect}
  {
  }

  /// @brief デストラクタ
  ~SopBlock() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ数を返す．
  SizeType
  cube_num() const
  {
    return mCubeNum;
  }

  /// @brief ビットベクタの先頭アドレスを返す．
  const SopBitVect*
  bitvect() const
  {
    return mBitVect;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブ数
  SizeType mCubeNum;

  // ブロックの先頭
  const SopBitVect* mBitVect;

};

END_NAMESPACE_YM_LOGIC

#endif // SOPBLOCK_H
