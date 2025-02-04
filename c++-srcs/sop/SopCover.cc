
/// @file SopCover.cc
/// @brief SopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num
) : SopBase{var_num},
    mCubeNum{0}
{
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num,
  const vector<SopCube>& cube_list
) : SopBase{var_num},
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
  auto dst_cube = _cube_begin(mChunk);
  for ( auto& cube: cube_list ) {
    auto src_cube = _cube_begin(cube.chunk());
    _cube_copy(dst_cube, src_cube);
    _cube_next(dst_cube);
  }
  _sort();
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num,
  const vector<vector<Literal>>& cube_list
) : SopBase{var_num},
    mCubeNum{cube_list.size()},
    mChunk(_cube_size() * cube_num(), 0ULL)
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
  auto dst_cube = _cube_begin(mChunk);
  for ( auto& lits: cube_list ) {
    _cube_set_literals(dst_cube, lits);
    _cube_next(dst_cube);
  }
  _sort();

}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num,
  std::initializer_list<std::initializer_list<Literal>>& cube_list
) : SopBase{var_num},
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
  auto dst_cube = _cube_begin(mChunk);
  for ( auto& lits: cube_list ) {
    _cube_set_literals(dst_cube, lits);
    _cube_next(dst_cube);
  }
  _sort();
}

// @brief コピーコンストラクタ
SopCover::SopCover(
  const SopCover& src
) : SopBase{src.variable_num()},
    mCubeNum{src.mCubeNum},
    mChunk{src.mChunk}
{
}

// @brief コピー代入演算子
SopCover&
SopCover::operator=(
  const SopCover& src
)
{
  if ( this != &src ) {
    SopBase::operator=(src);
    mCubeNum = src.mCubeNum;
    mChunk = src.mChunk;
  }

  return *this;
}

// @brief ムーブコンストラクタ
SopCover::SopCover(
  SopCover&& src
) : SopBase{src},
    mCubeNum{src.mCubeNum},
    mChunk{std::move(src.mChunk)}
{
}

// @brief ムーブ代入演算子
SopCover&
SopCover::operator=(
  SopCover&& src
)
{
  SopBase::operator=(src);
  mCubeNum = src.mCubeNum;
  std::swap(mChunk, src.mChunk);

  return *this;
}

// @brief キューブからのコピー変換コンストラクタ
SopCover::SopCover(
  const SopCube& cube
) : SopBase{cube.variable_num()},
    mCubeNum{1},
    mChunk{cube.chunk()}
{
}

#if 0
// @brief キューブからのムーブ変換コンストラクタ
SopCover::SopCover(
  SopCube&& cube
) : mVariableNum{cube.variable_num()},
    mCubeNum{1},
    mChunk{std::move(cube.mChunk)}
{
}
#endif

// @brief 内容を指定したコンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  SizeType cube_num,
  Chunk&& chunk
) : SopBase{variable_num},
    mCubeNum{cube_num},
    mChunk{std::move(chunk)}
{
}

// @brief リテラル数を返す．
SizeType
SopCover::literal_num() const
{
  return _literal_num(cube_num(), chunk());
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
SopCover::literal_num(
  Literal lit
) const
{
  return _literal_num(cube_num(), chunk(), lit);
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
SopCover::literal_num(
  SizeType varid,
  bool inv
) const
{
  return _literal_num(cube_num(), chunk(), varid, inv);
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
SopCover::literal_list() const
{
  return _literal_list(cube_num(), chunk());
}

// @brief キューブを取り出す．
SopCube
SopCover::get_cube(
  SizeType cube_id
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = _cube_begin(dst_chunk);
  auto src_cube = _cube_begin(chunk(), cube_id);
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
  auto src_cube = _cube_begin(mChunk, cube_id);
  return _get_pat(src_cube, var);
}

// @brief Expr に変換する．
Expr
SopCover::expr() const
{
  return _to_expr(cube_num(), chunk());
}

// @brief ハッシュ値を返す．
SizeType
SopCover::hash() const
{
  return _hash(cube_num(), chunk());
}

// @brief 内容をわかりやすい形で出力する．
void
SopCover::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  _print(s, mChunk, 0, mCubeNum, varname_list);
}

// @brief 内容をデバッグ用に出力する．
void
SopCover::debug_print(
  ostream& s
) const
{
  _debug_print(s, cube_num(), chunk());
}

END_NAMESPACE_YM_SOP
