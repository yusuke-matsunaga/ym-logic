
/// @file AlgCube.cc
/// @brief AlgCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCube.h"
#include "AlgMgr.h"
#include "AlgBlock.h"


BEGIN_NAMESPACE_YM_ALG

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num
) : AlgBase{var_num},
    mBody(_cube_size(), 0ULL)
{
}

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  Literal lit
) : AlgCube{var_num}
{
  cube_set_literal(mBody.begin(), lit);
}

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  const vector<Literal>& lit_list
) : AlgCube{var_num}
{
  cube_set_literals(mBody.begin(), lit_list);
}

// @brief コンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  std::initializer_list<Literal>& lit_list
) : AlgCube{var_num}
{
  cube_set_literals(mBody.begin(), lit_list);
}

// @brief コピーコンストラクタ
AlgCube::AlgCube(
  const AlgCube& src
) : AlgBase{src},
    mBody{src.mBody}
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
    mBody = src.mBody;
  }

  return *this;
}

// @brief ムーブコンストラクタ
AlgCube::AlgCube(
  AlgCube&& src
) : AlgBase{std::move(src)},
    mBody{std::move(src.mBody)}
{
}

// @brief ムーブ代入演算子
AlgCube&
AlgCube::operator=(
  AlgCube&& src
)
{
  AlgBase::operator=(std::move(src));
  std::swap(mBody, src.mBody);

  return *this;
}

// @brief 内容を指定するコンストラクタ
AlgCube::AlgCube(
  SizeType var_num,
  AlgBitVect&& body
) : AlgBase{var_num},
    mBody{std::move(body)}
{
}

// @brief 指定した変数のパタンを読み出す．
AlgPat
AlgCube::get_pat(
  SizeType var
) const
{
  return get_pat(mBody.begin(), var);
}

// @brief リテラル数を返す．
SizeType
AlgCube::literal_num() const
{
  if ( mBody.empty() ) {
    return 0;
  }
  return literal_num(to_block());
}

// @brief 空のキューブの時 true を返す．
bool
AlgCube::is_tautology() const
{
  if ( mBody.empty() ) {
    return true;
  }
  return cube_check_null(mBody.begin());
}

// @brief 内容をリテラルのリストに変換する．
vector<Literal>
AlgCube::literal_list() const
{
  SizeType nl = literal_num();

  auto src_cube = mBody.begin();
  vector<Literal> lit_list;
  lit_list.reserve(nl);
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    auto pat = get_pat(src_cube, var);
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
  return literal_num(to_block(), lit) > 0;
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
  auto dst_bv = new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src1_cube = _cube_begin(mBody);
  auto src2_cube = _cube_begin(right.mBody);
  if ( !cube_product(dst_cube, src1_cube, src2_cube) ) {
    cube_clear(dst_cube);
  }
  return make_cube(std::move(dst_bv));
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
  auto dst_cube = _cube_begin(mBody);
  auto src_cube = _cube_begin(right.mBody);
  if ( !cube_product(dst_cube, dst_cube, src_cube) ) {
    cube_clear(dst_cube);
  }

  return *this;
}

// @brief キューブとリテラルの論理積を計算する
AlgCube
AlgCube::operator*(
  Literal right
) const
{
  auto dst_bv = new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src_cube = _cube_begin(mBody);
  if ( !cube_product(dst_cube, src_cube, right) ) {
    cube_clear(dst_cube);
  }

  return make_cube(std::move(dst_bv));
}

// @brief リテラルとの論理積を計算し自身に代入する．
AlgCube&
AlgCube::operator*=(
  Literal right
)
{
  auto dst_cube = _cube_begin(mBody);
  if ( !cube_product(dst_cube, dst_cube, right) ) {
    cube_clear(dst_cube);
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

  auto dst_bv = new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src1_cube = _cube_begin(mBody);
  auto src2_cube = _cube_begin(right.mBody);
  if ( !cube_quotient(dst_cube, src1_cube, src2_cube) ) {
    cube_clear(dst_cube);
  }

  return make_cube(std::move(dst_bv));
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

  auto dst_cube = _cube_begin(mBody);
  auto src2_cube = _cube_begin(right.mBody);
  if ( !cube_quotient(dst_cube, dst_cube, src2_cube) ) {
    cube_clear(dst_cube);
  }

  return *this;
}

// @brief リテラルによる商を計算する
AlgCube
AlgCube::operator/(
  Literal right
) const
{
  auto dst_bv = new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src1_cube = _cube_begin(mBody);
  if ( !cube_quotient(dst_cube, src1_cube, right) ) {
    cube_clear(dst_cube);
  }

  return make_cube(std::move(dst_bv));
}

// @brief リテラルによる商を計算し自身に代入する．
AlgCube&
AlgCube::operator/=(
  Literal right
)
{
  auto dst_cube = _cube_begin(mBody);
  if ( !cube_quotient(dst_cube, dst_cube, right) ) {
    cube_clear(dst_cube);
  }

  return *this;
}

// @relates AlgCube
int
compare(
  const AlgCube& left,
  const AlgCube& right
)
{
  if ( left.variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  auto cube1 = left._cube_begin(left.mBody);
  auto cube2 = right._cube_begin(right.mBody);
  return cube_compare(cube1, cube2);
}

// @brief Expr に変換する．
Expr
AlgCube::expr() const
{
  return to_expr(to_block());
}

// @brief AlgBlockRef に変換する．
AlgBlockRef
AlgCube::to_block() const
{
  return AlgBlockRef{1, mBody};
}

// @brief ハッシュ値を返す．
SizeType
AlgCube::hash() const
{
  return hash(to_block());
}

// @brief 内容をわかりやすい形で出力する．
void
AlgCube::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  print(s, mBody, 0, 1, varname_list);
}

END_NAMESPACE_YM_ALG
