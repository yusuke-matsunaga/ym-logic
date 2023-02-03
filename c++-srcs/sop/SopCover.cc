
/// @file SopCover.cc
/// @brief SopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/Range.h"
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
) : mVariableNum{variable_num}
{
  SopMgr mgr(mVariableNum);
  auto dst = mgr.new_block(cube_list);
  _set(dst);
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  const vector<vector<Literal>>& cube_list
) : mVariableNum{variable_num}
{
  SopMgr mgr(mVariableNum);
  auto dst = mgr.new_block(cube_list);
  _set(dst);
}

// @brief コンストラクタ
SopCover::SopCover(
  SizeType variable_num,
  std::initializer_list<std::initializer_list<Literal>>& cube_list
) : mVariableNum{variable_num}
{
  SopMgr mgr(mVariableNum);
  auto dst = mgr.new_block(cube_list);
  _set(dst);
}

// @brief コピーコンストラクタ
SopCover::SopCover(
  const SopCover& src
) : mVariableNum{src.mVariableNum}
{
  SopMgr mgr{mVariableNum};
  auto dst = mgr.new_block(src.block());
  _set(dst);
}

// @brief コピー代入演算子
SopCover&
SopCover::operator=(
  const SopCover& src
)
{
  if ( this != &src ) {
    mVariableNum = src.mVariableNum;
    SopMgr mgr(mVariableNum);
    SizeType n = src.cube_num();
    auto src_block = src.block();
    if ( mCubeCap < n ) {
      delete_body();
      auto dst = mgr.new_block(src_block);
      _set(dst);
    }
    else {
      auto dst = block();
      mgr.copy_block(dst, src_block);
      _set(dst);
    }
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

  delete_body();

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
) : mVariableNum{cube.variable_num()}
{
  SopMgr mgr(mVariableNum);
  auto dst = mgr.new_block(cube.block());
  _set(dst);
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
  delete_body();
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
      auto pat = mgr.get_pat(mBody, i, var);
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

// @brief mBody の領域を削除する．
void
SopCover::delete_body()
{
  SopMgr::delete_bv(mBody);
}

// @brief 内容を表す SopBlock を返す．
SopBlock
SopCover::block() const
{
  return SopBlock{mCubeNum, mCubeCap, mBody};
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

// @brief SopBlock の内容をセットする．
void
SopCover::_set(
  const SopBlock& src
)
{
  mCubeNum = src.cube_num;
  mCubeCap = src.capacity;
  mBody = src.body;
}

END_NAMESPACE_YM_SOP
