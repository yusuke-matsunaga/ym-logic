#ifndef SOPBLOCK_H
#define SOPBLOCK_H

/// @file SopBlock.h
/// @brief SopBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
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
  /// @param[in] cube_num キューブ数
  /// @param[in] bitvect ビットベクタの先頭アドレス
  SopBlock(int cube_num,
	   const SopBitVect* bitvect);

  /// @brief デストラクタ
  ~SopBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ数を返す．
  int
  cube_num() const;

  /// @brief ビットベクタの先頭アドレスを返す．
  const SopBitVect*
  bitvect() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブ数
  int mCubeNum;

  // ブロックの先頭
  const SopBitVect* mBitVect;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cube_num キューブ数
// @param[in] bitvect ビットベクタの先頭アドレス
inline
SopBlock::SopBlock(int cube_num,
		   const SopBitVect* bitvect) :
  mCubeNum{cube_num},
  mBitVect{bitvect}
{
}

// @brief デストラクタ
inline
SopBlock::~SopBlock()
{
}

// @brief キューブ数を返す．
inline
int
SopBlock::cube_num() const
{
  return mCubeNum;
}

// @brief ビットベクタの先頭アドレスを返す．
inline
const SopBitVect*
SopBlock::bitvect() const
{
  return mBitVect;
}

END_NAMESPACE_YM_LOGIC

#endif // SOPBLOCK_H
