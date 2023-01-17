
/// @file SopCube.cc
/// @brief SopCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCube.h"
#include "SopBlock.h"
#include "SopMgr.h"
#include "LitSet.h"


BEGIN_NAMESPACE_YM_LOGIC

// @brief コンストラクタ
// @param[in] variable_num 変数の数
//
// * 空のキューブを作る．
SopCube::SopCube(
  SizeType variable_num
) : mVariableNum{variable_num}
{
  SopMgr mgr{mVariableNum};
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType variable_num,
  Literal lit
) : mVariableNum{variable_num}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, vector<Literal>{lit});
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType variable_num,
  const vector<Literal>& lit_list
) : mVariableNum{variable_num}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, lit_list);
}

// @brief コンストラクタ
SopCube::SopCube(
  SizeType variable_num,
  std::initializer_list<Literal>& lit_list
) : mVariableNum{variable_num}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, lit_list);
}

// @brief コピーコンストラクタ
SopCube::SopCube(
  const SopCube& src
) : mVariableNum{src.mVariableNum}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_copy(mBody, src.mBody);
}

// @brief コピー代入演算子
SopCube&
SopCube::operator=(
  const SopCube& src
)
{
  SopMgr mgr(src.mVariableNum);
  if ( mVariableNum != src.mVariableNum ) {
    delete_body();
    mVariableNum = src.mVariableNum;
    mBody = mgr.new_body(1);
  }
  mgr.cube_copy(mBody, src.mBody);

  return *this;
}

// @brief ムーブコンストラクタ
SopCube::SopCube(
  SopCube&& src
) : mVariableNum{src.mVariableNum},
    mBody{src.mBody}
{
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
SopCube&
SopCube::operator=(
  SopCube&& src
)
{
  delete_body();

  mVariableNum = src.mVariableNum;
  mBody = src.mBody;

  src.mBody = nullptr;

  return *this;
}

// @brief 内容を指定するコンストラクタ
SopCube::SopCube(
  SizeType variable_num,
  SopBitVect* body
) : mVariableNum{variable_num},
    mBody{body}
{
}

// @brief デストラクタ
SopCube::~SopCube()
{
  delete_body();
}

// @brief mBody を削除する．
void
SopCube::delete_body()
{
  if ( mBody != nullptr ) {
    SopMgr mgr(mVariableNum);
    mgr.delete_body(mBody, 1);
  }
}

// @brief 指定した変数のパタンを読み出す．
SopPat
SopCube::get_pat(
  SizeType var
) const
{
  SopMgr mgr(mVariableNum);
  return mgr.get_pat(mBody, 0, var);
}

// @brief リテラル数を返す．
SizeType
SopCube::literal_num() const
{
  if ( mBody == nullptr ) {
    return 0;
  }
  SopMgr mgr(mVariableNum);
  return mgr.literal_num(block());
}

// @brief 内容をリテラルのリストに変換する．
vector<Literal>
SopCube::to_literal_list() const
{
  SopMgr mgr(mVariableNum);
  SizeType nl = mgr.literal_num(block());

  vector<Literal> lit_list;
  lit_list.reserve(nl);
  for ( SizeType var = 0; var < mVariableNum; ++ var ) {
    auto pat = mgr.get_pat(mBody, 0, var);
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
SopCube::has_literal(
  Literal lit
) const
{
  SopMgr mgr(mVariableNum);
  return mgr.literal_num(block(), lit) > 0;
}

// @brief オペランドのキューブに含まれていたら true を返す．
bool
SopCube::check_containment(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  return mgr.cube_check_containment(mBody, right.mBody);
}

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
bool
SopCube::check_intersect(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  return mgr.cube_check_intersect(mBody, right.mBody);
}

// @brief キューブの論理積を計算する
SopCube
SopCube::operator*(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SopBitVect* body = mgr.new_body(1);
  bool stat = mgr.cube_product(body, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(body);
  }

  return SopCube(mVariableNum, body);
}

// @brief 論理積を計算し自身に代入する．
SopCube&
SopCube::operator*=(
  const SopCube& right
)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  bool stat = mgr.cube_product(mBody, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(mBody);
  }

  return *this;
}

// @brief キューブとリテラルの論理積を計算する
SopCube
SopCube::operator*(
  Literal right
) const
{
  SopMgr mgr(mVariableNum);
  SopBitVect* body = mgr.new_body(1);
  int nc = mgr.cover_product(body, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(body);
  }

  return SopCube(mVariableNum, body);
}

// @brief リテラルとの論理積を計算し自身に代入する．
SopCube&
SopCube::operator*=(
  Literal right
)
{
  SopMgr mgr(mVariableNum);
  int nc = mgr.cover_product(mBody, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(mBody);
  }

  return *this;
}

// @brief キューブによる商を計算する
SopCube
SopCube::operator/(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SopBitVect* body = mgr.new_body(1);
  bool stat = mgr.cube_quotient(body, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(body);
  }

  return SopCube(mVariableNum, body);
}

// @brief キューブによる商を計算し自身に代入する．
SopCube&
SopCube::operator/=(
  const SopCube& right
)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  bool stat = mgr.cube_quotient(mBody, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(mBody);
  }

  return *this;
}

// @brief リテラルによる商を計算する
SopCube
SopCube::operator/(
  Literal right
) const
{
  SopMgr mgr(mVariableNum);
  SopBitVect* body = mgr.new_body(1);
  int nc = mgr.cover_quotient(body, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(body);
  }

  return SopCube(mVariableNum, body);
}

// @brief リテラルによる商を計算し自身に代入する．
SopCube&
SopCube::operator/=(
  Literal right
)
{
  SopMgr mgr(mVariableNum);
  int nc = mgr.cover_quotient(mBody, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(mBody);
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
  ASSERT_COND( left.variable_num() == right.variable_num() );

  SopMgr mgr(left.variable_num());
  return mgr.cube_compare(left.mBody, right.mBody);
}

// @brief Expr に変換する．
Expr
SopCube::to_expr() const
{
  SopMgr mgr{mVariableNum};
  return mgr.to_expr(mBody, 1);
}

// @breif SopBlock を返す．
SopBlock
SopCube::block() const
{
  return SopBlock(1, mBody);
}

// @brief ハッシュ値を返す．
SizeType
SopCube::hash() const
{
  SopMgr mgr(mVariableNum);
  return mgr.hash(block());
}

// @brief 内容をわかりやすい形で出力する．
void
SopCube::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  SopMgr mgr(mVariableNum);
  mgr.print(s, mBody, 0, 1, varname_list);
}

END_NAMESPACE_YM_LOGIC
