#ifndef ALGBLOCK_H
#define ALGBLOCK_H

/// @file AlgBlock.h
/// @brief AlgBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Alg.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class AlgBlock AlgBlock.h "AlgBlock.h"
/// @brief AlgCover/AlgCube に共通なデータ構造
//////////////////////////////////////////////////////////////////////
class AlgBlock
{
public:

  /// @brief コンストラクタ
  /// @param[in] cube_num キューブ数
  /// @param[in] bitvect ビットベクタの先頭アドレス
  AlgBlock(int cube_num,
	   const AlgBitVect* bitvect);

  /// @brief デストラクタ
  ~AlgBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ数を返す．
  int
  cube_num() const;

  /// @brief ビットベクタの先頭アドレスを返す．
  const AlgBitVect*
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
  const AlgBitVect* mBitVect;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cube_num キューブ数
// @param[in] bitvect ビットベクタの先頭アドレス
inline
AlgBlock::AlgBlock(int cube_num,
		   const AlgBitVect* bitvect) :
  mCubeNum{cube_num},
  mBitVect{bitvect}
{
}

// @brief デストラクタ
inline
AlgBlock::~AlgBlock()
{
}

// @brief キューブ数を返す．
inline
int
AlgBlock::cube_num() const
{
  return mCubeNum;
}

// @brief ビットベクタの先頭アドレスを返す．
inline
const AlgBitVect*
AlgBlock::bitvect() const
{
  return mBitVect;
}

END_NAMESPACE_YM_LOGIC

#endif // ALGBLOCK_H
