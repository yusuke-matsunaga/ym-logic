
/// @file SopCube.cc
/// @brief SopCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCube.h"
#include "SopMgr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num
) : SopBase{var_num},
    mBody(_cube_size(), 0ULL)
{
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num,
  Literal lit
) : SopCube{var_num}
{
  SopMgr mgr{variable_num()};
  mgr.cube_set_literal(mBody.begin(), lit);
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num,
  const vector<Literal>& lit_list
) : SopCube{var_num}
{
  SopMgr mgr{variable_num()};
  mgr.cube_set_literals(mBody.begin(), lit_list);
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType var_num,
  std::initializer_list<Literal>& lit_list
) : SopCube{var_num}
{
  SopMgr mgr{variable_num()};
  mgr.cube_set_literals(mBody.begin(), lit_list);
}

// @brief コピーコンストラクタ
SopCube::SopCube(
  const SopCube& src
) : SopBase{src},
    mBody{src.mBody}
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
    mBody = src.mBody;
  }

  return *this;
}

// @brief ムーブコンストラクタ
SopCube::SopCube(
  SopCube&& src
) : SopBase{src},
    mBody{src.mBody}
{
}

// @brief ムーブ代入演算子
SopCube&
SopCube::operator=(
  SopCube&& src
)
{
  SopBase::operator=(src);
  std::swap(mBody, src.mBody);

  return *this;
}

// @brief 内容を指定するコンストラクタ
SopCube::SopCube(
  SizeType var_num,
  SopBitVect&& body
) : SopBase{var_num},
    mBody{body}
{
}

// @brief 指定した変数のパタンを読み出す．
SopPat
SopCube::get_pat(
  SizeType var
) const
{
  SopMgr mgr{variable_num()};
  return mgr.get_pat(mBody.begin(), var);
}

// @brief リテラル数を返す．
SizeType
SopCube::literal_num() const
{
  if ( mBody.empty() ) {
    return 0;
  }
  SopMgr mgr(variable_num());
  return mgr.literal_num(to_block());
}

// @brief 空のキューブの時 true を返す．
bool
SopCube::is_tautology() const
{
  if ( mBody.empty() ) {
    return true;
  }
  SopMgr mgr{variable_num()};
  return mgr.cube_check_null(mBody.begin());
}

// @brief 内容をリテラルのリストに変換する．
vector<Literal>
SopCube::literal_list() const
{
  SopMgr mgr{variable_num()};
  SizeType nl = mgr.literal_num(to_block());

  auto src_cube = mBody.begin();
  vector<Literal> lit_list;
  lit_list.reserve(nl);
  for ( SizeType var = 0; var < variable_num(); ++ var ) {
    auto pat = mgr.get_pat(src_cube, var);
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
  SopMgr mgr{variable_num()};
  return mgr.literal_num(to_block(), lit) > 0;
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
  SopMgr mgr{variable_num()};
  auto dst_bv = mgr.new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src1_cube = _cube_begin(mBody);
  auto src2_cube = _cube_begin(right.mBody);
  if ( !mgr.cube_product(dst_cube, src1_cube, src2_cube) ) {
    mgr.cube_clear(dst_cube);
  }
  return mgr.make_cube(std::move(dst_bv));
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
  SopMgr mgr{variable_num()};
  auto dst_cube = _cube_begin(mBody);
  auto src_cube = _cube_begin(right.mBody);
  if ( !mgr.cube_product(dst_cube, dst_cube, src_cube) ) {
    mgr.cube_clear(dst_cube);
  }

  return *this;
}

// @brief キューブとリテラルの論理積を計算する
SopCube
SopCube::operator*(
  Literal right
) const
{
  SopMgr mgr{variable_num()};
  auto dst_bv = mgr.new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src_cube = _cube_begin(mBody);
  if ( !mgr.cube_product(dst_cube, src_cube, right) ) {
    mgr.cube_clear(dst_cube);
  }

  return mgr.make_cube(std::move(dst_bv));
}

// @brief リテラルとの論理積を計算し自身に代入する．
SopCube&
SopCube::operator*=(
  Literal right
)
{
  SopMgr mgr{variable_num()};
  auto dst_cube = _cube_begin(mBody);
  if ( !mgr.cube_product(dst_cube, dst_cube, right) ) {
    mgr.cube_clear(dst_cube);
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

  SopMgr mgr{variable_num()};
  auto dst_bv = mgr.new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src1_cube = _cube_begin(mBody);
  auto src2_cube = _cube_begin(right.mBody);
  if ( !mgr.cube_quotient(dst_cube, src1_cube, src2_cube) ) {
    mgr.cube_clear(dst_cube);
  }

  return mgr.make_cube(std::move(dst_bv));
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

  SopMgr mgr{variable_num()};
  auto dst_cube = _cube_begin(mBody);
  auto src2_cube = _cube_begin(right.mBody);
  if ( !mgr.cube_quotient(dst_cube, dst_cube, src2_cube) ) {
    mgr.cube_clear(dst_cube);
  }

  return *this;
}

// @brief リテラルによる商を計算する
SopCube
SopCube::operator/(
  Literal right
) const
{
  SopMgr mgr{variable_num()};
  auto dst_bv = mgr.new_cube();
  auto dst_cube = _cube_begin(dst_bv);
  auto src1_cube = _cube_begin(mBody);
  if ( !mgr.cube_quotient(dst_cube, src1_cube, right) ) {
    mgr.cube_clear(dst_cube);
  }

  return mgr.make_cube(std::move(dst_bv));
}

// @brief リテラルによる商を計算し自身に代入する．
SopCube&
SopCube::operator/=(
  Literal right
)
{
  SopMgr mgr{variable_num()};
  auto dst_cube = _cube_begin(mBody);
  if ( !mgr.cube_quotient(dst_cube, dst_cube, right) ) {
    mgr.cube_clear(dst_cube);
  }

  return *this;
}

// @relates SopCube
int
compare(
  const SopCube& left,
  const SopCube& right
)
{
  if ( left.variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{left.variable_num()};
  auto cube1 = left._cube_begin(left.mBody);
  auto cube2 = right._cube_begin(right.mBody);
  return mgr.cube_compare(cube1, cube2);
}

// @brief Expr に変換する．
Expr
SopCube::expr() const
{
  SopMgr mgr{variable_num()};
  return mgr.to_expr(to_block());
}

// @brief SopBlockRef に変換する．
SopBlockRef
SopCube::to_block() const
{
  return SopBlockRef{1, mBody};
}

// @brief ハッシュ値を返す．
SizeType
SopCube::hash() const
{
  SopMgr mgr{variable_num()};
  return mgr.hash(to_block());
}

// @brief 内容をわかりやすい形で出力する．
void
SopCube::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  SopMgr mgr(variable_num());
  mgr.print(s, mBody, 0, 1, varname_list);
}

END_NAMESPACE_YM_SOP
