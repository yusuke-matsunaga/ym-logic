
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
    mChunk(_cube_size(), SOP_ALL1)
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
) : SopBase{src.variable_num()},
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
) : SopBase{src.variable_num()},
    mChunk{std::move(src.mChunk)}
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
  auto cube = _cube();
  return _cube_check_null(cube);
}

// @brief 内容をリテラルのリストに変換する．
vector<Literal>
SopCube::literal_list() const
{
  SizeType nl = literal_num();

  vector<Literal> lit_list;
  lit_list.reserve(nl);
  auto src_cube = _cube();
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
  SizeType varid,
  bool inv
) const
{
  auto blk = _block_pos(varid);
  auto mask = _get_mask(varid, inv);
  auto word = _get_word(_cube(), blk);
  if ( (word | mask) == mask ) {
    return true;
  }
  return false;
}

// @brief キューブの論理積を計算する
SopCube
SopCube::operator&(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = SopBase::_dst_cube(dst_chunk);
  auto cube1 = _cube();
  auto cube2 = right._cube();
  if ( !_cube_product(dst_cube, cube1, cube2) ) {
    _cube_clear(dst_cube);
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief 論理積を計算し自身に代入する．
SopCube&
SopCube::operator&=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto dst_cube = _dst_cube();
  auto src_cube = right._cube();
  if ( !_cube_product_int(dst_cube, src_cube) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief キューブとリテラルの論理積を計算する
SopCube
SopCube::operator&(
  Literal right
) const
{
  auto dst_chunk = _new_chunk(1);
  auto dst_cube = SopBase::_dst_cube(dst_chunk);
  auto src_cube = _cube();
  if ( !_cube_product(dst_cube, src_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return SopCube{variable_num(), std::move(dst_chunk)};
}

// @brief リテラルとの論理積を計算し自身に代入する．
SopCube&
SopCube::operator&=(
  Literal right
)
{
  auto dst_cube = _dst_cube();
  if ( !_cube_product_int(dst_cube, right) ) {
    _cube_clear(dst_cube);
  }
  return *this;
}

// @brief オペランドのキューブに含まれていたら true を返す．
bool
SopCube::check_containment(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }
  auto cube1 = _cube();
  auto end1 = _cube_end(cube1);
  auto cube2 = right._cube();
  for ( ; cube1 != end1; ++ cube1, ++ cube2 ) {
    auto pat1 = *cube1;
    auto pat2 = *cube2;
    if ( (pat1 & pat2) != pat1 ) {
      return false;
    }
  }
  return true;
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
  auto cube1 = _cube();
  auto cube2 = right._cube();
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

// @brief 内部で用いられるキューブを得る．
SopBase::Cube
SopCube::_cube() const
{
  return SopBase::_cube(chunk());
}

// @brief 内部で用いられるキューブを得る．
SopBase::DstCube
SopCube::_dst_cube()
{
  return SopBase::_dst_cube(chunk());
}

END_NAMESPACE_YM_SOP
