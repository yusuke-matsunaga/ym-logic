
/// @file SopCube.cc
/// @brief SopCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num
) : SopBase{var_num},
    mChunk(_cube_size(), 0ULL)
{
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num,
  Literal lit
) : SopCube{var_num}
{
  _cube_set_literal(mChunk.begin(), lit);
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num,
  const vector<Literal>& lit_list
) : SopCube{var_num}
{
  _cube_set_literals(mChunk.begin(), lit_list);
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num,
  std::initializer_list<Literal>& lit_list
) : SopCube{var_num}
{
  _cube_set_literals(mChunk.begin(), lit_list);
}

// @brief コピーコンストラクタ
SopCube::SopCube(
  const SopCube& src
) : SopBase{src},
    mChunk{src.mChunk}
{
}

// @brief コピー代入演算子
SopCube&
SopCube::operator=(
  const SopCube& src
)
{
  if ( this != &src ) {
    SopBase::operator=(src);
    mChunk = src.mChunk;
  }

  return *this;
}

// @brief ムーブコンストラクタ
SopCube::SopCube(
  SopCube&& src
) : SopBase{src},
    mChunk{src.mChunk}
{
}

// @brief ムーブ代入演算子
SopCube&
SopCube::operator=(
  SopCube&& src
)
{
  SopBase::operator=(src);
  std::swap(mChunk, src.mChunk);

  return *this;
}

// @brief 内容を指定するコンストラクタ
SopCube::SopCube(
  SizeType var_num,
  Chunk&& chunk
) : SopBase{var_num},
    mChunk{chunk}
{
}

// @brief 指定した変数のパタンを読み出す．
SopPat
SopCube::get_pat(
  SizeType var
) const
{
  return _get_pat(mChunk.begin(), var);
}

// @brief リテラル数を返す．
SizeType
SopCube::literal_num() const
{
  if ( mChunk.empty() ) {
    return 0;
  }
  return _literal_num(1, chunk());
}

// @brief 空のキューブの時 true を返す．
bool
SopCube::is_tautology() const
{
  if ( mChunk.empty() ) {
    return true;
  }
  auto cube = _cube_begin(mChunk);
  return _cube_check_null(cube);
}

// @brief 内容をリテラルのリストに変換する．
vector<Literal>
SopCube::literal_list() const
{
  SizeType nl = literal_num();

  vector<Literal> lit_list;
  lit_list.reserve(nl);
  auto src_cube = _cube_begin(mChunk);
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    auto pat = _get_pat(src_cube, var);
    if ( pat == SopPat::_1 ) {
      lit_list.push_back(Literal{var, false});
    }
    else if ( pat == SopPat::_0 ) {
      lit_list.push_back(Literal{var, true});
    }
  }

  return lit_list;
}

// @brief 指定したリテラルを含んでいたら true を返す．
bool
SopCube::check_literal(
  Literal lit
) const
{
  return _literal_num(1, chunk(), lit) > 0;
}

// @brief キューブの論理積を計算する
SopCube
SopCube::operator*(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = _cube_begin(dst_chunk);
  auto cube1 = _cube_begin(chunk());
  auto cube2 = _cube_begin(right.chunk());
  if ( !_cube_product(dst_cube, cube1, cube2) ) {
    _cube_clear(dst_cube);
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief 論理積を計算し自身に代入する．
SopCube&
SopCube::operator*=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_cube = _cube_begin(mChunk);
  auto src_cube = _cube_begin(right.chunk());
  if ( !_cube_product(dst_cube, dst_cube, src_cube) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief キューブとリテラルの論理積を計算する
SopCube
SopCube::operator*(
  Literal right
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = _cube_begin(dst_chunk);
  auto src_cube = _cube_begin(chunk());
  if ( !_cube_product(dst_cube, src_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief リテラルとの論理積を計算し自身に代入する．
SopCube&
SopCube::operator*=(
  Literal right
)
{
  auto dst_cube = _cube_begin(mChunk);
  if ( !_cube_product(dst_cube, dst_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief キューブによる商を計算する
SopCube
SopCube::operator/(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_chunk =_new_chunk(1);
  auto dst_cube = _cube_begin(dst_chunk);
  auto cube1 = _cube_begin(chunk());
  auto cube2 = _cube_begin(right.chunk());
  if ( !_cube_quotient(dst_cube, cube1, cube2) ) {
    _cube_clear(dst_cube);
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief キューブによる商を計算し自身に代入する．
SopCube&
SopCube::operator/=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_cube = _cube_begin(mChunk);
  auto cube2 = _cube_begin(right.chunk());
  if ( !_cube_quotient(dst_cube, dst_cube, cube2) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief リテラルによる商を計算する
SopCube
SopCube::operator/(
  Literal right
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = _cube_begin(dst_chunk);
  auto cube1 = _cube_begin(chunk());
  if ( !_cube_quotient(dst_cube, cube1, right) ) {
    _cube_clear(dst_cube);
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief リテラルによる商を計算し自身に代入する．
SopCube&
SopCube::operator/=(
  Literal right
)
{
  auto dst_cube = _cube_begin(mChunk);
  if ( !_cube_quotient(dst_cube, dst_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @relates SopCube
int
SopCube::_compare(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto cube1 = _cube_begin(chunk());
  auto cube2 = _cube_begin(right.chunk());
  return _cube_compare(cube1, cube2);
}

// @brief Expr に変換する．
Expr
SopCube::expr() const
{
  return _to_expr(1, chunk());
}

// @brief ハッシュ値を返す．
SizeType
SopCube::hash() const
{
  return _hash(1, chunk());
}

// @brief 内容をわかりやすい形で出力する．
void
SopCube::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  _print(s, chunk(), 0, 1, varname_list);
}

END_NAMESPACE_YM_SOP
