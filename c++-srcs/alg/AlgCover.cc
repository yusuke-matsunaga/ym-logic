
/// @file AlgCover.cc
/// @brief AlgCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"
#include "ym/Range.h"
#include "GenFactor.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num
) : AlgBase{var_num},
    mCubeNum{0}
{
}

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num,
  const vector<AlgCube>& cube_list
) : AlgBase{var_num}
{
  // cube_list のキューブのサイズが正しいかチェック
  for ( auto& cube: cube_list ) {
    if ( cube.variable_num() != variable_num() ) {
      throw std::invalid_argument{"variable_num of cube mismatch"};
    }
  }
  auto dst = new_block(cube_list);
  _set(std::move(dst));
}

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num,
  const vector<vector<Literal>>& cube_list
) : AlgBase{var_num}
{
  auto dst = new_block(cube_list);
  _set(std::move(dst));
}

// @brief コンストラクタ
AlgCover::AlgCover(
  SizeType var_num,
  std::initializer_list<std::initializer_list<Literal>>& cube_list
) : AlgBase{var_num}
{
  auto dst = new_block(cube_list);
  _set(std::move(dst));
}

// @brief コピーコンストラクタ
AlgCover::AlgCover(
  const AlgCover& src
) : AlgBase{src.variable_num()},
    mCubeNum{src.mCubeNum},
    mBody{src.mBody}
{
}

// @brief コピー代入演算子
AlgCover&
AlgCover::operator=(
  const AlgCover& src
)
{
  if ( this != &src ) {
    AlgBase::operator=(src);
    mCubeNum = src.mCubeNum;
    mBody = src.mBody;
  }

  return *this;
}

// @brief ムーブコンストラクタ
AlgCover::AlgCover(
  AlgCover&& src
) : AlgBase{std::move(src)},
    mCubeNum{src.mCubeNum},
    mBody{std::move(src.mBody)}
{
}

// @brief ムーブ代入演算子
AlgCover&
AlgCover::operator=(
  AlgCover&& src
)
{
  AlgBase::operator=(std::move(src));
  mCubeNum = src.mCubeNum;
  std::swap(mBody, src.mBody);

  return *this;
}

// @brief キューブからのコピー変換コンストラクタ
AlgCover::AlgCover(
  const AlgCube& cube
) : AlgBase{cube.variable_num()},
    mCubeNum{1},
    mBody{cube.body()}
{
}

#if 0
// @brief キューブからのムーブ変換コンストラクタ
AlgCover::AlgCover(
  AlgCube&& cube
) : mVariableNum{cube.variable_num()},
    mCubeNum{1},
    mBody{std::move(cube.mBody)}
{
}
#endif

// @brief 内容を指定したコンストラクタ
AlgCover::AlgCover(
  SizeType variable_num,
  SizeType cube_num,
  AlgBitVect&& body
) : AlgBase{variable_num},
    mCubeNum{cube_num},
    mBody{std::move(body)}
{
}

// @brief リテラル数を返す．
SizeType
AlgCover::literal_num() const
{
  return _literal_num(to_block());
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
AlgCover::literal_num(
  Literal lit
) const
{
  return _literal_num(to_block(), lit);
}

// @brief 指定されたリテラルの出現回数を返す．
SizeType
AlgCover::literal_num(
  SizeType varid,
  bool inv
) const
{
  return _literal_num(to_block(), varid, inv);
}

// @brief 内容をリテラルのリストのリストに変換する．
vector<vector<Literal>>
AlgCover::literal_list() const
{
  return _literal_list(to_block());
}

// @brief キューブを取り出す．
AlgCube
AlgCover::get_cube(
  SizeType cube_id
) const
{
  return _get_cube(to_block(), cube_id);
}

// @brief パタンを返す．
AlgPat
AlgCover::get_pat(
  SizeType cube_id,
  SizeType var
) const
{
  auto src_cube = _cube_begin(mBody, cube_id);
  return _get_pat(src_cube, var);
}

// @brief "quick factor" を行う．
Expr
AlgCover::quick_factor() const
{
  QuickFactor qf;
  return qf(*this);
}

// @brief "good factor" を行う．
Expr
AlgCover::good_factor() const
{
  GoodFactor gf;
  return gf(*this);
}

// @brief Expr に変換する．
Expr
AlgCover::expr() const
{
  return _to_expr(to_block());
}

// @brief AlgBlockRef に変換する．
AlgBlockRef
AlgCover::to_block() const
{
  return AlgBlockRef{mCubeNum, mBody};
}

// @brief ハッシュ値を返す．
SizeType
AlgCover::hash() const
{
  return _hash(to_block());
}

// @brief 内容をわかりやすい形で出力する．
void
AlgCover::print(
  ostream& s,
  const vector<string>& varname_list
) const
{
  _print(s, mBody, 0, mCubeNum, varname_list);
}

// @brief 内容をデバッグ用に出力する．
void
AlgCover::debug_print(
  ostream& s
) const
{
  _debug_print(s, to_block());
}

// @brief AlgBlock の内容をセットする．
void
AlgCover::_set(
  AlgBlock&& src
)
{
  mCubeNum = src.cube_num;
  std::swap(mBody, src.body);
}

END_NAMESPACE_YM_ALG
