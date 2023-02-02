
/// @file LitSet.cc
/// @brief LitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "SopMgr.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス LitSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num
) : mVariableNum{variable_num},
    mBody{nullptr}
{
  SopMgr mgr{mVariableNum};
  mBody = mgr.new_cube();
  mgr.cube_clear(mBody);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num,
  Literal lit
) : LitSet{variable_num}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literal(mBody, lit);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num,
  const vector<Literal>& lit_list
) : LitSet{variable_num}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literals(mBody, lit_list);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num,
  std::initializer_list<Literal>& lit_list
) : LitSet{variable_num}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literals(mBody, lit_list);
}

// @brief コピーコンストラクタ
LitSet::LitSet(
  const LitSet& src
) : LitSet{src.mVariableNum}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_copy(mBody, src.mBody);
}

// @brief 代入演算子
LitSet&
LitSet::operator=(
  const LitSet& src
)
{
  if ( &src != this ) {
    SopMgr mgr(src.mVariableNum);
    if ( mVariableNum != src.mVariableNum ) {
      SopMgr::delete_cube(mBody);
      mVariableNum = src.mVariableNum;
      mBody = mgr.new_cube();
    }
    mgr.cube_copy(mBody, src.mBody);
  }

  return *this;
}

// @brief ムーブコンストラクタ
LitSet::LitSet(
  LitSet&& src
) : mVariableNum{src.mVariableNum},
    mBody{src.mBody}
{
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
LitSet&
LitSet::operator=(
  LitSet&& src
)
{
  SopMgr::delete_cube(mBody);

  mVariableNum = src.mVariableNum;
  mBody = src.mBody;

  src.mBody = nullptr;

  return *this;
}

// @brief デストラクタ
LitSet::~LitSet()
{
  SopMgr::delete_cube(mBody);
}

// @brief 該当するリテラルが含まれているか調べる．
bool
LitSet::is_in(
  Literal lit
) const
{
  SopMgr mgr(mVariableNum);
  return mgr.litset_check(mBody, lit);
}

// @brief ユニオン演算付き代入
LitSet&
LitSet::operator+=(
  const LitSet& right
)
{
  SopMgr mgr(mVariableNum);
  mgr.litset_union(mBody, right.mBody);

  return *this;
}

// @brief 要素を足す．
LitSet&
LitSet::operator+=(
  Literal lit
)
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literal(mBody, lit);

  return *this;
}

// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
bool
LitSet::check_intersect(
  const SopCube& right
) const
{
  if ( mVariableNum != right.variable_num() ) {
    throw std::invalid_argument{"variable_num() mismatch"};
  }

  SopMgr mgr(mVariableNum);
  //return mgr.cube_check_intersect(mBody, right.mBody);
  return false;
}

END_NAMESPACE_YM_SOP
