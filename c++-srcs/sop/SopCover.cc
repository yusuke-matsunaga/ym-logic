
/// @file SopCover.cc
/// @brief SopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/Range.h"
#include "SopMgr.h"
#include "GenFactor.h"


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
) : SopBase{var_num}
{
  // cube_list のキューブのサイズが正しいかチェック
  for ( auto& cube: cube_list ) {
    if ( cube.variable_num() != variable_num() ) {
      throw std::invalid_argument{"variable_num of cube mismatch"};
    }
  }
  SopMgr mgr{variable_num()};
  auto dst = mgr.new_block(cube_list);
  _set(std::move(dst));
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num,
  const vector<vector<Literal>>& cube_list
) : SopBase{var_num}
{
  SopMgr mgr{variable_num()};
  auto dst = mgr.new_block(cube_list);
  _set(std::move(dst));
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType var_num,
  std::initializer_list<std::initializer_list<Literal>>& cube_list
) : SopBase{var_num}
{
  SopMgr mgr{variable_num()};
  auto dst = mgr.new_block(cube_list);
  _set(std::move(dst));
}

// @brief コピーコンストラクタ
SopCover::SopCover(
  const SopCover& src
) : SopBase{src.variable_num()},
    mCubeNum{src.mCubeNum},
    mBody{src.mBody}
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
    mBody = src.mBody;
  }

  return *this;
}

// @brief ムーブコンストラクタ
SopCover::SopCover(
  SopCover&& src
) : SopBase{src},
    mCubeNum{src.mCubeNum},
    mBody{std::move(src.mBody)}
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
  std::swap(mBody, src.mBody);

  return *this;
}

// @brief キューブからのコピー変換コンストラクタ
SopCover::SopCover(
  const SopCube& cube
) : SopBase{cube.variable_num()},
    mCubeNum{1},
    mBody{cube.body()}
{
}

#if 0
// @brief キューブからのムーブ変換コンストラクタ
SopCover::SopCover(
  SopCube&& cube
) : mVariableNum{cube.variable_num()},
    mCubeNum{1},
    mBody{std::move(cube.mBody)}
{
}
#endif

// @brief 内容を指定したコンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  SizeType cube_num,
  SopBitVect&& body
) : SopBase{variable_num},
    mCubeNum{cube_num},
    mBody{body}
{
}

// @brief リテラル数を返す．
SizeType
SopCover::literal_num() const
{
  SopMgr mgr{variable_num()};
  return mgr.literal_num(to_block());
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
SopCover::literal_num(
  Literal lit
) const
{
  SopMgr mgr{variable_num()};
  return mgr.literal_num(to_block(), lit);
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
SopCover::literal_num(
  SizeType varid,
  bool inv
) const
{
  SopMgr mgr{variable_num()};
  return mgr.literal_num(to_block(), varid, inv);
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
SopCover::literal_list() const
{
  SopMgr mgr{variable_num()};
  return mgr.literal_list(to_block());
}

// @brief キューブを取り出す．
SopCube
SopCover::get_cube(
  SizeType cube_id
) const
{
  SopMgr mgr{variable_num()};
  return mgr.get_cube(to_block(), cube_id);
}

// @brief パタンを返す．
SopPat
SopCover::get_pat(
  SizeType cube_id,
  SizeType var
) const
{
  SopMgr mgr{variable_num()};
  auto src_cube = _cube_begin(mBody, cube_id);
  return mgr.get_pat(src_cube, var);
}

// @brief "quick factor" を行う．
Expr
SopCover::quick_factor() const
{
  QuickFactor qf;
  return qf(*this);
}

// @brief "good factor" を行う．
Expr
SopCover::good_factor() const
{
  GoodFactor gf;
  return gf(*this);
}

// @brief Expr に変換する．
Expr
SopCover::expr() const
{
  SopMgr mgr{variable_num()};
  return mgr.to_expr(to_block());
}

// @brief SopBlockRef に変換する．
SopBlockRef
SopCover::to_block() const
{
  return SopBlockRef{mCubeNum, mBody};
}

// @brief ハッシュ値を返す．
SizeType
SopCover::hash() const
{
  SopMgr mgr{variable_num()};
  return mgr.hash(to_block());
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
  mgr.debug_print(s, to_block());
}

// @brief SopBlock の内容をセットする．
void
SopCover::_set(
  SopBlock&& src
)
{
  mCubeNum = src.cube_num;
  std::swap(mBody, src.body);
}

END_NAMESPACE_YM_SOP
