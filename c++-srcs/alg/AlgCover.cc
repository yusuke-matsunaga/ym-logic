
/// @file AlgCover.cc
/// @brief AlgCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"
#include "ym/AlgCube.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num
) : AlgBase{var_num},
    mCubeNum{0}
{
}

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num,
  const vector<AlgCube>& cube_list
) : AlgBase{var_num},
    mCubeNum{cube_list.size()},
    mChunk(_cube_size() * cube_num(), 0ULL)
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

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num,
  const vector<vector<Literal>>& cube_list
) : AlgBase{var_num},
    mCubeNum{cube_list.size()},
    mChunk(_cube_size() * cube_list.size(), 0ULL)
{
  // cube_list の中のリテラル番号が var_num に収まるがチェック
  for ( auto& lits: cube_list ) {
    for ( auto lit: lits ) {
      if ( lit.varid() >= variable_num() ) {
	throw std::out_of_range{"literal is out of range"};
      }
    }
  }

  // mChunk に内容をコピーする．
  auto dst_list = _cube_list(chunk());
  for ( auto& lits: cube_list ) {
    auto dst_cube = dst_list.back();
    _cube_set_literals(dst_cube, lits);
    dst_list.inc();
  }
  _sort();
}

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num,
  std::initializer_list<std::initializer_list<Literal>>& cube_list
) : AlgBase{var_num},
    mCubeNum{cube_list.size()},
    mChunk(_cube_size() * cube_list.size(), 0ULL)
{
  // cube_list の中のリテラル番号が var_num に収まるがチェック
  for ( auto& lits: cube_list ) {
    for ( auto lit: lits ) {
      if ( lit.varid() >= variable_num() ) {
	throw std::out_of_range{"literal is out of range"};
      }
    }
  }

  // mChunk に内容をコピーする．
  auto dst_list = _cube_list(chunk());
  for ( auto& lits: cube_list ) {
    auto dst_cube = dst_list.back();
    _cube_set_literals(dst_cube, lits);
    dst_list.inc();
  }
  _sort();
}

// @brief コピーコンストラクタ
AlgCover::AlgCover(
  const AlgCover& src
) : AlgBase{src.variable_num()},
    mCubeNum{src.mCubeNum},
    mChunk(_cube_size() * cube_num(), 0ULL)
{
  // mChunk に内容をコピーする．
  // mChunk の容量は余分に確保されている可能性があるので
  // _cube_size() に基づいて必要なぶんだけコピーする．
  auto src_begin = src.chunk().begin();
  auto src_end = src_begin + _cube_size() * cube_num();
  auto dst_begin = mChunk.begin();
  std::copy(src_begin, src_end, dst_begin);
}

// @brief コピー代入演算子
AlgCover&
AlgCover::operator=(
  const AlgCover& src
)
{
  if ( this != &src ) {
    AlgBase::operator=(src);
    mCubeNum = src.mCubeNum;
    // mChunk に内容をコピーする．
    // mChunk の容量は余分に確保されている可能性があるので
    // _cube_size() に基づいて必要なぶんだけコピーする．
    SizeType size = _cube_size() * cube_num();
    auto src_begin = src.chunk().begin();
    auto src_end = src_begin + size;
    mChunk.resize(size, 0ULL);
    auto dst_begin = mChunk.begin();
    std::copy(src_begin, src_end, dst_begin);
  }

  return *this;
}

// @brief ムーブコンストラクタ
AlgCover::AlgCover(
  AlgCover&& src
) : AlgBase{src.variable_num()},
    mCubeNum{src.mCubeNum},
    mChunk{std::move(src.mChunk)}
{
  src.mCubeNum = 0;
}

// @brief ムーブ代入演算子
AlgCover&
AlgCover::operator=(
  AlgCover&& src
)
{
  AlgBase::operator=(std::move(src));
  mCubeNum = src.mCubeNum;
  std::swap(mChunk, src.mChunk);
  src.mCubeNum = 0;

  return *this;
}

// @brief キューブからのコピー変換コンストラクタ
AlgCover::AlgCover(
  const AlgCube& cube
) : AlgBase{cube.variable_num()},
    mCubeNum{1},
    mChunk{cube.chunk()}
{
}

// @brief 内容を指定したコンストラクタ
AlgCover::AlgCover(
  SizeType variable_num,
  SizeType cube_num,
  Chunk&& chunk
) : AlgBase{variable_num},
    mCubeNum{cube_num},
    mChunk{std::move(chunk)}
{
}

// @brief リテラル数を返す．
SizeType
AlgCover::literal_num() const
{
  return _literal_num(cube_num(), chunk());
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
AlgCover::literal_num(
  Literal lit
) const
{
  return _literal_num(cube_num(), chunk(), lit);
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
AlgCover::literal_num(
  SizeType varid,
  bool inv
) const
{
  return _literal_num(cube_num(), chunk(), varid, inv);
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
AlgCover::literal_list() const
{
  return _literal_list(cube_num(), chunk());
}

// @brief キューブを取り出す．
AlgCube
AlgCover::get_cube(
  SizeType cube_id
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = _dst_cube(dst_chunk);
  auto src_cube = _cube(chunk(), cube_id);
  _cube_copy(dst_cube, src_cube);
  return AlgCube{variable_num(), std::move(dst_chunk)};
}

// @brief パタンを返す．
AlgPat
AlgCover::get_pat(
  SizeType cube_id,
  SizeType var
) const
{
  auto src_cube = _cube(chunk(), cube_id);
  return _get_pat(src_cube, var);
}

// @brief Expr に変換する．
Expr
AlgCover::expr() const
{
  return _to_expr(cube_num(), chunk());
}

// @brief ハッシュ値を返す．
SizeType
AlgCover::hash() const
{
  return _hash(cube_num(), chunk());
}

// @brief 内容をわかりやすい形で出力する．
void
AlgCover::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  _print(s, chunk(), 0, cube_num(), varname_list);
}

// @brief 内容をデバッグ用に出力する．
void
AlgCover::debug_print(
  ostream& s
) const
{
  _debug_print(s, cube_num(), chunk());
}

END_NAMESPACE_YM_ALG
