
/// @file SopCover.cc
/// @brief SopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Range.h"
#include "SopBlock.h"
#include "SopMgr.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SopCover::SopCover(
  SizeType variable_num
) : mVariableNum{variable_num},
    mCubeNum{0},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  const vector<SopCube>& cube_list
) : mVariableNum{variable_num},
    mCubeNum{cube_list.size()},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.cover_set(mBody, cube_list);
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  const vector<vector<Literal>>& cube_list
) : mVariableNum{variable_num},
    mCubeNum{cube_list.size()},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.cover_set(mBody, cube_list);
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  std::initializer_list<std::initializer_list<Literal>>& cube_list
) : mVariableNum{variable_num},
    mCubeNum{cube_list.size()},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.cover_set(mBody, cube_list);
}

// @brief コピーコンストラクタ
SopCover::SopCover(
  const SopCover& src
) : mVariableNum{src.mVariableNum},
    mCubeNum{src.mCubeNum},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.cover_copy(mBody, src.mBody, mCubeNum);
}

// @brief コピー代入演算子
SopCover&
SopCover::operator=(
  const SopCover& src
)
{
  if ( this != &src ) {
    SizeType n = src.cube_num();
    if ( mCubeCap < n ) {
      SopMgr mgr(mVariableNum);
      mgr.delete_body(mBody, mCubeCap);
    }
    mVariableNum = src.mVariableNum;
    mCubeNum = src.mCubeNum;
    mCubeCap = mCubeNum;;
    SopMgr mgr(mVariableNum);
    mBody = mgr.new_body(mCubeCap);
    mgr.cover_copy(mBody, src.mBody, mCubeNum);
  }

  return *this;
}

// @brief ムーブコンストラクタ
SopCover::SopCover(
  SopCover&& src
) : mVariableNum{src.mVariableNum},
    mCubeNum{src.mCubeNum},
    mCubeCap{src.mCubeCap},
    mBody{src.mBody}
{
  src.mCubeNum = 0;
  src.mCubeCap = 0;
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
SopCover&
SopCover::operator=(
  SopCover&& src
)
{
  SopMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);

  mVariableNum = src.mVariableNum;
  mCubeNum = src.mCubeNum;
  mCubeCap = src.mCubeCap;
  mBody = src.mBody;

  src.mCubeNum = 0;
  src.mCubeCap = 0;
  src.mBody = nullptr;

  return *this;
}

// @brief キューブからのコピー変換コンストラクタ
SopCover::SopCover(
  const SopCube& cube
) : mVariableNum{cube.variable_num()},
    mCubeNum{1},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.cube_copy(mBody, cube);
}

// @brief キューブからのムーブ変換コンストラクタ
SopCover::SopCover(
  SopCube&& cube
) : mVariableNum{cube.variable_num()},
    mCubeNum{1},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.cube_move(cube);
}

// @brief 内容を指定したコンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  SizeType cube_num,
  SizeType cube_cap,
  SopBitVect* body
) : mVariableNum{variable_num},
    mCubeNum{cube_num},
    mCubeCap{cube_cap},
    mBody{body}
{
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
SopCover::~SopCover()
{
  SopMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);
}

// @brief リテラル数を返す．
SizeType
SopCover::literal_num() const
{
  SopMgr mgr(mVariableNum);
  return mgr.literal_num(block());
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
SopCover::literal_num(
  Literal lit
) const
{
  SopMgr mgr(mVariableNum);
  return mgr.literal_num(block(), lit);
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
SopCover::to_literal_list() const
{
  vector<vector<Literal>> cube_list(mCubeNum);

  SopMgr mgr(mVariableNum);
  for ( int i: Range(mCubeNum) ) {
    vector<Literal>& tmp_list = cube_list[i];
    tmp_list.reserve(mVariableNum);
    for ( int j: Range(mVariableNum) ) {
      VarId var(j);
      SopPat pat = mgr.get_pat(mBody, i, var);
      if ( pat == SopPat::_1 ) {
	tmp_list.push_back(Literal(var, false));
      }
      else if ( pat == SopPat::_0 ) {
	tmp_list.push_back(Literal(var, true));
      }
    }
  }

  return cube_list;
}

// @brief パタンを返す．
SopPat
SopCover::get_pat(
  SizeType cube_id,
  VarId var
) const
{
  SopMgr mgr(mVariableNum);
  return mgr.get_pat(mBody, cube_id, var);
}

// @brief 内容を表す SopBlock を返す．
inline
SopBlock
SopCover::block() const
{
  return SopBlock{mCubeNum, mBody};
}

// @brief 論理和を計算する．
SopCover
SopCover::operator+(
  const SopCover& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum + right.mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_sum(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 論理和を計算して代入する．
SopCover&
SopCover::operator+=(
  const SopCover& right
)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum + right.mCubeNum;
  // 新しいブロックを作る．
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_sum(body, block(), right.block());

  mgr.delete_body(mBody, mCubeNum);

  mCubeNum = nc;
  mCubeCap = cap;
  mBody = body;

  return *this;
}

// @brief 論理和を計算する(キューブ版)．
SopCover
SopCover::operator+(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum + 1;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_sum(body, block(), right.block());
  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 論理和を計算して代入する(キューブ版)．
SopCover&
SopCover::operator+=(
  const SopCube& right
)
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum + 1;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_sum(body, block(), right.block());

  mgr.delete_body(mBody, mCubeNum);

  mCubeNum = nc;
  mCubeCap = cap;
  mBody = body;

  return *this;
}

// @brief 差分を計算する．
SopCover
SopCover::operator-(
  const SopCover& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_diff(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 差分を計算して代入する．
SopCover&
SopCover::operator-=(
  const SopCover& right
)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  // キューブ数は増えないのでブロックはそのまま
  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_diff(mBody, block(), right.block());

  return *this;
}

// @brief 差分を計算する(キューブ版)．
SopCover
SopCover::operator-(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_diff(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 差分を計算して代入する(キューブ版)．
SopCover&
SopCover::operator-=(
  const SopCube& right
)
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_diff(mBody, block(), right.block());

  return *this;
}

// @brief 論理積を計算する．
SopCover
SopCover::operator*(
  const SopCover& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum * right.mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_product(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 論理積を計算して代入する．
SopCover&
SopCover::operator*=(
  const SopCover& right
)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum * right.mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_product(body, block(), right.block());

  mgr.delete_body(mBody, mCubeNum);

  mCubeNum = nc;
  mCubeCap = cap;
  mBody = body;

  return *this;
}

// @brief 論理積を計算する(キューブ版)．
SopCover
SopCover::operator*(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_product(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 論理積を計算して代入する(キューブ版)．
SopCover&
SopCover::operator*=(
  const SopCube& right
)
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_product(mBody, block(), right.block());

  return *this;
}

// @brief 論理積を計算する(リテラル版)．
SopCover
SopCover::operator*(
  Literal right
) const
{
  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_product(body, block(), right);

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief 論理積を計算して代入する(リテラル版)．
SopCover&
SopCover::operator*=(
  Literal right
)
{
  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_product(mBody, block(), right);

  return *this;
}

// @brief algebraic division を計算する．
SopCover
SopCover::operator/(
  const SopCover& right
) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_quotient(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief algebraic division を行って代入する．
SopCover&
SopCover::operator/=(
  const SopCover& right
)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_quotient(mBody, block(), right.block());

  return *this;
}

// @brief キューブによる商を計算する．
SopCover
SopCover::operator/(
  const SopCube& right
) const
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_quotient(body, block(), right.block());

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief キューブによる商を計算する．
SopCover&
SopCover::operator/=(
  const SopCube& right
)
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_quotient(mBody, block(), right.block());

  return *this;
}

// @brief リテラルによる商を計算する．
SopCover
SopCover::operator/(
  Literal lit
) const
{
  SopMgr mgr(mVariableNum);
  SizeType cap = mCubeNum;
  SopBitVect* body = mgr.new_body(cap);
  SizeType nc = mgr.cover_quotient(body, block(), lit);

  return SopCover(mVariableNum, nc, cap, body);
}

// @brief リテラルによる商を計算して代入する．
SopCover&
SopCover::operator/=(
  Literal lit
)
{
  SopMgr mgr(mVariableNum);
  mCubeNum = mgr.cover_quotient(mBody, block(), lit);

  return *this;
}

// @brief 共通なキューブを返す．
SopCube
SopCover::common_cube() const
{
  SopMgr mgr(mVariableNum);
  return mgr.common_cube(block());
}

// @brief Expr に変換する．
Expr
SopCover::to_expr() const
{
  SopMgr mgr{mVariableNum};
  return mgr.to_expr(mBody, mCubeNum);
}

// @brief ハッシュ値を返す．
SizeType
SopCover::hash() const
{
  SopMgr mgr(mVariableNum);
  return mgr.hash(block());
}

// @brief 内容をわかりやすい形で出力する．
void
SopCover::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  SopMgr mgr(mVariableNum);
  mgr.print(s, mBody, 0, mCubeNum, varname_list);
}

// @relates SopCover
int
compare(
  const SopCover& left,
  const SopCover& right
)
{
  ASSERT_COND( left.mVariableNum == right.mVariableNum );

  SopMgr mgr(left.mVariableNum);

  return mgr.cover_compare(left.block(), right.block());
}

END_NAMESPACE_YM_LOGIC
