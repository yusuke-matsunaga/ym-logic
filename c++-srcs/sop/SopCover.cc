
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


BEGIN_NAMESPACE_YM_SOP

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
  auto dst_block = mgr.new_cover(cube_list);
  mBody = dst_block.body;
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
  auto dst_block = mgr.new_cover(cube_list);
  mBody = dst_block.body;
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
  auto dst_block = mgr.new_cover(cube_list);
  mBody = dst_block.body;
}

// @brief コピーコンストラクタ
SopCover::SopCover(
  const SopCover& src
) : mVariableNum{src.mVariableNum},
    mCubeNum{src.mCubeNum},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  SopMgr mgr{mVariableNum};
  auto dst_block = mgr.new_cover(src.block());
  mBody = dst_block.body;
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
      SopMgr::delete_cube(mBody);
    }
    mVariableNum = src.mVariableNum;
    mCubeNum = src.mCubeNum;
    mCubeCap = mCubeNum;;
    SopMgr mgr(mVariableNum);
    auto dst_block = mgr.new_cover(src.block());
    mBody = dst_block.body;
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
  SopMgr::delete_cube(mBody);

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
  auto dst_block = mgr.new_cover(cube.block());
  mBody = dst_block.body;
}

// @brief キューブからのムーブ変換コンストラクタ
SopCover::SopCover(
  SopCube&& cube
) : mVariableNum{cube.variable_num()},
    mCubeNum{1},
    mCubeCap{mCubeNum},
    mBody{nullptr}
{
  mBody = SopMgr::cube_extract(cube);
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
  SopMgr::delete_cube(mBody);
}

// @brief リテラル数を返す．
SizeType
SopCover::literal_num() const
{
  SopMgr mgr{variable_num()};
  return mgr.literal_num(block());
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
SopCover::literal_num(
  Literal lit
) const
{
  SopMgr mgr{variable_num()};
  return mgr.literal_num(block(), lit);
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
SopCover::literal_list() const
{
  vector<vector<Literal>> cube_list(mCubeNum);

  SopMgr mgr{variable_num()};
  for ( SizeType i: Range(mCubeNum) ) {
    auto& tmp_list = cube_list[i];
    tmp_list.reserve(mVariableNum);
    for ( SizeType var: Range(mVariableNum) ) {
      SopPat pat = mgr.get_pat(mBody, i, var);
      if ( pat == SopPat::_1 ) {
	tmp_list.push_back(Literal{var, false});
      }
      else if ( pat == SopPat::_0 ) {
	tmp_list.push_back(Literal{var, true});
      }
    }
  }

  return cube_list;
}

// @brief パタンを返す．
SopPat
SopCover::get_pat(
  SizeType cube_id,
  SizeType var
) const
{
  SopMgr mgr{variable_num()};
  return mgr.get_pat(mBody, cube_id, var);
}

// @brief 内容を表す SopBlock を返す．
inline
SopBlock
SopCover::block() const
{
  return SopBlock{mCubeNum, mBody};
}

// @brief 否定を計算する．
SopCover
SopCover::operator~() const
{
  SopMgr mgr{variable_num()};
  auto ans_block = mgr.cover_complement(block());
  return mgr.make_cover(ans_block);
}

// @brief 論理和を計算する．
SopCover
SopCover::operator+(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto ans_block = mgr.cover_sum(block(), right.block());
  return mgr.make_cover(ans_block);
}

// @brief 論理和を計算して代入する．
SopCover&
SopCover::operator+=(
  const SopCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_sum(block(), right.block());

  SopMgr::delete_cube(mBody);

  mCubeNum = dst_block.cube_num;
  mCubeCap = mCubeNum;
  mBody = dst_block.body;

  return *this;
}

// @brief 論理和を計算する(キューブ版)．
SopCover
SopCover::operator+(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_sum(block(), right.block());
  return mgr.make_cover(dst_block);
}

// @brief 論理和を計算して代入する(キューブ版)．
SopCover&
SopCover::operator+=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_sum(block(), right.block());

  mgr.delete_cube(mBody);

  mCubeNum = dst_block.cube_num;
  mCubeCap = mCubeNum;
  mBody = dst_block.body;

  return *this;
}

// @brief 差分を計算する．
SopCover
SopCover::operator-(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_diff(block(), right.block());

  return mgr.make_cover(dst_block);
}

// @brief 差分を計算して代入する．
SopCover&
SopCover::operator-=(
  const SopCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  // キューブ数は増えないのでブロックはそのまま
  SopMgr mgr{variable_num()};
  auto dst_block = block();
  mgr.cover_diff_int(dst_block, right.block());
  mCubeNum = dst_block.cube_num;

  return *this;
}

// @brief 差分を計算する(キューブ版)．
SopCover
SopCover::operator-(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_diff(block(), right.block());

  return mgr.make_cover(dst_block);
}

// @brief 差分を計算して代入する(キューブ版)．
SopCover&
SopCover::operator-=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = block();
  mgr.cover_diff_int(dst_block, right.block());
  mCubeNum = dst_block.cube_num;

  return *this;
}

// @brief 論理積を計算する．
SopCover
SopCover::operator*(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_product(block(), right.block());

  return mgr.make_cover(dst_block);
}

// @brief 論理積を計算して代入する．
SopCover&
SopCover::operator*=(
  const SopCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_product(block(), right.block());

  mgr.delete_cube(mBody);

  mCubeNum = dst_block.cube_num;
  mCubeCap = mCubeNum;
  mBody = dst_block.body;

  return *this;
}

// @brief 論理積を計算する(キューブ版)．
SopCover
SopCover::operator*(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_product(block(), right.block());

  return mgr.make_cover(dst_block);
}

// @brief 論理積を計算して代入する(キューブ版)．
SopCover&
SopCover::operator*=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = block();
  mgr.cover_product_int(dst_block, right.block().body);
  mCubeNum = dst_block.cube_num;

  return *this;
}

// @brief 論理積を計算する(リテラル版)．
SopCover
SopCover::operator*(
  Literal right
) const
{
  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_product(block(), right);

  return mgr.make_cover(dst_block);
}

// @brief 論理積を計算して代入する(リテラル版)．
SopCover&
SopCover::operator*=(
  Literal right
)
{
  SopMgr mgr{variable_num()};
  auto dst_block = block();
  mgr.cover_product_int(dst_block, right);
  mCubeNum = dst_block.cube_num;

  return *this;
}

// @brief algebraic division を計算する．
SopCover
SopCover::operator/(
  const SopCover& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_quotient(block(), right.block());

  return mgr.make_cover(dst_block);
}

// @brief algebraic division を行って代入する．
SopCover&
SopCover::operator/=(
  const SopCover& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_quotient(block(), right.block());

  mgr.delete_cube(mBody);

  mCubeNum = dst_block.cube_num;
  mCubeCap = mCubeNum;
  mBody = dst_block.body;

  return *this;
}

// @brief キューブによる商を計算する．
SopCover
SopCover::operator/(
  const SopCube& right
) const
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_quotient(block(), right.block());

  return mgr.make_cover(dst_block);
}

// @brief キューブによる商を計算する．
SopCover&
SopCover::operator/=(
  const SopCube& right
)
{
  if ( variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{variable_num()};
  auto dst_block = block();
  mgr.cover_quotient_int(dst_block, right.block().body);
  mCubeNum = dst_block.cube_num;

  return *this;
}

// @brief リテラルによる商を計算する．
SopCover
SopCover::operator/(
  Literal lit
) const
{
  SopMgr mgr{variable_num()};
  auto dst_block = mgr.cover_quotient(block(), lit);

  return mgr.make_cover(dst_block);
}

// @brief リテラルによる商を計算して代入する．
SopCover&
SopCover::operator/=(
  Literal lit
)
{
  SopMgr mgr{variable_num()};
  auto dst_block = block();
  mgr.cover_quotient_int(dst_block, lit);
  mCubeNum = dst_block.cube_num;

  return *this;
}

// @brief 共通なキューブを返す．
SopCube
SopCover::common_cube() const
{
  SopMgr mgr{variable_num()};
  auto bv = mgr.common_cube(block());
  return mgr.make_cube(bv);
}

// @brief Expr に変換する．
Expr
SopCover::expr() const
{
  SopMgr mgr{mVariableNum};
  return mgr.to_expr(block());
}

// @brief ハッシュ値を返す．
SizeType
SopCover::hash() const
{
  SopMgr mgr{variable_num()};
  return mgr.hash(block());
}

// @brief 内容をわかりやすい形で出力する．
void
SopCover::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  SopMgr mgr{variable_num()};
  mgr.print(s, mBody, 0, mCubeNum, varname_list);
}

// @brief 内容をデバッグ用に出力する．
void
SopCover::debug_print(
  ostream& s
) const
{
  SopMgr mgr{variable_num()};
  mgr.debug_print(s, mBody, mCubeNum);
}

// @relates SopCover
int
compare(
  const SopCover& left,
  const SopCover& right
)
{
  if ( left.variable_num() != right.variable_num() ) {
    throw std::invalid_argument("variable_num() is different from each other");
  }

  SopMgr mgr{left.mVariableNum};

  return mgr.cover_compare(left.block(), right.block());
}

END_NAMESPACE_YM_SOP
