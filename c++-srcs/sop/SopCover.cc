
/// @file SopCover.cc
/// @brief SopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num,
  const vector<SopCube>& cube_list
) : SopBase{var_num},
    mCubeNum{cube_list.size()},
    mChunk(_cube_size() * cube_num(), SOP_ALL1)
{
  // cube_list のキューブのサイズが正しいかチェック
  for ( auto& cube: cube_list ) {
    if ( cube.variable_num() != variable_num() ) {
      throw std::invalid_argument{"variable_num of cube mismatch"};
    }
  }

  // mChunk に内容をコピーする．
  auto dst_list = _cube_list(chunk());
  for ( auto& cube: cube_list ) {
    auto src_cube = _cube(cube.chunk());
    auto dst_cube = dst_list.back();
    _cube_copy(dst_cube, src_cube);
    dst_list.inc();
  }
  _sort();
}

// @brief キューブからのコピー変換コンストラクタ
SopCover::SopCover(
  const SopCube& cube
) : SopBase{cube.variable_num()},
    mCubeNum{1},
    mChunk{cube.chunk()}
{
}

// @brief キューブを取り出す．
SopCube
SopCover::get_cube(
  SizeType cube_id
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = SopBase::_dst_cube(dst_chunk);
  auto src_cube = _cube(chunk(), cube_id);
  _cube_copy(dst_cube, src_cube);
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief パタンを返す．
SopPat
SopCover::get_pat(
  SizeType cube_id,
  SizeType var
) const
{
  auto src_cube = _cube(chunk(), cube_id);
  return _get_pat(src_cube, var);
}

END_NAMESPACE_YM_SOP
