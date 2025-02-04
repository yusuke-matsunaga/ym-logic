
/// @file AlgCube.cc
/// @brief AlgCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_ALG

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num
) : AlgBase{var_num},
    mChunk(_cube_size(), 0ULL)
{
}

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  Literal lit
) : AlgCube{var_num}
{
  _cube_set_literal(_dst_cube(), lit);
}

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  const vector<Literal>& lit_list
) : AlgCube{var_num}
{
  _cube_set_literals(_dst_cube(), lit_list);
}

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  std::initializer_list<Literal>& lit_list
) : AlgCube{var_num}
{
  _cube_set_literals(_dst_cube(), lit_list);
}

// @brief コピーコンストラクタ
AlgCube::AlgCube(
  const AlgCube& src
) : AlgBase{src},
    mChunk{src.mChunk}
{
}

// @brief コピー代入演算子
AlgCube&
AlgCube::operator=(
  const AlgCube& src
)
{
  if ( this != &src ) {
    AlgBase::operator=(src);
    mChunk = src.mChunk;
  }
  return *this;
}

// @brief ムーブコンストラクタ
AlgCube::AlgCube(
  AlgCube&& src
) : AlgBase{std::move(src)},
    mChunk{std::move(src.mChunk)}
{
}

// @brief ムーブ代入演算子
AlgCube&
AlgCube::operator=(
  AlgCube&& src
)
{
  AlgBase::operator=(std::move(src));
  std::swap(mChunk, src.mChunk);
  return *this;
}

// @brief 内容を指定するコンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  Chunk&& chunk
) : AlgBase{var_num},
    mChunk{std::move(chunk)}
{
}

// @brief 指定した変数のパタンを読み出す．
AlgPat
AlgCube::get_pat(
  SizeType var
) const
{
  return _get_pat(_cube(), var);
}

// @brief リテラル数を返す．
SizeType
AlgCube::literal_num() const
{
  if ( mChunk.empty() ) {
    return 0;
  }
  return _literal_num(1, chunk());
}

// @brief 空のキューブの時 true を返す．
bool
AlgCube::is_tautology() const
{
  if ( mChunk.empty() ) {
    return true;
  }
  return _cube_check_null(_cube());
}

// @brief 内容をリテラルのリストに変換する．
vector<Literal>
AlgCube::literal_list() const
{
  vector<Literal> lit_list;
  SizeType nl = literal_num();
  lit_list.reserve(nl);
  auto src_cube = _cube();
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    auto pat = _get_pat(src_cube, var);
    if ( pat == AlgPat::_1 ) {
      lit_list.push_back(Literal{var, false});
    }
    else if ( pat == AlgPat::_0 ) {
      lit_list.push_back(Literal{var, true});
    }
  }
  return lit_list;
}

// @brief 指定したリテラルを含んでいたら true を返す．
bool
AlgCube::check_literal(
  Literal lit
) const
{
  return _literal_num(1, chunk(), lit) > 0;
}

// @brief キューブの論理積を計算する
AlgCube
AlgCube::operator*(
  const AlgCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = dst_chunk.begin();
  auto cube1 = _cube();
  auto cube2 = right._cube();
  if ( !_cube_product(dst_cube, cube1, cube2) ) {
    _cube_clear(dst_cube);
  }
  return AlgCube{variable_num(), std::move(dst_chunk)};
}

// @brief 論理積を計算し自身に代入する．
AlgCube&
AlgCube::operator*=(
  const AlgCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_cube = _dst_cube();
  auto src_cube = right._cube();
  if ( !_cube_product(dst_cube, dst_cube, src_cube) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief キューブとリテラルの論理積を計算する
AlgCube
AlgCube::operator*(
  Literal right
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = AlgBase::_dst_cube(dst_chunk);
  auto src_cube = _cube();
  if ( !_cube_product(dst_cube, src_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return AlgCube{variable_num(), std::move(dst_chunk)};
}

// @brief リテラルとの論理積を計算し自身に代入する．
AlgCube&
AlgCube::operator*=(
  Literal right
)
{
  auto dst_cube = _dst_cube();
  if ( !_cube_product(dst_cube, dst_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief キューブによる商を計算する
AlgCube
AlgCube::operator/(
  const AlgCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  auto dst_chunk = _new_chunk(1);
  auto dst_cube = AlgBase::_dst_cube(dst_chunk);
  auto cube1 = _cube();
  auto cube2 = right._cube();
  if ( !_cube_quotient(dst_cube, cube1, cube2) ) {
    _cube_clear(dst_cube);
  }
  return AlgCube{variable_num(), std::move(dst_chunk)};
}

// @brief キューブによる商を計算し自身に代入する．
AlgCube&
AlgCube::operator/=(
  const AlgCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  auto dst_cube = _dst_cube();
  auto cube2 = right._cube();
  if ( !_cube_quotient(dst_cube, dst_cube, cube2) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief リテラルによる商を計算する
AlgCube
AlgCube::operator/(
  Literal right
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = AlgBase::_dst_cube(dst_chunk);
  auto cube1 = _cube();
  if ( !_cube_quotient(dst_cube, cube1, right) ) {
    _cube_clear(dst_cube);
  }
  return AlgCube{variable_num(), std::move(dst_chunk)};
}

// @brief リテラルによる商を計算し自身に代入する．
AlgCube&
AlgCube::operator/=(
  Literal right
)
{
  auto dst_cube = _dst_cube();
  if ( !_cube_quotient(dst_cube, dst_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief Expr に変換する．
Expr
AlgCube::expr() const
{
  return _to_expr(1, chunk());
}

// @brief ハッシュ値を返す．
SizeType
AlgCube::hash() const
{
  return _hash(1, chunk());
}

// @brief 内容をわかりやすい形で出力する．
void
AlgCube::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  _print(s, chunk(), 0, 1, varname_list);
}

// @brief 内部で用いられるキューブを得る．
AlgBase::Cube
AlgCube::_cube() const
{
  return AlgBase::_cube(chunk());
}

// @brief 内部で用いられるキューブを得る．
AlgBase::DstCube
AlgCube::_dst_cube()
{
  return AlgBase::_dst_cube(chunk());
}

// @relates AlgCube
int
AlgCube::_compare(
  const AlgCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  auto cube1 = _cube();
  auto cube2 = right._cube();
  return _cube_compare(cube1, cube2);
}

END_NAMESPACE_YM_ALG
