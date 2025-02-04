#ifndef ALGSORTER_H
#define ALGSORTER_H

/// @file AlgSorter.h
/// @brief AlgSorter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/AlgBase.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
/// @class AlgSorter AlgSorter.h "AlgSorter.h"
/// @brief AlgCover をソートするためのクラス
///
/// 一時バッファを持つ．
//////////////////////////////////////////////////////////////////////
class AlgSorter :
  public AlgBase
{
public:

  /// @brief コンストラクタ
  AlgSorter(
    SizeType var_num ///< [in] 変数の数
  ) : AlgBase{var_num}
  {
  }

  /// @brief デストラクタ
  ~AlgSorter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ソートする．
  void
  sort(
    SizeType cube_num, ///< [in] キューブ数
    Chunk& chunk       ///< [in] 対象のビットベクタ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief sort() の下請け関数
  ///
  /// chunk の [begin, end) の区間をソートする．
  /// begin は含むが end は含まないことに注意
  void
  sort_sub(
    Chunk& chunk,   ///< [in] 対象のビットベクタ
    SizeType begin, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
  void
  _cube_swap(
    DstCube cube1, ///< [in] 1つめのキューブの位置
    DstCube cube2  ///< [in] 2つめのキューブの位置
  )
  {
    _cube_save(cube1);
    _cube_copy(cube1, cube2);
    _cube_restore(cube2);
  }

  /// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// 1 <- 2, 2 <- 3, 3 <- 1 となる．
  void
  _cube_rotate3(
    DstCube cube1, ///< [in] 1つめのキューブの位置
    DstCube cube2, ///< [in] 2つめのキューブの位置
    DstCube cube3  ///< [in] 3つめのキューブの位置
  )
  {
    _cube_save(cube1);
    _cube_copy(cube1, cube2);
    _cube_copy(cube2, cube3);
    _cube_restore(cube3);
  }

  /// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// 1 <- 2, 2 <- 3, 3 <- 4, 4 <- 1 となる．
  void
  _cube_rotate4(
    DstCube cube1, ///< [in] 1つめのキューブの位置
    DstCube cube2, ///< [in] 2つめのキューブの位置
    DstCube cube3, ///< [in] 3つめのキューブの位置
    DstCube cube4  ///< [in] 4つめのキューブの位置
  )
  {
    _cube_save(cube1);
    _cube_copy(cube1, cube2);
    _cube_copy(cube2, cube3);
    _cube_copy(cube3, cube4);
    _cube_restore(cube4);
  }

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  void
  _cube_save(
    Cube src_cube ///< [in] ソースのビットベクタ
  )
  {
    auto dst_cube = mTmpChunk.begin();
    _cube_copy(dst_cube, src_cube);
  }

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  void
  _cube_restore(
    DstCube dst_cube ///< [in] コピー先のビットベクタ
  )
  {
    auto src_cube = mTmpChunk.begin();
    _cube_copy(dst_cube, src_cube);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一時的に用いられるバッファ
  Chunk mTmpChunk;

};

END_NAMESPACE_YM_ALG

#endif // ALGSORTER_H
